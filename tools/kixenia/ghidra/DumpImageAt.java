// DumpImageAt.java - write the initialized memory from a base address to a flat file.
// args: <output file> <base address>
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.*;
import java.io.*;

public class DumpImageAt extends GhidraScript {
    public void run() throws Exception {
        long base = Long.decode(getScriptArgs()[1]);
        long end = base;
        for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
            long e = b.getEnd().getOffset() + 1;
            if (b.getStart().getOffset() >= base && e > end) end = e;
        }
        byte[] out = new byte[(int) (end - base)];
        for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
            if (!b.isInitialized() || b.getStart().getOffset() < base) continue;
            byte[] data = new byte[(int) b.getSize()];
            b.getBytes(b.getStart(), data);
            System.arraycopy(data, 0, out, (int) (b.getStart().getOffset() - base), data.length);
        }
        try (FileOutputStream f = new FileOutputStream(getScriptArgs()[0])) { f.write(out); }
        println("dumped " + out.length + " bytes");
    }
}
