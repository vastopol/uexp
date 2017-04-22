//
// CSE141L Lab 3: Materialize your processor
// University of California, San Diego
// 
// SuperGarbageSim version 0.1
// Written by Donghwan Jeon, 5/21/2007
// 

import java.io.*;
import java.util.StringTokenizer;


public class SuperGarbageSim {	 
	BufferedReader binInput;
	BufferedReader chInput3;
	BufferedReader chInput2;
	
	long mem[];
	int textStart;
	private static BufferedReader stdin = new BufferedReader(new InputStreamReader( System.in ));	
	int pc;
	int count;
	
	SuperGarbageSim(int size) {
		mem = new long[size];		
	}
	
	void cmdLoad(String filename) {
		String chFileName = null;
		String cntFileName = null;
		try {			
			FileReader fr = new FileReader(filename);			
			binInput = new BufferedReader(fr);
			chFileName = filename.replaceFirst("bin", "in");			
			chInput3 = new BufferedReader(new FileReader(chFileName));
			cntFileName = filename.replaceFirst("bin", "cnt");
			chInput2 = new BufferedReader(new FileReader(cntFileName));
			
			do {
				long addr, data;
				addr = Long.parseLong(binInput.readLine(), 16);				
				data = Long.parseLong(binInput.readLine(), 16);
				if ((data & 0x200000000L) != 0) {
					data |= 0xFFFFFFFC00000000L;
				}
				
				if (addr == 0x3FFFFFFFFL) {				
					textStart = (int)data;
					pc = textStart;
					break;
					
				} else {
					mem[(int)addr] = data;
				}
			} while(true);
			count = 0;		
			System.out.println("SuperGarbage Program " + filename + " loaded. Current PC is 0x" + Integer.toHexString(pc));
			
		} catch(Exception e) {
			System.out.println("Cannot find one of the required files: " + filename + " , " + chFileName + " , " + cntFileName);									
		}		
	}
	
	void run() {		
		int op, srcA, srcB, dest;
		
		do {
			op = (int)mem[pc];
			srcA = (int)mem[pc+1];
			srcB = (int)mem[pc+2];
			dest = (int)mem[pc+3];
			
			int nextPC = exec(op, srcA, srcB, dest, pc);
			
			if (nextPC == -1)
				break;
			else 
				pc = nextPC;
		} while(true);	
		
	}
	
	long getInput(long ch) {
		long res;
		BufferedReader in;
		if (ch < 0 || ch >= 16)
			return -1;
		
		if (ch == 2)
			in = chInput2;
		else if (ch == 3)
			in = chInput3;
		else
			return -1;
			
		
		try {
			while(true) {				
				String input = in.readLine().trim();
				res = Long.parseLong(input, 16);
				System.out.println("[in : " + ch + " ]: 0x" + Long.toHexString(res));
								
				if (res != -1)
					break;					
			} 
			return res;
		} catch(Exception e) {
			e.printStackTrace();
			return -1;
		}
	}
	
	void putOutput(long ch, long val) {
		System.out.println("[out : " + ch + "]: 0x" + Long.toHexString(val & 0x3FFFFFFFFL));
	}
	
	int exec(int op, int srcA, int srcB, int dest, int pc) 	{	  
		int nextPC = pc + 4;
	  
		switch(op) {
			case 0: mem[dest] = mem[srcA] - mem[srcB]; break;
			case 1: mem[dest] = mem[srcA] >> 1; break;
			case 2: mem[dest] = ~(mem[srcA] | mem[srcB]); break;
			case 3: 
				if (mem[srcA] > 4095) {
					System.err.println("Address error " + mem[srcA]);
				}
				long temp = mem[srcB]; mem[dest] = mem[(int)mem[srcA]]; mem[(int)mem[srcA]] = temp; 
				break;
			case 4: 
				long in = getInput(mem[srcA]); 
				if (in == -1) 
					break; 
				else 
					mem[dest] = in; 
				break;//in  mem[dest], mem[srcA]; break; // in  mem, channel #				
			case 5: putOutput(mem[srcB], mem[srcA]); break;//out mem[srcA], mem[srcB]; break; // out data, channel#
			case 6:
				//System.err.println("condition = " + Long.toHexString(mem[srcA]));
				//System.err.println("target = " + Long.toHexString(mem[srcB]));
				mem[dest] = nextPC;
				if (mem[srcA] < 0) {
					nextPC = (int)mem[srcB];
				}
				break;
			case 7: 
				nextPC = -1;
				System.out.println("=== Program Ends ====");
				break;
			default: System.err.println("Invalid instruction at 0x" + Integer.toHexString(pc));
		}		  
		return nextPC;	  
	}
	
	void cmdGo(int num) {
		int op, srcA, srcB, dest;
		if (num == 0)
			num = Integer.MAX_VALUE;
		
		for (int i=0; i<num; i++){			
			if (pc == -1)
				return;
			cmdDisasm(pc, 1);
			op = (int)mem[pc];
			srcA = (int)mem[pc+1];
			srcB = (int)mem[pc+2];
			dest = (int)mem[pc+3];
			
			int nextPC = exec(op, srcA, srcB, dest, pc);
			count++;
			pc = nextPC;
		}
		System.out.println("Current PC is 0x" + Integer.toHexString(pc));
	}

