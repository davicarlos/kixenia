import ghidra.app.script.GhidraScript;
import java.nio.file.*;
public class ReadWords extends GhidraScript {
  public void run() throws Exception {
    StringBuilder sb = new StringBuilder();
    for (String s : Files.readAllLines(Paths.get(getScriptArgs()[0]))) { if (s.isBlank()) continue; sb.append(s).append(" ").append(String.format("%08x", getInt(toAddr(s)))).append("\n"); }
    Files.writeString(Paths.get(getScriptArgs()[1]), sb.toString());
  }
}
