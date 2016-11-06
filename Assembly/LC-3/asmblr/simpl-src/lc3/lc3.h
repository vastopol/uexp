/*
Copyright (c) 2005, Garrett Franklin Boyer
 All rights reserved.
 
 
 Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
Neither the name of the Georgia Institute of Technology nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
/**
 * @file lc.h
 *
 * CS2110 LC-3 Simulator Declarations
 */

#ifndef LC_H_INCLUDED
#define LC_H_INCLUDED

#include "common.h"
#include <stdio.h>
#include <sys/types.h>

#include "model/symbol.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lc_parse;
struct lc_state;

#if SIZEOF_LONG==2
#define LC__T long
#elif SIZEOF_INT==2
#define LC__T int
#elif SIZEOF_SHORT==2
#define LC__T short
#else
"Could not find a suitable 2-byte type.";
#endif

/**
 * An unsigned LC-3 word type.
 */
typedef unsigned LC__T lcword_u;
/**
 * A signed LC-3 word type.
 */
typedef unsigned LC__T lcword_s;

/** Number of registers on an LC-3. */
#define LC_REGS 8
/** Length of LC-3 words, in bits */
#define LC_LEN 16
/** Number of words in memory (2^LC_LEN) */
#define LC_MEM 65536
/** Number of bytes alloted for console output */
#define LC_CONSOLE_SIZE 511
/** Number of bytes for the LC's input */
#define LC_INPUT_SIZE 127

/** Count the number of each instruction executed */
#define LC_IPROFILE

/** Keyboard status register mm-io address.
 * Bit 15 set means input is ready.
 */
#define LC_KBSR 0xFE00
/** Keyboard data register mm-io address.
 * Bits [7:0] contain the character read.
 */
#define LC_KBDR 0xFE02
/** Display status register mm-io address.
 * Bit [15] is set if the display is ready.
 */
#define LC_DSR  0xFE04
/** Display data register mm-io address.
 * Write to it and bits [7:0] get placed to output.
 */
#define LC_DDR  0xFE06
/** Machine control register.
 * If the high bit is set, the machine is running.
 */
#define LC_MCR 0xFFFE

/**
 * Test whether a state is in privileged mode.
 *
 * We define privileged mode as when program counter is < 0x3000.
 */
#define LC_PRIV(lc_state) ((lc_state)->pc < 0x3000)

/**
 * Function type used to allow reading of different types of LC files.
 *
 * The LC-3 supports files in three different formats:
 *
 * - .bin - ASCII zeros and ones that may have spaces and semicolon-delimited
 *       comments
 *
 * - .hex - ASCII hexadecimal code, that may have spaces and ; comments
 *
 * - .obj - binary format files
 *
 * Returns the word on success, and EOF on end-of-file or unrecoverable error.
 */
typedef int (*lc_readword_t)(struct lc_parse *parser);

/**
 * Function type for reading in characters.
 *
 * When implementing this type, instead of emulating the true "polling"
 * behavior I would suggest just waiting.
 *
 * Returns:
 *  Error => EOF
 *  No Character Ready => 0
 *  Character ready => character value 1..255
 */
typedef int (*lc_read_t)(void *obj);

/**
 * Function type for writing out an individual character.
 *
 * Returns:
 *  Error => EOF
 *  Data successfully written => anything but EOF
 */
typedef int (*lc_write_t)(void *obj, int ch);

/**
 * Error message display function.
 */
typedef void (*lc_printerr_t)(void *obj, const char *msg);

#ifdef __cplusplus
#define CAST(t, v) t(v)
#else
#define CAST(t, v) (t)(v)
#endif

#define LC__MASK(i) ((1<<(i))-1)
/**
 * Get a "bits" bits, starting at bit number "shift", from "val"
 */
#define LC_GET_U(val, shift, bits) \
	CAST(lcword_u, (((val))>>(shift)) & LC__MASK(bits))

/**
 * Do the same thing as GET_U, but sign extend for a 16-bit word.
 */
/*#define LC_GET_S(val, shift, bits) \
	CAST(lcword_u, \
	     LC_GET_U(val, shift, bits) | \
	     (((val)&(1<<(shift+bits-1))) ? \
	     (LC__MASK(16-bits)<<bits) : 0))
*/
#define LC_GET_S(val, shift, bits) \
	CAST(lcword_u, \
             (((val)>>shift) & LC__MASK(bits)) \
             | ((~(((val>>((shift) + (bits) - 1)) & 1) - 1)) << (bits)))

