import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;
public class FindWord extends GhidraScript {
  public void run() throws Exception {
    for (String a : getScriptArgs()) {
      long v = Long.decode(a);
      byte[] pat = {(byte)(v>>24),(byte)(v>>16),(byte)(v>>8),(byte)v};
      Address start = currentProgram.getMinAddress();
      int n = 0;
      while (n < 20) {
        Address f = currentProgram.getMemory().findBytes(start, pat, null, true, monitor);
        if (f == null) break;
        MemoryBlock b = currentProgram.getMemory().getBlock(f);
        var fn = getFunctionContaining(f);
        println("WORD " + a + " at " + f + " " + (b==null?"":b.getName()) + (fn==null?"":" in "+fn.getEntryPoint()));
        start = f.add(1); n++;
      }
    }
  }
}
