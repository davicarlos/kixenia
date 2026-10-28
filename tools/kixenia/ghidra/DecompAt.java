// DecompAt.java - make functions at exact addresses and write their C.
// args: <output dir> <address>...
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.io.*;

public class DecompAt extends GhidraScript {
    public void run() throws Exception {
        String[] a = getScriptArgs();
        File dir = new File(a[0]); dir.mkdirs();
        FunctionManager fm = currentProgram.getFunctionManager();
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        for (int i = 1; i < a.length; i++) {
            Address addr = toAddr(Long.decode(a[i]));
            if (getInstructionAt(addr) == null) {
                new DisassembleCommand(addr, null, true).applyTo(currentProgram, monitor);
            }
            Function f = fm.getFunctionAt(addr);
            if (f == null) {
                Function outer = fm.getFunctionContaining(addr);
                if (outer != null) {
                    fm.removeFunction(outer.getEntryPoint());
                }
                new CreateFunctionCmd(addr).applyTo(currentProgram, monitor);
                f = fm.getFunctionAt(addr);
            }
            DecompileResults r = f == null ? null : di.decompileFunction(f, 120, monitor);
            try (PrintWriter w = new PrintWriter(new FileWriter(new File(dir, a[i] + ".c")))) {
                w.println(r != null && r.getDecompiledFunction() != null ? r.getDecompiledFunction().getC() : "// failed");
            }
        }
    }
}
