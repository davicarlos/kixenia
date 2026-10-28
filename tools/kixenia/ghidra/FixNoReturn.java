// FixNoReturn.java - the save/restore register helpers (__savegprlr_NN, __savefpr_NN...) are
// reached by bl and return, but the analysis marks some as non-returning and cuts every caller
// short. Clears the flag on non-returning functions in the given range, disassembles the code
// after each call to them and fixes up the bodies of the calling functions.
// args: <first address> <last address>
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.util.*;

public class FixNoReturn extends GhidraScript {
  public void run() throws Exception {
    String[] a = getScriptArgs();
    Address lo = toAddr(Long.decode(a[0])), hi = toAddr(Long.decode(a[1]));
    Set<Function> callers = new HashSet<>();
    int fixed = 0, sites = 0;
    for (Function f : currentProgram.getFunctionManager().getFunctions(lo, true)) {
      if (f.getEntryPoint().compareTo(hi) > 0) break;
      if (!f.hasNoReturn()) continue;
      f.setNoReturn(false);
      fixed++;
    }
    // The helpers are entered in the middle too (__restfpr_NN), so look at every address.
    for (Address t = lo; t.compareTo(hi) <= 0; t = t.add(4)) {
      for (Reference r : getReferencesTo(t)) {
        if (!r.getReferenceType().isCall()) continue;
        Instruction i = getInstructionAt(r.getFromAddress());
        if (i == null) continue;
        i.setFlowOverride(FlowOverride.NONE);
        new DisassembleCommand(i.getFallThrough() != null ? i.getFallThrough() : r.getFromAddress().add(4), null, true)
            .applyTo(currentProgram, monitor);
        Function c = getFunctionContaining(r.getFromAddress());
        if (c != null) callers.add(c);
        sites++;
      }
    }
    for (Function c : callers) CreateFunctionCmd.fixupFunctionBody(currentProgram, c, monitor);
    println("FIXNORETURN " + fixed + " helpers, " + sites + " call sites, " + callers.size() + " functions");
  }
}
