// DumpImage.java - write the initialized memory from 0x82000000 to a flat file.
// args: <output file>
import ghidra.app.script.GhidraScript;
import ghidra.program.model.mem.*;
import java.io.*;

public class DumpImage extends GhidraScript {
    public void run() throws Exception {
        long base = 0x82000000L;
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
