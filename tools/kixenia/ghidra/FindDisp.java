import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import java.util.*;
// Lista toda instrução de carga/armazenamento/addi cujo deslocamento de 16 bits
// é um dos argumentos (acha acessos lis/lwz a globais sem xref no Ghidra).
public class FindDisp extends GhidraScript {
  public void run() throws Exception {
    Set<Long> want = new HashSet<>();
    for (String a : getScriptArgs()) want.add(Long.decode(a) & 0xffff);
    InstructionIterator it = currentProgram.getListing().getInstructions(true);
    while (it.hasNext()) {
      Instruction in = it.next();
      String m = in.getMnemonicString();
      if (!(m.startsWith("st") || m.startsWith("l") || m.startsWith("addi"))) continue;
      for (int i = 0; i < in.getNumOperands(); i++) for (Object o : in.getOpObjects(i)) if (o instanceof Scalar) {
        long v = ((Scalar)o).getUnsignedValue() & 0xffff;
        if (want.contains(v)) {
          Function f = getFunctionContaining(in.getAddress());
          println("DISP " + in.getAddress() + " " + in + " in " + (f==null?"?":f.getEntryPoint().toString()));
        }
      }
    }
  }
}
