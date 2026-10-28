import ghidra.app.script.GhidraScript;
public class Floats extends GhidraScript {
  public void run() throws Exception {
    String[] a = getScriptArgs(); var ad = toAddr(a[0]); int n = Integer.decode(a[1]);
    StringBuilder s = new StringBuilder();
    for (int i = 0; i < n; i++) { int v = getInt(ad.add(i*4)); s.append(String.format("%08X(%g) ", v, Float.intBitsToFloat(v))); if (i%3==2) s.append("| "); }
    println("FL " + a[0] + " " + s);
  }
}
