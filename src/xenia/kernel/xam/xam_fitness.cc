/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2026 Xenia Canary. All rights reserved.                          *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/kernel/xam/xam_fitness.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <mutex>

#include "xenia/base/filesystem.h"
#include "xenia/base/logging.h"
#include "xenia/base/memory.h"
#include "xenia/emulator.h"
#include "xenia/kernel/kernel_state.h"
#include "xenia/kernel/util/shim_utils.h"
#include "xenia/kernel/xenumerator.h"
#include "xenia/kernel/xam/user_profile.h"
#include "xenia/kernel/xam/xam_private.h"
#include "xenia/xbox.h"

// The console's fitness profile lives with the profile's Xbox Live data: xam
// hands nearly every XamFitness* call to its Live service (XMsg app 0xFB,
// 0xB0079 to 0xB0089). Here the body profile and the fitness events a title
// adds are files in the storage root, which is enough for 4D53090F (Nike+
// Kinect Training): without XamFitnessInitialize its Kinect calorie tracking
// takes every player for 175 cm, 75 kg and 28 years old.

namespace xe {
namespace kernel {
namespace xam {

namespace {

std::mutex fitness_mutex;

std::filesystem::path GetFitnessPath(uint64_t xuid, const char* suffix) {
  return kernel_state()->emulator()->storage_root() / "fitness" /
         fmt::format("{:016X}{}", xuid, suffix);
}

// The signed-in profile of a user index, or null.
UserProfile* GetFitnessProfile(uint32_t user_index) {
  if (user_index >= XUserMaxUserCount) {
    return nullptr;
  }
  return kernel_state()->xam_state()->GetUserProfile(user_index);
}

// XamFitnessGetCurrentBodyProfileRecord's record (0x10 bytes), as 4D53090F
// reads it at 0x82F55B80 and 0x82F55718: the gender (2 for female), the
// height in meters, the weight in kilograms and the year of birth.
#pragma pack(push, 1)
struct X_FITNESS_BODY_PROFILE_RECORD {
  uint8_t version;
  uint8_t gender;
  xe::be<float> height_meters;
  xe::be<float> weight_kilograms;
  xe::be<uint16_t> birth_year;
  // When the record was made, in fitness time (the Guide's fitness app puts
  // the current time here before XamFitnessAddBodyProfileRecord).
  xe::be<uint32_t> msg_time;
};
#pragma pack(pop)
static_assert_size(X_FITNESS_BODY_PROFILE_RECORD, 0x10);

// Days since 1970-01-01 of a proleptic Gregorian date.
int64_t DaysFromCivil(int64_t year, uint32_t month, uint32_t day) {
  year -= month <= 2;
  const int64_t era = (year >= 0 ? year : year - 399) / 400;
  const uint32_t year_of_era = uint32_t(year - era * 400);
  const uint32_t day_of_year =
      (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1;
  const uint32_t day_of_era =
      year_of_era * 365 + year_of_era / 4 - year_of_era / 100 + day_of_year;
  return era * 146097 + int64_t(day_of_era) - 719468;
}

}  // namespace

std::optional<FitnessBodyProfile> LoadFitnessBodyProfile(uint64_t xuid) {
  std::lock_guard<std::mutex> lock(fitness_mutex);
  FILE* file = xe::filesystem::OpenFile(GetFitnessPath(xuid, ".body"), "rb");
  if (!file) {
    return std::nullopt;
  }
  X_FITNESS_BODY_PROFILE_RECORD record;
  const bool read =
      std::fread(&record, 1, sizeof(record), file) == sizeof(record);
  std::fclose(file);
  if (!read) {
    return std::nullopt;
  }
  FitnessBodyProfile profile;
  profile.female = record.gender == 2;
  profile.height_meters = record.height_meters;
  profile.weight_kilograms = record.weight_kilograms;
  profile.birth_year = record.birth_year;
  return profile;
}

bool SaveFitnessBodyProfile(uint64_t xuid, const FitnessBodyProfile& profile) {
  std::lock_guard<std::mutex> lock(fitness_mutex);
  const auto path = GetFitnessPath(xuid, ".body");
  std::error_code error;
  std::filesystem::create_directories(path.parent_path(), error);
  FILE* file = xe::filesystem::OpenFile(path, "wb");
  if (!file) {
    return false;
  }
  X_FITNESS_BODY_PROFILE_RECORD record = {};
  record.version = 1;
  record.gender = profile.female ? 2 : 1;
  record.height_meters = profile.height_meters;
  record.weight_kilograms = profile.weight_kilograms;
  record.birth_year = profile.birth_year;
  const bool written =
      std::fwrite(&record, 1, sizeof(record), file) == sizeof(record);
  std::fclose(file);
  XELOGI("Fitness: body profile of {:016X}: {}, {:.2f} m, {:.1f} kg, born {}",
         xuid, profile.female ? "female" : "male", profile.height_meters,
         profile.weight_kilograms, profile.birth_year);
  return written;
}

// Starts the fitness service (0x8168F4E0, message 0xB0080); the word it
// writes tells 4D53090F the service has data.
dword_result_t XamFitnessInitialize_entry(lpdword_t ready) {
  if (!ready) {
    return X_E_INVALIDARG;
  }
  *ready = 1;
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessInitialize, kNone, kImplemented);

dword_result_t XamFitnessUnInitialize_entry() { return X_E_SUCCESS; }
DECLARE_XAM_EXPORT1(XamFitnessUnInitialize, kNone, kImplemented);

// The user's body profile (0x8168F168, message 0xB007B), which they enter in
// XamShowFitnessBodyProfileUI; titles keep their defaults until then.
dword_result_t XamFitnessGetCurrentBodyProfileRecord_entry(dword_t user_index,
                                                           lpvoid_t buffer,
                                                           dword_t size) {
  if (user_index >= XUserMaxUserCount || !buffer || !(size & 0xFFFF)) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  const auto profile = LoadFitnessBodyProfile(user->xuid());
  if (!profile) {
    return X_HRESULT_FROM_WIN32(X_ERROR_NOT_FOUND);
  }
  X_FITNESS_BODY_PROFILE_RECORD record = {};
  record.version = 1;
  record.gender = profile->female ? 2 : 1;
  record.height_meters = profile->height_meters;
  record.weight_kilograms = profile->weight_kilograms;
  record.birth_year = profile->birth_year;
  std::memcpy(buffer, &record,
              std::min<size_t>(sizeof(record), size & 0xFFFF));
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessGetCurrentBodyProfileRecord, kNone,
                    kImplemented);

// A workout for the profile's fitness history (0x8168FB40, message 0xB007A).
// 4D53090F sends 0xC bytes at 0x82F55E48: the start in seconds since 2003,
// the duration in seconds, the calories, the kind of tracking and the
// intensity in tenths of a MET.
dword_result_t XamFitnessAddFitnessEvent_entry(dword_t user_index,
                                               lpvoid_t event, dword_t size) {
  if (user_index >= XUserMaxUserCount || !event) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  const uint32_t length = size & 0xFFFF;
  const uint8_t* data = event.as<const uint8_t*>();
  if (length >= 0xC) {
    XELOGI(
        "Fitness: workout of {:016X} at {}: {} s, {} kcal, kind {:02X}, "
        "{:.1f} MET",
        user->xuid(), xe::load_and_swap<uint32_t>(data),
        xe::load_and_swap<uint16_t>(data + 4),
        xe::load_and_swap<uint32_t>(data + 6), data[0xA], data[0xB] / 10.0f);
  }
  std::lock_guard<std::mutex> lock(fitness_mutex);
  const auto path = GetFitnessPath(user->xuid(), ".events");
  std::error_code error;
  std::filesystem::create_directories(path.parent_path(), error);
  FILE* file = xe::filesystem::OpenFile(path, "ab");
  if (!file) {
    return X_E_FAIL;
  }
  const uint16_t be_length = xe::byte_swap(uint16_t(length));
  std::fwrite(&be_length, 1, sizeof(be_length), file);
  std::fwrite(data, 1, length, file);
  std::fclose(file);
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessAddFitnessEvent, kNone, kImplemented);

// How the profile shares fitness data (0x8168F688): 0 without a profile, 1
// for one that isn't on Xbox Live, and for one on Live a setting that is 2
// until the player changes it.
dword_result_t XamFitnessGetPrivacySettings_entry(dword_t user_index,
                                                  lpdword_t privacy) {
  if (user_index >= XUserMaxUserCount || !privacy) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  *privacy = user->signin_state() ==
                     static_cast<uint32_t>(SignInState::SignedInToLive)
                 ? 2
                 : 1;
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessGetPrivacySettings, kNone, kImplemented);

// METs are kept in tenths (0x8168F5B0).
dword_result_t XamFitnessConvertFloatMetToByteMet_entry(double_t met,
                                                        lpvoid_t unused,
                                                        lpvoid_t byte_met) {
  if (!byte_met) {
    return X_E_INVALIDARG;
  }
  *byte_met.as<uint8_t*>() = uint8_t(int64_t(double(met) * 10.0));
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessConvertFloatMetToByteMet, kNone, kImplemented);

// Fitness times are seconds since 2003-01-01 (0x8168F5E8 and 0x8168FC78).
struct X_SYSTEMTIME {
  xe::be<uint16_t> year;
  xe::be<uint16_t> month;
  xe::be<uint16_t> day_of_week;
  xe::be<uint16_t> day;
  xe::be<uint16_t> hour;
  xe::be<uint16_t> minute;
  xe::be<uint16_t> second;
  xe::be<uint16_t> milliseconds;
};
static_assert_size(X_SYSTEMTIME, 0x10);

dword_result_t XamFitnessSystemTimeToMsgTime_entry(
    pointer_t<X_SYSTEMTIME> system_time, lpdword_t msg_time) {
  if (!system_time || !msg_time || system_time->month < 1 ||
      system_time->month > 12 || system_time->day < 1 ||
      system_time->day > 31) {
    return X_E_INVALIDARG;
  }
  const int64_t days = DaysFromCivil(system_time->year, system_time->month,
                                     system_time->day) -
                       DaysFromCivil(2003, 1, 1);
  *msg_time = uint32_t(days * 86400 + system_time->hour * 3600 +
                       system_time->minute * 60 + system_time->second);
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessSystemTimeToMsgTime, kNone, kImplemented);

dword_result_t XamFitnessMsgTimeToSystemTime_entry(
    dword_t msg_time, pointer_t<X_SYSTEMTIME> system_time) {
  if (!system_time) {
    return X_E_INVALIDARG;
  }
  int64_t days = msg_time / 86400 + DaysFromCivil(2003, 1, 1);
  const uint32_t seconds = msg_time % 86400;
  // Civil date from days since 1970-01-01.
  days += 719468;
  const int64_t era = (days >= 0 ? days : days - 146096) / 146097;
  const uint32_t day_of_era = uint32_t(days - era * 146097);
  const uint32_t year_of_era = (day_of_era - day_of_era / 1460 +
                                day_of_era / 36524 - day_of_era / 146096) /
                               365;
  const uint32_t day_of_year = day_of_era - (365 * year_of_era +
                                             year_of_era / 4 -
                                             year_of_era / 100);
  const uint32_t mp = (5 * day_of_year + 2) / 153;
  const uint32_t day = day_of_year - (153 * mp + 2) / 5 + 1;
  const uint32_t month = mp < 10 ? mp + 3 : mp - 9;
  const int64_t year = int64_t(year_of_era) + era * 400 + (month <= 2);
  system_time->year = uint16_t(year);
  system_time->month = uint16_t(month);
  system_time->day = uint16_t(day);
  // 2003-01-01 was a Wednesday.
  system_time->day_of_week = uint16_t((msg_time / 86400 + 3) % 7);
  system_time->hour = uint16_t(seconds / 3600);
  system_time->minute = uint16_t(seconds / 60 % 60);
  system_time->second = uint16_t(seconds % 60);
  system_time->milliseconds = 0;
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessMsgTimeToSystemTime, kNone, kImplemented);

// Below: the rest of the fitness service, which on a console is the Xbox
// Live fitness service (XMsg app 0xFB). Without it the body profile records
// and the events are the files above; the summaries and the event
// enumerator, whose formats only the service defines, come back empty.

namespace {

// Finishes a call the way xam does: through the overlapped when the title
// gave one (the result and the item count in its length), else directly.
X_HRESULT FinishFitness(uint32_t overlapped_ptr, X_HRESULT result,
                        uint32_t length = 0) {
  if (!overlapped_ptr) {
    return result;
  }
  kernel_state()->CompleteOverlappedImmediateEx(
      overlapped_ptr,
      XSUCCEEDED(result) ? X_ERROR_SUCCESS : X_ERROR_FUNCTION_FAILED, result,
      length);
  return X_HRESULT_FROM_WIN32(X_ERROR_IO_PENDING);
}

bool FitnessFileExists(uint64_t xuid, const char* suffix) {
  std::error_code error;
  return std::filesystem::exists(GetFitnessPath(xuid, suffix), error);
}

}  // namespace

// 0x8168F470 (0xB0086): like XamFitnessInitialize, for one user (the Guide's
// fitness app calls it when the sync status says the profile is new).
dword_result_t XamFitnessInitializeForOneUser_entry(dword_t user_index,
                                                    lpdword_t ready) {
  if (user_index > 6 || !ready) {
    return X_E_INVALIDARG;
  }
  *ready = 1;
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessInitializeForOneUser, kNone, kImplemented);

// 0x8168F830 (0xB0085): the profile's sync state with the service; the
// Guide's fitness app reads the body profile on 0, 2 and 4, and sets the
// profile up on 1. Here there is nothing to sync.
dword_result_t XamFitnessGetSyncStatus_entry(dword_t user_index,
                                             dword_t size, dword_t flags,
                                             lpdword_t status,
                                             dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount || size != 0x10 || !flags || !status) {
    return X_E_INVALIDARG;
  }
  *status = 0;
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessGetSyncStatus, kNone, kImplemented);

// 0x8168FA00 (0xB0079): a new body profile record becomes the current one;
// the earlier ones are kept for XamFitnessGetAllBodyProfileRecords.
dword_result_t XamFitnessAddBodyProfileRecord_entry(dword_t user_index,
                                                    lpvoid_t record_ptr,
                                                    dword_t size) {
  if (user_index >= XUserMaxUserCount || !record_ptr || !(size & 0xFFFF)) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  X_FITNESS_BODY_PROFILE_RECORD record = {};
  std::memcpy(&record, record_ptr.as<const uint8_t*>(),
              std::min<size_t>(size & 0xFFFF, sizeof(record)));
  FitnessBodyProfile profile;
  profile.female = record.gender == 2;
  profile.height_meters = record.height_meters;
  profile.weight_kilograms = record.weight_kilograms;
  profile.birth_year = record.birth_year;
  if (!SaveFitnessBodyProfile(user->xuid(), profile)) {
    return X_E_FAIL;
  }
  std::lock_guard<std::mutex> lock(fitness_mutex);
  FILE* file =
      xe::filesystem::OpenFile(GetFitnessPath(user->xuid(), ".bodies"), "ab");
  if (file) {
    std::fwrite(&record, 1, sizeof(record), file);
    std::fclose(file);
  }
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessAddBodyProfileRecord, kNone, kImplemented);

// 0x8168F328 (0xB007C): the body profile records, newest first, as many as
// fit; the count goes in the overlapped's length.
dword_result_t XamFitnessGetAllBodyProfileRecords_entry(
    dword_t user_index, dword_t unknown, lpvoid_t buffer, dword_t buffer_size,
    dword_t unknown_2, dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount || !buffer) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  std::vector<X_FITNESS_BODY_PROFILE_RECORD> records;
  {
    std::lock_guard<std::mutex> lock(fitness_mutex);
    FILE* file = xe::filesystem::OpenFile(
        GetFitnessPath(user->xuid(), ".bodies"), "rb");
    if (file) {
      X_FITNESS_BODY_PROFILE_RECORD record;
      while (std::fread(&record, 1, sizeof(record), file) == sizeof(record)) {
        records.push_back(record);
      }
      std::fclose(file);
    }
  }
  uint32_t count = 0;
  uint8_t* out = buffer.as<uint8_t*>();
  for (auto it = records.rbegin(); it != records.rend(); ++it) {
    if ((count + 1) * sizeof(*it) > buffer_size) {
      break;
    }
    std::memcpy(out + count * sizeof(*it), &*it, sizeof(*it));
    ++count;
  }
  return FinishFitness(overlapped_ptr, X_E_SUCCESS, count);
}
DECLARE_XAM_EXPORT1(XamFitnessGetAllBodyProfileRecords, kNone, kSketchy);

// 0x81698BF8: despite its name in the export table, this fills a 0x10 byte
// structure (whose first word must be 0x10) with three values xam caches
// about the console (0x81A815C4, 0x81A9F6AC, 0x81A815C0), unrelated to the
// fitness data; they aren't known, so they stay 0.
dword_result_t XamFitnessClearBodyProfileRecords_entry(lpdword_t info) {
  if (!info) {
    return X_E_INVALIDARG;
  }
  if (info[0] != 0x10) {
    return X_E_FAIL;
  }
  info[1] = 0;
  info[2] = 0;
  info[3] = 0;
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessClearBodyProfileRecords, kNone, kSketchy);

// 0x8168F0E0 (0xB0084): forgets the profile's fitness data.
dword_result_t XamFitnessClearAll_entry(dword_t user_index, dword_t unknown,
                                        dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  {
    std::lock_guard<std::mutex> lock(fitness_mutex);
    std::error_code error;
    for (const char* suffix : {".body", ".bodies", ".events"}) {
      std::filesystem::remove(GetFitnessPath(user->xuid(), suffix), error);
    }
  }
  XELOGI("Fitness: cleared the data of {:016X}", user->xuid());
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessClearAll, kNone, kImplemented);

// 0x8168F990 (0xB0089): whether the profile has any fitness data.
dword_result_t XamFitnessContainsFitnessData_entry(dword_t user_index,
                                                   lpdword_t contains,
                                                   dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount || !contains) {
    return X_E_INVALIDARG;
  }
  UserProfile* user = GetFitnessProfile(user_index);
  if (!user) {
    return X_E_FAIL;
  }
  *contains = FitnessFileExists(user->xuid(), ".body") ||
                      FitnessFileExists(user->xuid(), ".events")
                  ? 1
                  : 0;
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessContainsFitnessData, kNone, kImplemented);

// 0x8168F8C0: the privacy setting xam suggests, 6 unless the profile's
// region (setting 6) isn't the one compared at 0x815F4400, when it asks
// 0x816D9CE0 and suggests 3 or 4.
dword_result_t XamFitnessGetSuggestedPrivacySettings_entry(dword_t user_index,
                                                           lpdword_t privacy) {
  if (user_index >= XUserMaxUserCount || !privacy) {
    return X_E_INVALIDARG;
  }
  *privacy = 6;
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessGetSuggestedPrivacySettings, kNone,
                    kImplemented);

// 0x8168F568: tenths of a MET back to a float.
dword_result_t XamFitnessConvertByteMetToFloatMet_entry(dword_t byte_met,
                                                        lpvoid_t met) {
  if (!met) {
    return X_E_INVALIDARG;
  }
  xe::store_and_swap<float>(met.as<uint8_t*>(),
                            float(byte_met & 0xFF) * 0.1f);
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessConvertByteMetToFloatMet, kNone, kImplemented);

// The summaries (0xB007D, 0xB007E) are computed by the Live service in a
// layout only it knows; they come back empty, with the buffer cleared where
// the call names one.
dword_result_t XamFitnessGetOverallSummary_entry(dword_t user_index,
                                                 lpvoid_t buffer, dword_t size,
                                                 dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount || !buffer) {
    return X_E_INVALIDARG;
  }
  if (!GetFitnessProfile(user_index)) {
    return X_E_FAIL;
  }
  std::memset(buffer.as<uint8_t*>(), 0, size & 0xFFFF);
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessGetOverallSummary, kNone, kSketchy);