/**
 * Turn lcword_u to signed integer.
 */
#define LC_TO_SIGNED(val) ((val)>32767?CAST(int, val)-65536:CAST(int, val))

/**
 * This reads one word from a .bin file f and stores it in valp.
 *
 * It is very loose about formatting, accepting spaces in between the
 * numbers and fully supporting comments.
 *
 * @param parse - parser struct
 * @return negative on error
 */
int lc_readword_bin(struct lc_parse *parse);
/**
 * This reads one word from a .hex file f and stores it in valp.
 *
 * It is very loose about formatting, accepting spaces in between the
 * numbers and fully supporting comments.
 *
 * @param parse - parser struct
 * @return negative on error
 */
int lc_readword_hex(struct lc_parse *parse);
/**
 * This reads one word from a .obj file f and stores it in valp.
 *
 * An object file is just a MSB-first dump of the machine code, with the
 * first word being the origin, just like a .bin or .hex file.
 *
 * @param parse - parser struct
 * @return negative on error
 */
int lc_readword_obj(struct lc_parse *parse);

/**
 * LC3 register number.
 */
typedef enum {
    LC_REG_R0,
    LC_REG_R1,
    LC_REG_R2,
    LC_REG_R3,
    LC_REG_R4,
    LC_REG_R5,
    LC_REG_R6,
    LC_REG_R7,
    LC_REG_PC,
    LC_REG_PSR
} lc_regnum_t;

/**
 * Strcture to use to help with parsing.
 */
struct lc_parse
{
    /** Read function for this */
    lc_readword_t readword;
    /** File to read from */
    FILE *f;
    /** Current line number for parsing */
    int line;
    /** Last line where there was an error, or 0 if no errors */
    int error;
    /** Object used to print errors */
    void *printerr_obj;
    /** Function used to print errors */
    lc_printerr_t printerr_func;
};

/**
 * Send out a parse error.
 * @param parser - the parser object
 * @param format - the format string printf-style by format args
 */
void lc_parse_err(struct lc_parse *parser, const char *format, ...);

/**
 * This structure represents the internal state of the LC-3 machine.
 * 
 * You can declare many of these and run multiple LC-3 machines if you like.
 *
 * @see lc_init
 * @see lc_destroy 
 * @see lc_read
 * @see lc_run
 * @see lc_run_one
 */
struct lc_state {
    /** The LC3's memory - 65536 words allocated by lc_init */
    lcword_u *mem;
    
    /** The registers in the LC machine, of which there are eight. */
    lcword_u reg[LC_REGS];
    /** The program counter. (Must come directly after reg!) */
    lcword_u pc;
    /** The flags - these are set by the previous instruction.
     * See LC_FL_P, LC_FL_Z, LC_FL_N.
     * Must come directly after pc!
     *
     * In the strict LC-3 definition, the three lowest bits of the PSR are
     * to signify negative, zero, and positive, respectively.  However, Dr. 
     * Lumetta's distribution of the tools use the PSR as the three
     * respective bits when encoded into a BR instruction, such that
     * whether_to_branch = (PSR & IR) != 0.
     *
     * For this reason I do not strictly call it the PSR.
     */
    lcword_u flag;
    
    #ifdef LC_IPROFILE
    /** Instruction counts for each instruction */
    int icounts[16];
    /** Total number of branches taken */
    int branch;
    /** Last register written to (or -1 if no register written to) */
    int destreg;
    #endif
    
    /** If this is set, trap will be equivalent to jsr of the vector
     * number, rather than being manually intercepted by the code.
     */
    int real_traps;
    
    /** Symbol table; may be NULL */
    const struct symt *symt;
    
    /** Boolean if the machine is halted */
    int halted;
    
    /** The number of warnings encountered */
    int warning;
    
    /** Input stream object */
    void *read_obj;
    /** Input stream function */
    lc_read_t read_func;
    
    /** Output stream object */
    void *write_obj;
    /** Output stream function */
    lc_write_t write_func;
    
    /* May later abstract these into another struct */
    
    /** Output position in the buffer, starting at 0. */
    int outpos;
    /** The input buffer position */
    int inpos;
    /** The output buffer for I/O */
    char output[LC_CONSOLE_SIZE+1];
    /** The input data */
    char input[LC_INPUT_SIZE+1];
    
    /** File or object to send errors/warnings to */
    void *printerr_obj;
    /** Print the error */
    lc_printerr_t printerr_func;
    
    /** Errors that already occured for output pruning */
    int errset;
};

