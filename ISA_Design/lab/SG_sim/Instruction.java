//
// CSE141L Lab 3: Materialize your processor
// University of California, San Diego
// 
// SuperGarbage Instruction Class
// Written by Donghwan Jeon, 5/21/2007
// 

public class Instruction {

	static final int SUB = 0;
	static final int RSFT = 1;
	static final int NOR = 2;
	static final int SWAP = 3;
	static final int IN = 4;
	static final int OUT = 5;
	static final int BLZ = 6;
	static final int HALT = 7;
	static final int WORD = 8;
	
	static String opName[] = {"SUB", "RSFT", "NOR", "SWAP", "IN", "OUT", "BLZ", "HALT", "WORD"};
	
	Instruction(int n_opcode, long n_src0, long n_src1, long n_dst) {
		opcode = n_opcode;
		if (opcode == -1) {
			System.err.println(n_opcode + " is invalid opcode");
		}
		src0 = n_src0;
		src1 = n_src1;
		dst = n_dst;
	}
	
	Instruction(String op, long n_src0, long n_src1, long n_dst) {
		opcode = getIndex(op);
		if (opcode == -1) {
			System.err.println(op + " is invalid opcode");
		}
		src0 = n_src0;
		src1 = n_src1;
		dst = n_dst;
	}
	
	int opcode;
	long src0, src1, dst;
	
	public String toString() {
		return opName[opcode] + "  \t" + Long.toString(src0) + "\t" 
		+ Long.toString(src1) + "\t" + Long.toString(dst);
	}
	
	static public int getIndex(String str) {
		for (int i=0; i<opName.length; i++) {
			if (str.toUpperCase().compareTo(opName[i]) == 0) {
				return i;				
			}
		}
		return -1;
	}
	
	public static void main(String args[]) {
		System.out.println(new Instruction(SUB, 1, 2, 3));
		Instruction test = new Instruction(0x1111, 1024, 2048, 3111);
		
	}
}