dword_result_t XamFitnessGetTitleSummary_entry(dword_t user_index,
                                               dword_t title_id,
                                               lpvoid_t buffer, dword_t size,
                                               dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount || !buffer) {
    return X_E_INVALIDARG;
  }
  if (!GetFitnessProfile(user_index)) {
    return X_E_FAIL;
  }
  std::memset(buffer.as<uint8_t*>(), 0, size & 0xFFFF);
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessGetTitleSummary, kNone, kSketchy);

dword_result_t XamFitnessGetTitleSummaries_entry(
    dword_t user_index, dword_t unknown_2, dword_t unknown_3,
    dword_t unknown_4, dword_t unknown_5, dword_t unknown_6,
    dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount) {
    return X_E_INVALIDARG;
  }
  if (!GetFitnessProfile(user_index)) {
    return X_E_FAIL;
  }
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessGetTitleSummaries, kNone, kSketchy);

dword_result_t XamFitnessGetAllTitleSummaries_entry(
    dword_t user_index, dword_t unknown_2, dword_t unknown_3,
    dword_t unknown_4, dword_t unknown_5, dword_t overlapped_ptr) {
  if (user_index >= XUserMaxUserCount) {
    return X_E_INVALIDARG;
  }
  if (!GetFitnessProfile(user_index)) {
    return X_E_FAIL;
  }
  return FinishFitness(overlapped_ptr, X_E_SUCCESS);
}
DECLARE_XAM_EXPORT1(XamFitnessGetAllTitleSummaries, kNone, kSketchy);