/**
 * Initializes an LC state.
 * This allocates the proper amount of memory and fills everything to zero.
 * This will also set the program counter to 0x3000, and set the flag to
 * "zero", ie LC_FL_Z.
 * Input and output are set to the internal buffer.
 * @param st the state to initialize
 * @return negative on error
 */
int lc_init(struct lc_state *st);
/**
 * Destroy an LC state.
 * This basically frees st->mem (but not st itself) and does any other
 * cleaning up that may be necessary.
 * @param st the state to clean
 */
void lc_destroy(struct lc_state *st);
/**
 * Copy the contents of the memory and registers into a new uninitialized LC
 * state.
 *
 * The symbol table is only copied by pointer so any changes in one will be
 * reflected in the other.
 *
 * @param dest - an uninitialized destination
 * @param src - source state
 *
 * @return NULL on error, dest on success.
 */
struct lc_state *lc_copy(struct lc_state *dest, const struct lc_state *src);
/**	
 * Set input to file.
 *
 * @param st - the lc3 machine
 * @param f - the file to read input from
 */
void lc_set_input(struct lc_state *st, FILE *f);

/**
 * Set output to file.
 *
 * @param st - the lc3 machine
 * @param f - the file to send output to
 */
void lc_set_output(struct lc_state *st, FILE *f);
/**
 * Run the code in the LC state.  Note that the program counter must be
 * initialized prior to running this; lc_init will initialize it to 0x3000.
 *
 * These operations are not supported:
 * - Return from interrupt
 * - Some traps.  Currently, only halt, putc, puts, putsc are implemented.
 *
 * If an instruction is unsupported or illegal the st->warning will be
 * incremented for each such occurence.  It will also be incremented if the
 * execution runs out of time.
 *
 * All output is directed to the output buffer.  Excess characters are
 * ignored.
 *
 * This will automatically un-halt the machine upon entry.
 *
 * @param st - the state to run
 * @param max_instructions - the maximum number of instructions to run
 * @return number of instructions executed not including halt, or -1 if
 *         max_instructions was reached before a halt
 */
int lc_run(struct lc_state *st, int max_instructions);
/**
 * Run one instruction in the LC simulator.
 *
 * If the machine is halted this will still execute the instruction.
 *
 * @param st - the state of the LC machine
 * @return 0 if the machine has halted, 1 in any other case
 */
int lc_run_one(struct lc_state *st);
/**
 * Read a file that is either a .bin, .hex, or .obj file and place it
 * into memory.  This will set the program counter at the origin specified.
 *
 * @param st - the state to read the code into
 * @param f - the file to read the code from
 * @param readword - the function used to grab a single word from a file;
 *        example for .bin files use lc_getword_bin.
 * @return negative on error; otherwise, the number of instructions read
 */
int lc_read(struct lc_state *st, FILE *f, lc_readword_t readword);
/**
 * Print a more readable version of the instruction; it is still primarily
 * ones and zeros, but it is much more readable.
 *
 * Example output: ADD 001 001 1 11111
 *
 * @param buf - character buffer (32 no color, 64 with color)
 * @param ist - the instruction
 * @param use_color - whether to use color
 */
void lc_printbin_str(char *buf, lcword_u ist, int use_color);
/**
 * Print a more readable version of the instruction; it is still primarily
 * ones and zeros, but it is much more readable.
 *
 * Example output: ADD 001 001 1 11111
 *
 * @param f - the file to print to
 * @param ist - the instruction
 * @param use_color - whether to use color
 */
void lc_printbin(FILE *f, lcword_u ist, int use_color);
/**
 * Give a human-readable version of the instruction, that tries a little bit
 * to guess what the instruction means.  For examples see documentation on
 * lc_trans - this function prints the part after the semicolon.
 *
 * @param st - current machine state
 * @param f - the file to print to
 * @param addr - the address of the instruction to print
 */
void lc_symbolic(const struct lc_state *st, FILE *f, lcword_u addr);
/**
 * Symbolic output to string.
 * Maximum size: 64 characters
 *
 * @param st - current machine line
 * @param s - output string to write to (64 charcters in size)
 * @param addr - address to disassemble
 */
void lc_symbolic_str(const struct lc_state *st, char *s, lcword_u addr);
/**
 * Disassemble a line.
 *
 * @param st - current machine line
 * @param f - output file
 * @param addr - the address of the line to disassemble
 */
void lc_disassemble(const struct lc_state *st, FILE *f, lcword_u addr);
/**
 * Disassemble to string.
 * Maximum size: 64 characters
 *
 * @param st - current machine line
 * @param s - output string to write to (64 charcters in size)
 * @param addr - address to disassemble
 */
