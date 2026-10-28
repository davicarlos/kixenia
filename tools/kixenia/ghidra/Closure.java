// Closure.java - decompile every function reachable from the roots whose entry lies in
// [lo, hi), and write graph.json with sizes and callees.
// args: <output dir> <lo> <hi> <root>...
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class Closure extends GhidraScript {
  public void run() throws Exception {
    String[] a = getScriptArgs();
    Path out = Paths.get(a[0]);
    Files.createDirectories(out);
    long lo = Long.decode(a[1]), hi = Long.decode(a[2]);
    DecompInterface d = new DecompInterface();
    d.openProgram(currentProgram);
    Deque<Address> todo = new ArrayDeque<>();
    Set<Address> seen = new TreeSet<>();
    for (int i = 3; i < a.length; i++) todo.add(toAddr(Long.decode(a[i])));
    StringBuilder js = new StringBuilder("[\n");
    boolean first = true;
    while (!todo.isEmpty()) {
      Address ad = todo.poll();
      if (!seen.add(ad)) continue;
      Function f = getFunctionAt(ad);
      if (f == null) {
        if (getInstructionAt(ad) == null) new DisassembleCommand(ad, null, true).applyTo(currentProgram, monitor);
        if (getFunctionContaining(ad) == null) new CreateFunctionCmd(ad).applyTo(currentProgram, monitor);
        f = getFunctionAt(ad);
      }
      if (f == null) continue;
      StringBuilder callees = new StringBuilder();
      for (Function c : f.getCalledFunctions(monitor)) {
        long o = c.getEntryPoint().getOffset();
        callees.append("\"").append(c.getEntryPoint()).append("\",");
        if (o >= lo && o < hi && !seen.contains(c.getEntryPoint())) todo.add(c.getEntryPoint());
      }
      var r = d.decompileFunction(f, 90, monitor);
      String code = (r != null && r.getDecompiledFunction() != null) ? r.getDecompiledFunction().getC() : "";
      Files.writeString(out.resolve(String.format("0x%08x.c", ad.getOffset())), code);
      js.append(first ? "" : ",\n").append(String.format("{\"addr\":\"0x%08x\",\"size\":%d,\"callees\":[%s]}",
          ad.getOffset(), f.getBody().getNumAddresses(),
          callees.length() > 0 ? callees.substring(0, callees.length() - 1) : ""));
      first = false;
    }
    js.append("\n]\n");
    Files.writeString(out.resolve("graph.json"), js.toString());
    println("closure: " + seen.size() + " functions");
  }
}
