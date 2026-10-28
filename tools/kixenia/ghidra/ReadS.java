import ghidra.app.script.GhidraScript;
public class ReadS extends GhidraScript {
  public void run() throws Exception {
    for (String a : getScriptArgs()) {
      byte[] b = getBytes(toAddr(a), 96); StringBuilder s = new StringBuilder();
      for (byte x : b) { if (x == 0) break; s.append((char)(x & 0xff)); }
      println("STR " + a + " \"" + s + "\"");
    }
  }
}
