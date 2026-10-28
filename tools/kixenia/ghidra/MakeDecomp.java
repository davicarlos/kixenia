import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
public class MakeDecomp extends GhidraScript {
  public void run() throws Exception {
    var a = toAddr(getScriptArgs()[0]);
    new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
    new CreateFunctionCmd(a).applyTo(currentProgram, monitor);
    var f = getFunctionAt(a);
    DecompInterface d = new DecompInterface(); d.openProgram(currentProgram);
    var r = d.decompileFunction(f, 120, monitor);
    println("DEC\n" + r.getDecompiledFunction().getC());
  }
}
