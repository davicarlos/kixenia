import ghidra.app.script.GhidraScript;
public class Dump extends GhidraScript {
  public void run() throws Exception {
    String[] a = getScriptArgs(); byte[] b = getBytes(toAddr(a[0]), Integer.decode(a[1]));
    StringBuilder s = new StringBuilder(); for (int i = 0; i < b.length; i++) { s.append(String.format("%02x", b[i]&0xff)); s.append(i%2==1?" ":""); }
    println("DUMP " + s);
  }
}