	void cmdDisasm(int start, int num) {
		int op, srcA, srcB, dest;
		
		if (num == 0) {
			start = pc;
			num = 10;			
		}
		
		for (int i=0; i<num; i++) {
			int base = start + 4*i;
			op = (int)mem[base];
			srcA = (int)mem[base + 1];
			srcB = (int)mem[base + 2];
			dest = (int)mem[base + 3]; 
			if (op <0 || op > Instruction.WORD) {
				System.err.println("Invalid Instruction " + op + " at addr 0x" + base);
			}
			
			System.out.println("0x" + Integer.toHexString(base) + "\t : \t" + 
					new Instruction(op, srcA, srcB, dest) + 
					"\t\t  0x" + Long.toString(mem[srcA], 16) + 
					"\t 0x" + Long.toString(mem[srcB], 16) + 
					"\t 0x" + Long.toString(mem[dest], 16));			
		}
	}
	

	String getFormattedString(long src) {
		String base = "000000000";
		String temp = base.concat(Long.toHexString(src & 0x3FFFFFFFFL));
		return temp.substring(temp.length() - 9);
	}
 
	void cmdGetmem(int start, int num) {
		int addr;
		addr = start - start%8;
		
		
		for (int i=0; i<=num/8; i++) {
			System.out.print("\n0x" + getFormattedString(addr + 8*i) + ":\t");
			for (int j=0; j<8; j++)				
				System.out.print(" " + getFormattedString(mem[addr+ 8*i + j]) + " ");
		}
		
	}
	
	void cmdSetmem(int addr, long value) {
		mem[addr] = value;
		cmdGetmem(addr, 16);
	}
	
	void cmdSetPC(int addr) {
		pc = addr;
		System.out.println("Current PC = 0x" + Integer.toHexString(pc));
	}
	
	void cmdGetPC() {
		System.out.println("Current PC = 0x" + Integer.toHexString(pc));		
	}
	
	void cmdGetCount() {
		System.out.println("Dynamic Instruction Count = " + count); 
	}
	
	static void cmdHelp() {
		String desc[] = {
				"=== SuperGarbage Simulator Version 0.1 ===",
				"Written for CSE141L, by Donghwan Jeon",
				"",
				"load\t [$filename] \t\t- load a SuperGarbage Application",
				"go\t [$number] \t\t- execute $number instructions from the PC",
				"step\t [$number] \t\t- execute one instructions at PC",
				"disasm\t [$start_addr] [$number]- disassemble $number instructions from $start_addr",
				"getmem\t [$start_addr] [$number]- show data for $number locations in the memory from $start_addr",
				"setmem\t [$addr] [$data] \t- set $addr to $data",
				"setpc\t [$addr] \t\t- set PC to $addr",
				"getpc\t \t\t\t- print the PC addr",
				"count\t \t\t\t- print the executed instruction count",
				"exit\t\t\t\t- exit SuperGarbageSim",
				"help"				
		};
		for (int i=0; i<desc.length; i++) {
			System.out.println(desc[i]);
		}
	}
	
	int getNum(String str) {
		try {
			if (str.startsWith("0x") || str.startsWith("0X")) 
				return Integer.parseInt(str.substring(2), 16);
			else
				return Integer.parseInt(str);
		} catch(Exception e) {
			System.out.println("Wrong number format");
			return -1;
		}
	}	
	
	
	int cmdIndex(String cmd) {
		String cmds[] = {"GO", "STEP", "DISASM", "GETMEM", "SETMEM", "SETPC", "GETPC", "COUNT", "LOAD", "EXIT", "HELP"};
		for (int i=0; i<cmds.length; i++) {				
			if (cmd.toUpperCase().compareTo(cmds[i]) == 0) {				
				return i;				
			}
		}
		return -1;
	}
	
	void shell() throws IOException {
		String cmd;
		int i;
		int arg0, arg1;
		int cmdIndex;
		String input;
		String arg0_str;
		StringTokenizer st;		
				
		while (true) {
			cmd = null;
			arg0_str = null;
			arg0 = 0;
			arg1 = 0;		
			
			System.out.print("\n>> ");
			input = stdin.readLine().trim();			
			st = new StringTokenizer(input);
			
			if (st.hasMoreTokens())
				cmd = st.nextToken();
			
			if (cmd == null)
				continue;
			
			cmdIndex = cmdIndex(cmd);
			if (cmdIndex == -1) {
				cmdHelp();
			}
			
			if (st.hasMoreTokens()) { 
				arg0_str = st.nextToken().trim();
				if (cmdIndex != cmdIndex("load"))
					arg0 = getNum(arg0_str);
			}
			
			if (st.hasMoreTokens()) { 
				arg1 = getNum(st.nextToken().trim());				
			}
						
			if (arg0 == -1 || arg1 == -1) {				
				continue;
			}					 
			
			switch(cmdIndex) {					     	
				case 0: cmdGo(arg0); break;
				case 1: cmdGo(1); break;
				case 2: cmdDisasm(arg0, arg1); break;
				case 3: cmdGetmem(arg0, arg1); break;
				case 4: cmdSetmem(arg0, arg1); break;
				case 5: cmdSetPC(arg0); break;
				case 6: cmdGetPC(); break;
				case 7: cmdGetCount(); break;
				case 8: cmdLoad(arg0_str); break;
				case 9: return;
				default: cmdHelp(); break;					
			}				
		}		
	}


	
	public static void main(String[] args) {				
		int size = 16 * 1024;
		SuperGarbageSim sim = new SuperGarbageSim(size);	
		cmdHelp();
		try {
			sim.shell();
		} catch(Exception e) {
			e.printStackTrace();		
		}
		System.out.println("Bye!");		
	}

}
