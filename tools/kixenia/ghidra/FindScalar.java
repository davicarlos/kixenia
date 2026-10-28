import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import java.util.*;
public class FindScalar extends GhidraScript {
  public void run() throws Exception {
    Set<Long> want = new HashSet<>();
    for (String a : getScriptArgs()) want.add(Long.decode(a));
    Map<String,Set<String>> hits = new TreeMap<>();
    InstructionIterator it = currentProgram.getListing().getInstructions(true);
    while (it.hasNext()) {
      Instruction in = it.next();
      for (int i = 0; i < in.getNumOperands(); i++) for (Object o : in.getOpObjects(i)) if (o instanceof Scalar) {
        long v = ((Scalar)o).getUnsignedValue() & 0xffff; long s = ((Scalar)o).getSignedValue();
        for (long w : want) if (v == (w & 0xffff) && (in.getMnemonicString().startsWith("cmp") || in.getMnemonicString().startsWith("ori") || in.getMnemonicString().startsWith("addi") || in.getMnemonicString().startsWith("li"))) {
          Function f = getFunctionContaining(in.getAddress());
          hits.computeIfAbsent(Long.toHexString(w), k -> new TreeSet<>()).add((f==null?"?":f.getEntryPoint().toString()) + ":" + in.getMnemonicString());
        }
      }
    }
    for (var e : hits.entrySet()) println("HIT " + e.getKey() + " " + e.getValue());
  }
}
