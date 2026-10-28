// Holes.java - addresses inside a function's .pdata range that hold no instruction, with the
// error bookmarks there.
// args: <start> <size in bytes>...
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;

public class Holes extends GhidraScript {
  public void run() throws Exception {
    String[] a = getScriptArgs();
    for (int k = 0; k + 1 < a.length; k += 2) {
      Address s = toAddr(Long.decode(a[k]));
      long n = Long.decode(a[k + 1]);
      StringBuilder out = new StringBuilder();
      for (long o = 0; o < n; o += 4) {
        Address x = s.add(o);
        if (getInstructionAt(x) == null) {
          out.append(x).append(" ");
          for (Bookmark b : currentProgram.getBookmarkManager().getBookmarks(x))
            out.append("[").append(b.getCategory()).append(": ").append(b.getComment()).append("] ");
          Data d = getDataAt(x);
          if (d != null) out.append("(dado ").append(d.getDataType().getName()).append(") ");
        }
      }
      println("HOLES " + a[k] + " " + out);
    }
  }
}