void lc_disassemble_str(const struct lc_state *st, char *s, lcword_u addr);
/**
 * This will read a bin/hex/obj file, and print out its contents in the
 * following fashion, putting prefix at the beginning of each line.
 *
 * @param f - the file to read from
 * @param readword - the way to read words from f
 * @param out - the output file to print to
 * @param prefix - the prefix to print every line
 */
int lc_trans(FILE *f, lc_readword_t readword, FILE *out, const char *prefix);
/**
 * This will read the contents of an LC-state and disassemble it nicely.
 *
 * @param st - the state
 * @param out - the output file to print to
 * @param prefix - the prefix to print every line - in the example it was
 *        " | "
 * @param start - the memory address to start reading from
 * @param icount - number of lines to disassemble
 */
void lc_trans_state(struct lc_state *st, FILE *out, const char *prefix,
    lcword_u start, int icount);

/** Format a printout using color */
#define LC_PA_COLOR 0x01
/** Format a printout using symbolic notation rather than disassembly */
#define LC_PA_SYMB 0x02
/** Print an address assuming it is executable (rather than data) */
#define LC_PA_EXEC 0x04

/**
 * Print the current state of the LC-3 machine.
 *
 * Uses colors when available.
 *
 * @param st - the state
 * @param s - string to write to
 * @param flags - display flags (bitwise OR of LC_PA_COLOR and LC_PA_SYMB)
 * @param old_regs - the old contents of each of the registers
 * @return the end of the string
 */
char *lc_print_state_str(const struct lc_state *st, char *s, int flags,
        const lcword_u *old_regs);

/**
 * Print a detailed listing of a particular address.
 * @param st - the machine state
 * @param f - file to print address to
 * @param addr - address to print
 * @param flags - bitwise or of LC_PA_COLOR | LC_PA_SYMB | LC_PA_EXEC
 */
void lc_print_addr(const struct lc_state *st, FILE *f, int addr, int flags);

/**
 * Print a detailed listing of a particular address.
 * @param st - the machine state
 * @param buf - file to print address to (needs to be really big for colors)
 * @param addr - address to print
 * @param flags - bitwise or of LC_PA_COLOR | LC_PA_SYMB | LC_PA_EXEC
 */
void lc_print_addr_str(const struct lc_state *st, char *buf, int addr, int flags);

/** Ala fgetc. */
int lc_file_read(void *obj);
/** Ala fputc. */
int lc_file_write(void *obj, int c);
/**
 * Output one character to the state's output buffer.
 *
 * @param obj - the object to write to (an lc_state)
 * @param c - the character
 * @return negative on error, zero on success
 */
int lc_buffer_write(void *obj, int c);
/**
 * Read in one character from the input buffer.
 *
 * @param obj - object to read from (an lc_state)
 * @return the inputted character, orEOF  if end of input
 */
int lc_buffer_read(void *obj);
/**
 * Standard error-printing mechanism to a FILE * output stream.
 * 
 * @param vfile - FILE * pointer to print error to
 * @param msg - the message to print
 */
void lc_printerr(void *vfile, const char *msg);

/**
 * Load the operating system into the LC-3 state.
 * @param st - the system in question
 */
void lc_load_os(struct lc_state *st);


/** ADD instruction number */
#define IST_ADD 1
/** AND instruction number */
#define IST_AND 5
/** BR instruction number */
#define IST_BR 0
/** JMP instruction number */
#define IST_JMP 12
/** JSR instruction number */
#define IST_JSR 4
/** LD instruction number */
#define IST_LD 2
/** LDI instruction number */
#define IST_LDI 10
/** LDR instruction number */
#define IST_LDR 6
/** LEA instruction number */
#define IST_LEA 14
/** NOT instruction number */
#define IST_NOT 9
/** RTI instruction number */
#define IST_RTI 8
/** ST instruction number */
#define IST_ST 3
/** STI instruction number */
#define IST_STI 11
/** STR instruction number */
#define IST_STR 7
/** TRAP instruction number */
#define IST_TRAP 15

/** Code for an entire return instruction */
#define LC_RET 0xC1C0

/** Positive flag (for st->flag) */
#define LC_FL_P (1<<9)
/** Zero flag (for st->flag) */
#define LC_FL_Z (1<<10)
/** Negative flag (for st->flag) */
#define LC_FL_N (1<<11)

#ifdef __cplusplus
} /* extern "C" */

