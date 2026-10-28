import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
public class FnGraph extends GhidraScript {
  public void run() throws Exception {
    String[] a = getScriptArgs();
    List<String> addrs = Files.readAllLines(Paths.get(a[0]));
    String out = a[1];
    DecompInterface d = new DecompInterface(); d.openProgram(currentProgram);
    StringBuilder js = new StringBuilder("[\n");
    boolean first = true;
    for (String s : addrs) {
      s = s.trim(); if (s.isEmpty()) continue;
      var ad = toAddr(s);
      Function f = getFunctionAt(ad);
      boolean created = false;
      if (f == null) {
        Function c = getFunctionContaining(ad);
        if (c == null || !c.getEntryPoint().equals(ad)) {
          if (getInstructionAt(ad) == null) new DisassembleCommand(ad, null, true).applyTo(currentProgram, monitor);
          if (c == null) { new CreateFunctionCmd(ad).applyTo(currentProgram, monitor); created = true; }
        }
        f = getFunctionAt(ad);
      }
      String entry = f == null ? "" : f.getEntryPoint().toString();
      StringBuilder callers = new StringBuilder(), callees = new StringBuilder();
      if (f != null) {
        for (Function c : f.getCallingFunctions(monitor)) callers.append("\"").append(c.getEntryPoint()).append("\",");
        for (Function c : f.getCalledFunctions(monitor)) callees.append("\"").append(c.getEntryPoint()).append("\",");
        var r = d.decompileFunction(f, 60, monitor);
        String code = (r != null && r.getDecompiledFunction() != null) ? r.getDecompiledFunction().getC() : "";
        Files.writeString(Paths.get(out, s + ".c"), code);
      }
      String cont = "";
      if (f == null) { Function c = getFunctionContaining(ad); if (c != null) cont = c.getEntryPoint().toString(); }
      js.append(first ? "" : ",\n").append(String.format("{\"addr\":\"%s\",\"entry\":\"%s\",\"created\":%b,\"thunk\":%b,\"size\":%d,\"inside\":\"%s\",\"callers\":[%s],\"callees\":[%s]}",
        s, entry, created, f != null && f.isThunk(), f == null ? 0 : f.getBody().getNumAddresses(), cont,
        callers.length() > 0 ? callers.substring(0, callers.length() - 1) : "", callees.length() > 0 ? callees.substring(0, callees.length() - 1) : ""));
      first = false;
    }
    js.append("\n]\n");
    Files.writeString(Paths.get(out, "graph.json"), js.toString());
    println("FNGRAPH done " + addrs.size());
  }
}
