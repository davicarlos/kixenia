import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
public class Dis extends GhidraScript {
  public void run() throws Exception {
    for (String a : getScriptArgs()) {
      Instruction in = getInstructionAt(toAddr(a));
      StringBuilder s = new StringBuilder();
      for (int i = 0; i < 4 && in != null; i++, in = in.getNext()) {
        s.append(in.toString()).append(" ; ");
        for (Reference r : in.getReferencesFrom()) { var sym = getSymbolAt(r.getToAddress()); if (sym != null) s.append("[").append(sym.getName()).append("] "); }
      }
      println("DIS " + a + " " + s);
    }
  }
}
