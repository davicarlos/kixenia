// DecompIn.java - write the C of the functions containing the given addresses.
// args: <output dir> <address>...
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class DecompIn extends GhidraScript {
    public void run() throws Exception {
        String[] a = getScriptArgs();
        File dir = new File(a[0]); dir.mkdirs();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (int i = 1; i < a.length; i++) {
            Function f = getFunctionContaining(toAddr(Long.decode(a[i])));
            if (f == null) { println("no function at " + a[i]); continue; }
            DecompileResults r = di.decompileFunction(f, 120, monitor);
            try (PrintWriter w = new PrintWriter(new File(dir, a[i] + ".c"))) {
                w.println("// " + f.getName() + " at " + f.getEntryPoint() + " (contains " + a[i] + ")");
                w.println(r.getDecompiledFunction() != null ? r.getDecompiledFunction().getC() : r.getErrorMessage());
            }
        }
    }
}
