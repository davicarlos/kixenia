import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Instruction;
public class Dis2 extends GhidraScript {
  public void run() throws Exception {
    var a = toAddr(getScriptArgs()[0]); int n = Integer.parseInt(getScriptArgs()[1]);
    var f = getFunctionContaining(a); println("DIS func " + (f==null?"none":f.getEntryPoint()+" "+f.getBody().getNumAddresses()));
    for (int i = -4; i < n; i++) { var ad = a.add(i*4); Instruction in = getInstructionAt(ad); println("DIS " + ad + " " + (in==null? String.format("%08x", getInt(ad)) : in.toString())); }
  }
}
