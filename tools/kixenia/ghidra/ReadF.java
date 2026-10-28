import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
public class ReadF extends GhidraScript {
  public void run() throws Exception {
    for (String a : getScriptArgs()) {
      Address ad = toAddr(a);
      try { int v = getInt(ad); println("VAL " + a + " int=0x" + Integer.toHexString(v) + " float=" + Float.intBitsToFloat(v) + " block=" + currentProgram.getMemory().getBlock(ad).getName()); }
      catch (Exception e) { println("VAL " + a + " ERR " + e); }
    }
  }
}