/**
 * Light wrapper around LC-3 machine in order to accomplish the following:
 *
 *  - Automatic construction and destruction
 *  - Replacement of error codes with exceptions
 *  - Easier function calling (duh!)
 */
class LC3Machine : public lc_state
{
 public:
    /** A name for lcword_u.  I'm thinking of removing this; it doesn't really
     * serve a purpose.
     */
    typedef lcword_u Word;
    
 public:
    /** Construct the LC-3 machine using lc_init */
    LC3Machine() { Exception::when(lc_init(this) < 0); }
    /** Copy another LC-3 machine using lc_copy */
    LC3Machine(const LC3Machine &other)
     { Exception::when(!lc_copy(this, &other)); }
    /** Copy from a pure-C lc_state */
    LC3Machine(const lc_state *st)
     { Exception::when(!lc_copy(this, st)); }
    /** Free all resources (lc_destroy) */
    ~LC3Machine() { lc_destroy(this); }
    /**
     * Assign operator (basically lc_destroy and lc_copy)
     * @param other - machine state to duplicate
     */
    LC3Machine & operator = (const LC3Machine &other)
     { if (this != &other)
        { lc_destroy(this); Exception::when(!lc_copy(this, &other)); }
       return *this; }
    
    /** Set the input stream to file */
    void setInput(FILE *f) { lc_set_input(this, f); }
    /** Set the output stream to file */
    void setOutput(FILE *f) { lc_set_output(this, f); }
    /** Run until halt, or max_instructions exceeded */
    int run(int max_instructions = 999999999)
     { return lc_run(this, max_instructions); }
    /** Run a single instruction, and return true if machine is not halted. */
    bool runOne() { return lc_run_one(this) != 0; }
    
    /**
     * Read the file into the machine and set PC to the program's origin.
     * @param f - file to read
     * @param lc_readword_t - a method of extracting words from the input file
     * @return negative on error, positive on success
     */
    int read(FILE *f, lc_readword_t readword = lc_readword_obj)
     { return lc_read(this, f, readword); }
    
    /**
     * Print the particular address symbolically.
     * @param f - stream to write to
     * @param addr - the address to symbolically print
     */
    void printSymbolic(FILE *f, lcword_u addr) const
     { lc_symbolic(this, f, addr); }
    
    /**
     * Write out symbolically the address.
     * @param buf - buffer to write to
     * @param addr - address to write to
     */
    void strSymbolic(char *buf, lcword_u addr) const
     { lc_symbolic_str(this, buf, addr); }
    /**
     * Print the assembly contents of a line to file.
     * @param f - file to print to
     * @param addr - the address to disassemble
     */
    void printAssembly(FILE *f, lcword_u addr) const
     { lc_disassemble(this, f, addr); }
    /**
     * Print the assembly contents of a line to file.
     * @param buf - buffer to write to
     * @param addr - the address to disassemble
     */
    void strAssembly(char *buf, lcword_u addr) const
     { lc_disassemble_str(this, buf, addr); }
    /**
     * Print out the 1's and 0's.
     * @param f - file to print to
     * @param addr - address
     */
    void printBits(FILE *f, lcword_u addr, int flags=0) const
     { lc_printbin(f, mem[addr], flags); }
    /**
     * Print out the 1's and 0's.
     * @param buf - bufer to write to
     * @param addr - address
     */
    void strBits(char *buf, lcword_u addr, int flags=0) const
     { lc_printbin_str(buf, mem[addr], flags); }
    /**
     * Print a particular memory address.
     * @param f - file to print to
     * @param addr - address to print
     * @param flags - bitwise OR of LC_PA_COLOR LC_PA_SYMB LC_PA_EXEC
     */
    void printAddr(FILE *f, int addr, int flags = 0) const
     { lc_print_addr(this, f, addr, flags); }
    /**
     * Print a particular memory address.
     * @param buf - buffer to write to
     * @param addr - address to print
     * @param flags - bitwise OR of LC_PA_COLOR LC_PA_SYMB LC_PA_EXEC
     */
    void strAddr(char *buf, int addr, int flags = 0) const
     { lc_print_addr_str(this, buf, addr, flags); }
    
    /**
     * Change whether we are using raw interrupts.
     * @param raw - use true if interrupts should be raw, false otherwise
     */
    void setRaw(bool raw) { real_traps = raw; }
    /**
     * Are we using raw interrupts?
     * @return true if so
     */
    bool isRaw() const { return real_traps != 0; }
};

#endif

#endif