// 0x8168FCF0: an enumerator of fitness events (0xB007F through 0xB000B)
// from a query of at least 0x38 bytes: +4 flags (0x100 and 0x200 bound the
// time, 0x400, 0x800 and 0x1000 have fields of their own, and of 0x1800 one
// bit must be set), +5 period kind (1 to 5; 5 wants a count of 1), +8 and
// +0xC the bounds in fitness time, +0x28 the items per enumerate, +0x2C
// where the buffer size goes, +0x30 where the handle goes. Items are 0x24
// bytes in a layout the service defines, so the enumerator stays empty.
dword_result_t XamFitnessCreateFitnessEventEnumerator_entry(dword_t user_index,
                                                            lpvoid_t query) {
  if (user_index >= XUserMaxUserCount || !query) {
    return X_E_INVALIDARG;
  }
  const uint8_t* q = query.as<const uint8_t*>();
  uint32_t count = xe::load_and_swap<uint32_t>(q + 0x28);
  uint32_t size_ptr = xe::load_and_swap<uint32_t>(q + 0x2C);
  uint32_t handle_ptr = xe::load_and_swap<uint32_t>(q + 0x30);
  if (!count || !size_ptr || !handle_ptr) {
    return X_E_INVALIDARG;
  }
  if (!GetFitnessProfile(user_index)) {
    return X_E_FAIL;
  }
  uint16_t flags = xe::load_and_swap<uint16_t>(q + 4);
  uint8_t period = q[5];
  if (xe::load_and_swap<uint16_t>(q) < 0x38 || !(flags & 0x1800) ||
      (flags & 0x1800) == 0x1800 || !period || period > 5 ||
      (period == 5 && count != 1) ||
      ((flags & 0x800) && xe::load_and_swap<uint32_t>(q + 0x18) >= 4)) {
    return X_E_INVALIDARG;
  }
  auto e = object_ref<XStaticUntypedEnumerator>(
      new XStaticUntypedEnumerator(kernel_state(), count, 0x24));
  X_STATUS status = e->Initialize(user_index, 0xFB, 0xB007F, 0xB000B, 0);
  if (XFAILED(status)) {
    return X_E_FAIL;
  }
  xe::store_and_swap<uint32_t>(kernel_memory()->TranslateVirtual(size_ptr),
                               count * 0x24);
  xe::store_and_swap<uint32_t>(kernel_memory()->TranslateVirtual(handle_ptr),
                               e->handle());
  return X_E_SUCCESS;
}
DECLARE_XAM_EXPORT1(XamFitnessCreateFitnessEventEnumerator, kNone, kSketchy);

}  // namespace xam
}  // namespace kernel
}  // namespace xe

DECLARE_XAM_EMPTY_REGISTER_EXPORTS(Fitness);
