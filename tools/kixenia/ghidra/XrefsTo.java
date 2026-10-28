import ghidra.app.script.GhidraScript;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.listing.Function;
public class XrefsTo extends GhidraScript {
  public void run() throws Exception {
    for (String a : getScriptArgs()) {
      for (Reference r : getReferencesTo(toAddr(a))) {
        Function f = getFunctionContaining(r.getFromAddress());
        println("XR " + a + " <- " + r.getFromAddress() + " " + r.getReferenceType() + " in " + (f == null ? "?" : f.getName() + "@" + f.getEntryPoint()));
      }
    }
  }
}
