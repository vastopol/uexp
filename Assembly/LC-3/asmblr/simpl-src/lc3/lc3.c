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
 * @file lc.c
 *
 * Definitions for LC simulator functions.
 *
 * Please note that 60%+ is legacy code.  This is maintained in concert
 * with a C library used for autograding assignments.  Although a good bit
 * of code is not used in SimpLC, it is used as part of a separate library.
 */

/*

WARNING!

After the multi-architecture framework was developed, a lot of the
formatting utilities here are not part of the UI, but are used only as part
of legacy grader code.

Make sure when modifying formatting code that you are modifying the right
code.  Sometimes you will want to change whether something is displayed
in hex or not; that would correspond to the assembler definition in
archlc3.cpp.  Other formatting elements will be in format/format.cpp and
and asmformat/asmrule.cpp depending if it is related to disassembly.

 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "lc3.h"
#define LC3OS_INCLUDE_DEFINITION
#include "lc3os.h"

#ifdef LC_IPROFILE
#define PROF_ONLY(e) e
#else
#define PROF_ONLY(e) (void)0
#endif

/* These macros are meant to be used inside a function where the variable
 * ist is the binary representation of an instruction and pc is the
 * incremented program counter.
 */
#define RegisterDest(start) LC_GET_U(ist, start, 3)
#define D_AR RegisterDest(9)
#define D_SR1 RegisterDest(6)
#define D_SR2 RegisterDest(0)
#define D_U(len) (ist&(1<<3))
#define D_IMM(len) (LC_GET_S(ist, 0, len))
#define ADD(a,b) ((a+b)&0xFFFF)
#define D_OFF(len) (ADD(pc,D_IMM(len)))

/*--------------------------------------------------------------------------*/
/* LC-3 File Reading */
/*--------------------------------------------------------------------------*/

/*
 * Write out a parse error.
 */
void lc_parse_err(struct lc_parse *parser, const char *format, ...)
{
    char buf[256];
    char *s = buf;
    va_list vl;
    
    /* GNU provides vasprintf, but unfortunately it's not standard, so I
     * have to rely on "good faith."  The snprintf function is not C89.
     * I am contemplating abandoning C89.
     */
    s += sprintf(s, "line %d: ", parser->line);
    va_start(vl, format);
    s += vsprintf(s, format, vl);
    va_end(vl);
    
    parser->printerr_func(parser->printerr_obj, buf);
    
    parser->error = parser->line;
}

/*
 * This reads one word from a .bin file f and stores it in valp.
 *
 * It is very loose about formatting, accepting spaces in between the
 * numbers and fully supporting comments.
 *
 * @param f - file to read from
 * @param valp - the place to store the read value
 * @return negative on error
 */
int lc_readword_bin(struct lc_parse *parse)
{
    lcword_u val;
    int c;
    int shift;
    
    do {
        val = 0;
        c = 0;
        shift = 16;
        
        while ((c = fgetc(parse->f)) != EOF && c != '\r' && c != '\n')
        {
            if (c == '0' || c == '1')
            {
                shift--;
                if (c == '1') val |= (1 << shift);
            }
            else if (c == ';')
            {
                while (c != EOF && c != '\r' && c != '\n')
                    c = fgetc(parse->f);
                break;
            }
            else if (!isspace(c))
            {
                if (parse->error != parse->line)
                    lc_parse_err(parse, "Illegal character '%c'", c);
            }
        }
        
        parse->line++;
    } while (shift == 16 && c != EOF);
    
    if (shift != 0 && shift != 16)
        lc_parse_err(parse, "Expected 16 bits, got %d", 16 - shift);
    
    if (shift == 16) return EOF;
    
    return val;
}

/*
 * This reads one word from a .hex file f and stores it in valp.
 *
 * It is very loose about formatting, accepting spaces in between the
 * numbers and fully supporting comments.
 *
 * @param f - file to read from
 * @param valp - the place to store the read value
 * @return negative on error
 */
int lc_readword_hex(struct lc_parse *parse)
{
    lcword_u val;
    int c;
    int shift;
    
    do {
        val = 0;
        c = 0;
        shift = 16;
        
        while ((c = fgetc(parse->f)) != EOF && c != '\r' && c != '\n')
        {
            if (isxdigit(c))
            {
                shift -= 4;
                if (c >= 'a')
                    val |= (c - 'a' + 10) << shift;
                else if (c >= 'A')
                    val |= (c - 'A' + 10) << shift;
                else
                    val |= (c - '0') << shift;
            }
            else if (c == ';')
            {
                while (c != EOF && c != '\r' && c != '\n')
                    c = fgetc(parse->f);
                break;
            }
            else if (!isspace(c))
            {
                if (parse->error != parse->line)
                    lc_parse_err(parse, "Illegal character '%c'", c);
            }
        }
        
        parse->line++;
    } while (shift == 16 && c != EOF);
    
    if (shift != 0 && shift != 16)
        lc_parse_err(parse, "Expected 16 bits, got %d", 16 - shift);
    
    if (shift == 16) return EOF;
    
    return val;
}

/*
 * This reads one word from a .obj file f and stores it in valp.
 *
 * An object file is just a MSB-first dump of the machine code, with the
 * first word being the origin, just like a .bin or .hex file.
 *
 * @param f - file to read from
 * @param valp - the place to store the read value
 * @return negative on error
 */
int lc_readword_obj(struct lc_parse *parser)
{
    int c1 = 0;
    int c2 = 0;
    
    if ((c1 = fgetc(parser->f)) == EOF)
        return EOF;
    if ((c2 = fgetc(parser->f)) == EOF)
        return EOF; /* this shouldn't happen */
    
    return (lcword_u)((c1 << 8) | (c2));
}

/*--------------------------------------------------------------------------*/
/* Display/UI Functions */
/*--------------------------------------------------------------------------*/

/* Conditions for branching for the BR instruction, for visual display */
static const char *lc_branch_names[] =
    { "0", ">0", "=0", ">=0", "<0", "!=0", "<=0", "1" };
static const char *lc_branch_nzp[] =
    { "", "p", "z", "zp", "n", "np", "nz", "" };
/*    { NULL, "> 0", "= 0", ">= 0", "< 0", "!= 0", "<= 0", NULL };*/
/*    { NULL, "p", "z", "zp", "n", "np", "nz", NULL };*/

/*
 * Print an address symbolically.
 */
static char *lc_symbolic_addr(const struct lc_state *st, char *buf,
    size_t size, int addr, int relative)
{
    const char *name = NULL;
    
    if (st->symt)
    {
        name = symt_getname(st->symt, addr);
    }
    
    if (name)
    {
        strncpy(buf, name, size);
    }
    else
    {
        sprintf(buf, "x%04X", (addr - relative) & 0xffff);
    }
    
    buf[size-1] = 0;
    
    return buf + size;
}

/*
 * This structure represents how an opcode is split up visually.
 */
struct lc_display_format
{
    /** Mask to AND with */
    int mask;
    /** Associated value */
    int val;
    /** Instruction name, such as "ADD" */
    const char *name;
    /** Fields in a specified format.
     *
     * Lowercase are sign-extended, uppercase are not.
     *
     * - X is hex.
     * - D is decimal.
     * - A is address (with symbolic resolution)
     * - Q is a non-tranlsated field.
     * - R is a register.
     * - C is a condition-code.
     *
     */
    const char *fields;
};

/*
 * Instruction formats for printing.
 */
static struct lc_display_format lc_ist_formats[] =
{
    { 0xfe00, 0x0000, NULL,  "0Q4X4X4X" }, /* branch on no condition */
    { 0xf1ff, 0x0000, NULL,  "0Q4X4X4X" }, /* branch to next memory address */
    { 0xf000, 0x0000, "BR",  "3C0Q0Q9a" },
    { 0xf038, 0x1000, "ADD", "3R3R3Q3R" },
    { 0xf020, 0x1020, "ADD", "3R3R1Q5d" },
    { 0xf000, 0x2000, "LD ", "3R0Q0Q9a" },
    { 0xf000, 0x3000, "ST ", "3R0Q0Q9a" },
    { 0xf800, 0x4800, "JSR", "1Q0Q0QBa" },
    { 0xfe3f, 0x4000, "JSR", "1Q2Q3R6Q" },
    { 0xf038, 0x5000, "AND", "3R3R3Q3R" },
    { 0xf020, 0x5020, "AND", "3R3R1Q5x" },
    { 0xf000, 0x6000, "LDR", "3R3R0Q6d" },
    { 0xf000, 0x7000, "STR", "3R3R0Q6d" },
    { 0xffff, 0x8000, "RTI", "0Q4Q4Q4Q" },
    { 0xf03f, 0x903f, "NOT", "3R3R0Q6Q" },
    /*{ 0xf038, 0x9000, "XOR", "3R3R3Q3R" },
    { 0xf020, 0x9020, "XOR", "3R3R1Q5x" },*/
    { 0xf000, 0xa000, "LDI", "3R0Q0Q9a" },
    { 0xf000, 0xb000, "STI", "3R0Q0Q9a" },
    { 0xfe3f, 0xc000, "JMP", "3Q3R0Q6Q" },
    { 0xf000, 0xe000, "LEA", "3R0Q0Q9a" },
    { 0xff00, 0xf000, "TRP", "4Q0Q0Q8X" },
    { 0,      0,      NULL , "0Q4X4X4X" }
};

/*
 * Print one field of a value.
 *
 * @param val - the value
 * @param f - the output file
 * @param shift - the position of the rightmost bit to print
 * @param bits - the width of the field
 */
static char *lc_printbin_field(lcword_u val, char *s, int shift, int bits)
{
    int i;
    
    for (i = shift + bits; --i >= shift;)
        if (((val >> i) & 0x1))
            *s++ = '1';
        else
            *s++ = '0';
    
    return s;
}

static void lc_printbin_str_format(char *buf, lcword_u ist, int use_color,
    const struct lc_display_format *format)
{
    int i = 12;
    int col = 0;
    int c = 0;
    char *s = buf;
    const char * const colors[2] = { "\033[0;30;46m", "\033[1;37;44m" };
    const char *fields;
    
    
    if (!format->name)
    {
        if (use_color) s += sprintf(s, colors[1]);
        s = lc_printbin_field(ist, s, 12, 4);
    }
    else
        s += sprintf(s, "%-3s ", format->name);
    
    for (fields = format->fields; *fields;)
    {
        int bits = *fields++ - '0';
        char type = *fields++;
        
        if (bits > 9) bits += '0' - 'A' + 10;
        
        if (bits > '9')
        {
            bits += '0' - 'A' + 10;
        }
        
        if (!use_color)
        {
            if (c != 0) *s++ = ' ';
        }
        else if (type == 'Q')
        {
            s += sprintf(s, "\033[0;37;40m");
        }
        else if (bits != 0)
        {
            s += sprintf(s, colors[col]);
            col ^= 1;
        }
        
        i -= bits;
        s = lc_printbin_field(ist, s, i, bits);
        c++;
    }
    
    if (use_color) s += sprintf(s, "\033[0m");
    
    *s = '\0';
}

/*
 * Print to string.
 *
 * TODO: CONFIRM!!!!
 *
 * Maximum length (no color): 32
 * Maximum length (with color): 64
 *
 * @param buf - buffer to write to
 * @param ist - instruction to print
 * @param use_color - whether to use color
 */
void lc_printbin_str(char *buf, lcword_u ist, int use_color)
{
    struct lc_display_format *format;

    for (format = lc_ist_formats; (ist & format->mask) != format->val; format++)
    {
    }
    
    lc_printbin_str_format(buf, ist, use_color, format);
}

/*
 * Print a more readable version of the instruction; it is still primarily
 * ones and zeros, but it is much more readable.
 *
 * Example output: ADD 001 001 1 11111
 *
 * @param f - the file to print to
 * @param ist - the instruction
 */
void lc_printbin(FILE *f, lcword_u ist, int use_color)
{
    char buf[128];
    
    lc_printbin_str(buf, ist, use_color);
    
    fputs(buf, f);
}


/*
 *
 * Disassemble!
 * Maximum size: 64 characters
 *
 * @param st - the machine
 * @param buf - the string to write to
 * @param addr - the address of the instruction to print
 */
void lc_disassemble_str(const struct lc_state *st, char *buf, lcword_u addr)
{
    char *s = buf;
    struct lc_display_format *fmt;
    lcword_u ist = st->mem[addr];
    
    for (fmt = lc_ist_formats; (ist & fmt->mask) != fmt->val; fmt++) { }
    
    if (fmt->name)
    {
        const char *fields;
        int any = 0;
        int shift = 12;

        s += sprintf(s, "%s", fmt->name);
        
        for (fields = fmt->fields; *fields;)
        {
            int bits = *fields++ - '0';
            char type = *fields++;
            
            if (bits > 9) bits += '0' - 'A' + 10;
            
            shift -= bits;
            
            if (type != 'Q')
            {
                int v = (ist >> shift) & ((1 << bits) - 1);
                
                /* TODO: Is there a sign-extension macro? */
                if ((type & 0x20) && (v & (1 << (bits-1))))
                    v |= -1 & ~((1 << bits) - 1);
                
                if (any) *s++ = ',';
                any = 1;
                
                switch(type | 0x20)
                {
                 case 'x':
                    s += sprintf(s, " x%04X", v & 0xffff);
                    break;
                 case 'd':
                    s += sprintf(s, " #%d", v);
                    break;
                 case 'r':
                    s += sprintf(s, " R%d", v);
                    break;
                 case 'a':
                    *s++ = ' ';
                    lc_symbolic_addr(st, s, 30, (v + addr + 1) & 0xffff,
                        addr + 1);
                    s += strlen(s);
                    break;
                 case 'c':
                    s += sprintf(s, "%s", lc_branch_nzp[v]);
                    any = 0;
                    break;
                 case 'q':
                    break;
                 default:
                    abort();
                }
            }
        }
    }
    else if (ist < 127 && isgraph(ist))
        sprintf(s, ".FILL #%-3d ; '%c'", ist, ist);
    else
        sprintf(s, ".FILL #%d", LC_TO_SIGNED(ist));
}

/*
 * Disassemble a line.
 *
 * @param st - current machine line
 * @param f - output file
 * @param addr - the address of the line to disassemble
 */
void lc_disassemble(const struct lc_state *st, FILE *f, lcword_u addr)
{
    char buf[80];
    
    lc_disassemble_str(st, buf, addr);
    
    fprintf(f, "%s", buf);
}

/*

void xlate()
{
    "add ra, rb, rc" ->
        "movw b(%ebp), %ax"
        "movw c(%ebp), %bx"
        "add %ax, %bx"
        "movw %ax, a(%ebp)"
    "add ra, rb, c" ->
        "movw b(%ebp), %ax"
        "addw $c, %ax"
        "movw %ax, a(%ebp)"
    "and ra, rb, rc" ->
        "movw b(%ebp), %ax"
        "movw c(%ebp), %bx"
        "and %ax, %bx"
        "movw %ax, a(%ebp)"
    "and ra, rb, c" ->
        "movw b(%ebp), %ax"
        "andw $c, %ax"
        "movw %ax, a(%ebp)"
    "not ra, rb" ->
        "movw b(%ebp), %ax"
        "notw %ax"
        "movw %ax, b(%ebp)"
    "lea addr" ->
        "movw $addr, %ax"
    
}

*/

/*
 * Give a human-readable version of the instruction, that tries to guess
 * at intention.
 *
 * Maximum size: <35 characters
 *
 * @param st - the machine
 * @param buf - the string to write to
 * @param addr - the address of the instruction to print
 */
void lc_symbolic_str(const struct lc_state *st, char *buf, lcword_u addr)
{
#define ADDR(x) (lc_symbolic_addr(st, addr_buf, sizeof(addr_buf), x, 0), addr_buf)
    int d;
    int s2;
    int s;
    char addr_buf[15];
    lcword_u ist = st->mem[addr];
    int pc = addr + 1;
    
    *buf = '\0';
    
    switch(LC_GET_U(ist, 12, 4))
    {
     case IST_TRAP:
        if (ist == 0xf025)
            sprintf(buf, "halt");
        else if (ist == 0xf020)
            sprintf(buf, "R0 <- input char");
        else if (ist == 0xf021)
            sprintf(buf, "disp R0 char");
        else if (ist == 0xf022)
            sprintf(buf, "disp R0 zstring");
        else if (ist == 0xf023)
            sprintf(buf, "R0 <- prompt char");
        else if (ist == 0xf024)
            sprintf(buf, "disp R0 packed string");
        else if ((ist & 0x0f00) == 0)
            sprintf(buf, "trap x%02X", (int)LC_GET_U(ist, 0, 12));
        break;
        
     case IST_ADD:
        d = (int)D_AR;
        s = (int)D_SR1;
        /*printf("%4X\n", D_IMM(5));*/
        if (!(ist & (1<<5)))
        {
            if (!(ist & (3 << 3)))
                sprintf(buf, "R%d <- R%d + R%d", d, s, (int)D_SR2);
        }
        else if ((ist & (1<<4)))
            sprintf(buf, "R%d <- R%d - %d", d, s, (int)(0x10000-D_IMM(5)));
        else if (D_IMM(5) != 0)
            sprintf(buf, "R%d <- R%d + %d", d, s, (int)D_IMM(5));
        else if (d != s)
            sprintf(buf, "R%d <- R%d", d, s);
        else
            sprintf(buf, "R%d test", s);
        break;
        
     case IST_AND:
        d = (int)D_AR;
        s = (int)D_SR1;
        if (!(ist & (1<<5)))
        {
            if (!(ist & (3 << 3)))
            {
                s2 = (int)D_SR2;
                if (s == d && s2 == s)
                    sprintf(buf, "R%d test", d);
                else if (s2 == s)
                    sprintf(buf, "R%d <- R%d", d, s);
                else
                    sprintf(buf, "R%d <- R%d & R%d", d, s, s2);
            }
        }
        else if ((ist&31) == 0)
            sprintf(buf, "R%d <- 0", d);
        else if ((ist&31) == 31)
        {
            if (s == d)
                sprintf(buf, "R%d test", s);
            else
                sprintf(buf, "R%d <- R%d", d, s);
        }
        else
            sprintf(buf, "R%d <- R%d & x%04X", d, s, (int)D_IMM(5));
        break;
        
     case IST_NOT:
        d = (int)D_AR;
        s = (int)D_SR1;
        /*
        if (!(ist&(1<<5)))
        {
            if (!(ist & (3<<3)))
            {
                s2 = (int)D_SR2;
                if (s == s2)
                    sprintf(buf, "R%d <- 0", d);
                else
                    sprintf(buf, "R%d <- R%d ^ R%d", d, s, s2);
            }
        }
        else if ((ist&31) == 0 && s == d)
            sprintf(buf, "R%d test", d);
        else if ((ist&31) == 0)
            sprintf(buf, "R%d <- R%d", d, s);
        else */
        if ((ist&31) == 31)
            sprintf(buf, "R%d <- ~R%d", d, s);
        /*
        else
            sprintf(buf, "R%d <- R%d ^ x%04X", d, s, (int)D_IMM(5));
        */
        break;
        
     case IST_LEA:
        sprintf(buf, "R%d <- %s", (int)D_AR, ADDR(D_OFF(9)));
        break;
        
     case IST_LD:
        sprintf(buf, "R%d <- m[%s]", (int)D_AR, ADDR(D_OFF(9)));
        break;
     case IST_LDI:
        sprintf(buf, "R%d <- m[m[%s]]", (int)D_AR, ADDR(D_OFF(9)));
        break;
     case IST_LDR:
        if (!(ist & 31))
            sprintf(buf, "R%d <- m[R%d]", (int)D_AR, (int)D_SR1);
        else if (ist & (1<<5))
            sprintf(buf, "R%d <- m[R%d - %d]",
                (int)D_AR, (int)D_SR1, (int)(0x10000-D_IMM(6)));
        else
            sprintf(buf, "R%d <- m[R%d + %d]",
                (int)D_AR, (int)D_SR1, (int)D_IMM(6));
        break;
     case IST_ST:
        sprintf(buf, "m[%s] <- R%d", ADDR(D_OFF(9)), (int)D_AR);
        break;
     case IST_STI:
        sprintf(buf, "m[m[%s]] <- R%d", ADDR(D_OFF(9)), (int)D_AR);
        break;
     case IST_STR:
        if (!(ist & 31))
            sprintf(buf, "m[R%d] <- R%d", (int)D_SR1, (int)D_AR);
        else if (ist & (1<<5))
            sprintf(buf, "m[R%d - %d] <- R%d",
                (int)D_SR1, (int)(0x10000-D_IMM(6)), (int)D_AR);
        else
            sprintf(buf, "m[R%d + %d] <- R%d",
                (int)D_SR1, (int)D_IMM(6), (int)D_AR);
        break;
     case IST_BR: /* BR 0x0... */
        if (LC_GET_U(ist, 0, 9) == 0)
            sprintf(buf, "no-op (.fill %d)", (int)ist);
        else if (LC_GET_U(ist, 9, 3) == 7)
            sprintf(buf, "PC <- %s", ADDR(D_OFF(9)));
        else if (LC_GET_U(ist, 9, 3) != 0)
            sprintf(buf, "if%s, PC <- %s",
                lc_branch_names[LC_GET_U(ist, 9, 3)],
                ADDR(D_OFF(9))
                );
        else if (ist < 128 && isgraph(ist))
            sprintf(buf, "no-op (.fill %-3d '%c')", (int)ist, (char)ist);
        else
            sprintf(buf, "no-op (.fill %d)", (int)ist);
        break;
        
     case IST_JMP:
        if (!(ist & 0x0e3f))
            sprintf(buf, "PC <- R%d%s", (int)D_SR1,
                ist==0xC1C0 ? " (return)" : "");
        break;
        
     case IST_JSR:
        /* I've been experimenting with how to satisfy two things:
         * - It's easy to glance and tell it's a function call
         * - It's obvious that it modifies R7 (this is important!!!!!)
         * I think this format does the best job so far.
         */
        if (ist & (1<<11))
            sprintf(buf, "call %s, R7 <- x%04X", ADDR(D_OFF(11)), pc);
        else if (!(ist & 0x0e3f))
            sprintf(buf, "PC <- R%d, R7 <- x%04X", (int)D_SR1, pc);
        break;
        
     default:
        break;
        /*sprintf(buf, "inval (.fill x%04X)", (int)ist);*/
    }
    
    /* invalid fall-through case */
    if (!*buf)
        sprintf(buf, "inval (.fill %d)", LC_TO_SIGNED(ist));
    
    #undef ADDR
}

/*
 * Give a human-readable version of the instruction, that tries a little bit
 * to guess what the instruction means.  For examples see documentation on
 *
 * @param f - the file to print to
 * @param ist - the instruction to print
 * @param pc - the current program counter; needed to calculate PC-relative
 *        offsets
 */
void lc_symbolic(const struct lc_state *st, FILE *f, lcword_u addr)
{
    char buf[80];
    
    lc_symbolic_str(st, buf, addr);
    
    fprintf(f, "%s", buf);
}

/*
 * Translate a single line to the output.
 * @param out - output file
 * @param prefix - string to begin every line with
 * @param spot - the program counter or memory location where instr is located
 * @param val - value to translate
 */
static void lc_trans_line(struct lc_state *st, FILE *out, const char *prefix,
    lcword_u spot)
{
    lcword_u val = st->mem[spot];
    
    fprintf(out, "%sx%04X x%04X  ", prefix, spot, val);
    lc_printbin(out, val, 0);
    fprintf(out, " ; ");
    lc_symbolic(st, out, spot);
    (void) fputc('\n', out);
}

/*
 * Translate several lines that are the same (or just 1).
 * If it's a small number of repetitions, it will print each; otherwise, it
 * will condense them down with an ellipsis.
 * @param out - output file
 * @param prefix - prefix string to begin every line with
 * @param spot - the memory location at which this instr is
 * @param count - how many occurences there are beginning at spot, probably
 *        usually 1
 * @param val - the value to print
 */
static void lc_trans_lines(struct lc_state *st, FILE *out,
    const char *prefix, lcword_u spot, int count)
{
    int i;
    
    if (count < 8)
    {
        for (i = 0; i < count; i++)
            lc_trans_line(st, out, prefix, spot + i);
    }
    else
    {
        lc_trans_line(st, out, prefix, spot);
        fprintf(out, "%s ... (last line repeated %d/x%04X additional times)\n",
            prefix, count-2, count-2);
        lc_trans_line(st, out, prefix, spot + count - 1);
    }
}

/*
 * This will read a .obj/.hex/.bin file, and print out its contents in the
 * following fashion, putting prefix at the beginning of each line.
 *
 * @code
 * | x3000 x5020  AND 000 000 1 00000 ; R0 <- 0
 * | x3001 x1021  ADD 000 000 1 00001 ; R0 <- R0 + x0001
 * | x3002 x221C  LD  001   000011100 ; R1 <- mem[x301F]
 * | x3003 xE420  LEA 010   000100000 ; R2 <- PCi + x0020 (x3024)
 * | x3004 x1681  ADD 011 010 0 00001 ; R3 <- R2 + R1
 * | x3005 x70C0  STR 000 011  000000 ; mem[R3 + x0000] <- R0
 * | x3006 x0000  BR  000   000000000 ; no-op
 * |  (repeated 3001/x0BB9 additional times)
 * @endcode
 *
 * @param f - the file to read from
 * @param readword - the way to read words from f
 * @param out - the output file to print to
 * @param prefix - the prefix to print every line - in the example it was
 *        " | "
 */
int lc_trans(FILE *f, lc_readword_t readword, FILE *out, const char *prefix)
{
    int spot;
    int lastval = 0;
    struct lc_parse parser;
    int count = 0;
    int val;
    struct lc_state st;
    
    parser.readword = readword;
    parser.f = f;
    parser.line = 0;
    parser.error = 0;
    parser.printerr_obj = stderr;
    parser.printerr_func = lc_printerr;
    
    if ((spot = parser.readword(&parser)) < 0)
        return -1;
    if ((lastval = parser.readword(&parser)) < 0)
        return -1;
    if (lc_init(&st) < 0) return -1;
    do {
        st.mem[spot] = lastval;
        spot++;
        count++;
        val = parser.readword(&parser);
        if (val != lastval || val < 0)
        {
            lc_trans_lines(&st, out, prefix, (spot - count) & 0xffff,
                count);
            count = 0;
        }
        lastval = val;
    } while (val >= 0);
    
    lc_destroy(&st);
    
    return (parser.error == 0) ? 0 : -1;
}

/*
 * This will read the contents of an LC-state and disassemble it nicely.
 *
 * @code
 * | x3000 x5020  AND 000 000 1 00000 ; R0 <- 0
 * | x3001 x1021  ADD 000 000 1 00001 ; R0 <- R0 + x0001
 * | x3002 x221C  LD  001   000011100 ; R1 <- mem[x301F]
 * | x3003 xE420  LEA 010   000100000 ; R2 <- PCi + x0020 (x3024)
 * | x3004 x1681  ADD 011 010 0 00001 ; R3 <- R2 + R1
 * | x3005 x70C0  STR 000 011  000000 ; mem[R3 + x0000] <- R0
 * | x3006 x0000  BR  000   000000000 ; no-op
 * |  (repeated 3001/x0BB9 additional times)
 * @endcode
 *
 * @param st - the state
 * @param out - the output file to print to
 * @param prefix - the prefix to print every line - in the example it was
 *        " | "
 * @param start - the memory address to start reading from
 * @param icount - number of lines to disassemble
 */
void lc_trans_state(struct lc_state *st, FILE *out, const char *prefix,
    lcword_u start, int icount)
{
    lcword_u end = start + icount;
    lcword_u spot;
    lcword_u val;
    lcword_u lastval = st->mem[start];
    int count = 0;
    
    for (spot = start; spot != end;)
    {
        spot++;
        count++;
        val = st->mem[spot];
        if (val != lastval || spot == end)
        {
            lc_trans_lines(st, out, prefix, (spot - count) & 0xffff,
                count);
            count = 0;
        }
        lastval = val;
    }
}

#define bold(s, a) if(use_color)s+=sprintf(s,"\033[%sm",a);else(void)0

/*
 * Print the current state of the LC-3 machine.
 *
 * Uses colors when available.
 *
 * @param st - the state
 * @param s - string to write to
 * @param flags - print out flags
 */
char *lc_print_state_str(const struct lc_state *st, char *s, int flags,
        const lcword_u *old_regs)
{
    int use_color = flags & LC_PA_COLOR;
    const char *name;
    int i;
    
    for (i = 0; i < 8; i += 4)
    {
        int j;
        
        s += sprintf(s, " ");
        for (j = i; j < i+4; j++)
        {
            int v = st->reg[j];
            int changed = (old_regs && v != old_regs[j]);
            #ifdef LC_IPROFILE
            changed = changed || (j == st->destreg);
            #endif
            /*s += sprintf(s, "");*/
            bold(s, changed ? "1;32" : "1");
            s += sprintf(s, "r%d", j);
            bold(s, "0");
            s += sprintf(s, " ");
            if (changed) { bold(s, "1;34"); }
            s += sprintf(s, "x%04x ", v);
            if (st->symt && (name = symt_getname(st->symt, v)) != NULL)
                s += sprintf(s, "%-10s", name);
            else if (v < 128 && isgraph(v))
                s += sprintf(s, "%-3d '%c'   ", v, v);
            else
                s += sprintf(s, "%-6d    ", LC_TO_SIGNED(v));
            bold(s, "0");
        }
        s += sprintf(s, "\n");
    }
    
    s += sprintf(s, " ");
    s += sprintf(s, " cc(%s) ",
        st->flag == LC_FL_P ? "+" : st->flag == LC_FL_N ? "-" : "0");
    /*s += sprintf(s, "\n")*/
    s += sprintf(s, "Now at ");
    bold(s, "1;32");
    s += sprintf(s, "x%04X", st->pc);
    if (st->symt && (name = symt_getname(st->symt, st->pc)) != NULL)
    {
        bold(s, "1;35");
        s += sprintf(s, " %s", name);
    }
    bold(s, "0");
    s += sprintf(s, ": ");
    bold(s, "33");
    if (flags & LC_PA_SYMB)
        lc_symbolic_str(st, s, st->pc);
    else
        lc_disassemble_str(st, s, st->pc);
    s += strlen(s);
    bold(s, "0");
    s += sprintf(s, "\n");
    
    return s;
}

/*
 * Print a detailed listing of a particular address.
 */
void lc_print_addr_str(const struct lc_state *st, char *s, int addr, int flags)
{
#define BOLD(fmt) if (use_color) s += sprintf(s, "\033[%sm", fmt); else (void)0
    const char *symname;
    int use_color = flags & LC_PA_COLOR;
    int val = st->mem[addr];
    
    if (!(flags & LC_PA_EXEC))
    {
        if (st->symt && (symname = symt_getname(st->symt, addr)))
        {
            int i;
            BOLD("1;35");
            strncpy(s, symname, 15);
            for (i = strlen(symname); i < 16; i++)
                s[i] = ' ';
            s += i;
        }
        else
        {
            s += sprintf(s, "%16s", "");
        }

        BOLD("0;32");
        s += sprintf(s, "%04X", addr);
        BOLD("0");
        s += sprintf(s, ": ");
    }
    else
    {
        BOLD("0;32");
        s += sprintf(s, "%04X", addr);
        BOLD("0");
        s += sprintf(s, ": ");
    }
    
    BOLD("1;34");
    s += sprintf(s, "%04X ", val);
    
    if (!(flags & LC_PA_EXEC))
    {
        lc_printbin_str_format(s, val, use_color, lc_ist_formats);
        s += strlen(s);
        *s++ = ' ';
    }
    
    BOLD("0;36");
    s += sprintf(s, "%-6d ", LC_TO_SIGNED(val));
    BOLD("0");
    
    if (flags & LC_PA_EXEC)
    {
        lc_printbin_str(s, val, use_color);
        s += strlen(s);
        s += sprintf(s, " ");
        if (!st->symt || !(symname = symt_getname(st->symt, addr)))
        {
            BOLD("0;35");
            s += sprintf(s, "    %04X%5s", addr, st->pc == addr ? "->" : "");
        }
        else
        {
            BOLD("1;35");
            s += sprintf(s, st->pc == addr ? "%-11s->" : "%-12s ", symname);
        }
        BOLD("0");
        if (flags & LC_PA_SYMB)
            lc_symbolic_str(st, s, addr);
        else
            lc_disassemble_str(st, s, addr);
        s += strlen(s);
    }
    else
    {
        if (val < 128 && isgraph(val))
        {
            BOLD("0;33");
            s += sprintf(s, "'%c' ", val);
        }
        else if (st->symt && (symname = symt_getname(st->symt, val)))
        {
            BOLD("0;34");
            s += sprintf(s, "%s", symname);
        }
    }
    
    BOLD("0");
    /*printf("\n");*/
}
#undef BOLD

void lc_print_addr(const struct lc_state *st, FILE *f, int addr, int flags)
{
    char buf[1024];
    lc_print_addr_str(st, buf, addr, flags);
    fputs(buf, f);
}

/*
 * Standard error-printing mechanism to a FILE * output stream.
 * 
 * @param vfile - FILE * pointer to print error to
 * @param msg - the message to print
 */
void lc_printerr(void *vfile, const char *msg)
{
    (void) fprintf(vfile, "[lc3sim] %s\n", msg);
}

/*
 * Make a warning about something happening in the assembler.
 *
 * @param st - the state to warn about
 * @param format - format string to show, plus arguments afterward
 * @param type - a unique number on the error type so we don't show
 *        thousands of repeated messages
 */
static void lc_warn(struct lc_state *st, int type, const char *format, ...)
{
    char buf[1024];
    char *s = buf;
    va_list vl;
    
    st->warning++;
    
    if (st->printerr_obj)
    {
        if (st->warning < 8 || !(st->errset & (1 << type)))
        {
            va_start(vl, format);
            s += sprintf(s, "pc=x%04X: ", st->pc-1);
            s += vsprintf(s, format, vl);
            *s++ = 0;
            st->printerr_func(st->printerr_obj, buf);
            st->errset |= (1<<type);
        }
        else if ((st->warning & (st->warning-1)) == 0)
        {
            sprintf(buf, "pc=x%04X: Over %d similar warnings (omitted)!",
                st->pc-1, st->warning-1);
            st->printerr_func(st->printerr_obj, buf);
        }
    }
}

/*--------------------------------------------------------------------------*/
/* Input and Output */
/*--------------------------------------------------------------------------*/

/* Ala fgetc. */
int lc_file_read(void *obj)
{
    return fgetc(obj);
}

/* Ala fputc. */
int lc_file_write(void *obj, int c)
{
    return fputc(c, obj);
}

/*
 * Output one character to the state's output buffer.
 *
 * @param st - the machine to output to
 * @param c - the character
 * @return negative on error, zero on success
 */
int lc_buffer_write(void *obj, int c)
{
    struct lc_state *st = obj;
    
    if (st->outpos >= LC_CONSOLE_SIZE)
    {
        lc_warn(st, 1, "Console buffer has all %d characters full!",
            LC_CONSOLE_SIZE);
        return EOF;
    }
    
    st->output[st->outpos] = (char)c;
    st->outpos++;
    st->output[st->outpos] = 0;
    
    return 0;
}

/*
 * Read in one character from the input buffer.
 *
 * @param st - the lc3's state
 * @return the inputted character, or EOF if end of input
 */
int lc_buffer_read(void *obj)
{
    struct lc_state *st = obj;
    int value = st->input[st->inpos];
    
    if (value != 0) /* don't increment past end of input */
        st->inpos++;
    else
        value = EOF; /* halt if at end of input */
        
    return value;
}

/*--------------------------------------------------------------------------*/
/* Simulation Functions */
/*--------------------------------------------------------------------------*/

/*
 * Read a character from proper stream.
 *
 * @return EOF on error, the character read 0..255 otherwise
 */
static int lc_input_char(struct lc_state *st)
{
    int result;
    
    st->pc--; /* so the display will make sense */
    result = st->read_func(st->read_obj);
    st->pc++;
    
    if (result == EOF || st->halted)
    {
        st->pc--; /* Put pc back, in case of resume */
        if (!st->halted)
            lc_warn(st, 2, "Reading beyond end of input -- halting.");
        st->halted = 1;
    }
    
    return result;
}

/*
 * Write a character to the proper output stream.
 */
static int lc_output_char(struct lc_state *st, int c)
{
    int result;
    
    if (c > 256)
        lc_warn(st, 3, "Outputting character %d with high-byte", c);
    
    result = st->write_func(st->write_obj, c&0xff);
    
    if (result == EOF)
    {
        lc_warn(st, 4, "Error trying to write '%c' to output", c);
        return EOF;
    }
    
    return 0;
}

/*
 * Print a string to whatever output stream.
 *
 * @param st - the state
 * @param str - the string to write
 * @return EOF on error, 0 on success
 */
static int lc_output_str(struct lc_state *st, const char *str)
{
    for (; *str; str++)
    {
        if (lc_output_char(st, *str) != 0)
            return EOF;
    }
    return 0;
}

/*
 * Set the flags based on the value s.
 * @param st - the state
 * @param s - an LC word to set the state's flags on
 */
static inline void lc_setflag(struct lc_state *st, lcword_s s)
{
    if (s & (1<<15))
       st->flag = LC_FL_N;
    else if (s == 0)
       st->flag = LC_FL_Z;
    else
       st->flag = LC_FL_P;
}

static void lc_m_set_special(struct lc_state *st, lcword_u addr, lcword_u val)
{
    switch(addr)
    { /* Unfortunately I'm lazy and actually storing the I/O in mem */
      case LC_DDR:
        if (st->mem[LC_DSR])
        {
            st->mem[LC_DSR] = 0;
            (void) st->write_func(st->write_obj, val);
        }
        else
            lc_warn(st, 5, "Writing to DDR when display is not ready; data ignored!");
        break;
      case LC_MCR:
        if (!(val & 0x8000))
        {
            if (!st->real_traps)
                lc_warn(st, 12, "Turning off machine via MCR");
            st->halted = 1;
        }
        break;
      default:
        if (!LC_PRIV(st))
            lc_warn(st, 6, "Writing x%04X to reserved memory at x%04X!",
                val, addr);
    }

}

/*
 * Set a memory location to a value.
 *
 * Currently this doesn't really do much.  This might be useful later for
 * intercepting certian memory locations or if we decide to actuaooy
 * to implement memory access typpe instructions.
 *
 * @param st - the state whose memory to set
 * @param addr - the memory address
 * @param val - the value to set that memory address to
 */
static inline void lc_m_set(struct lc_state *st, lcword_u addr, lcword_u val)
{
    if (((addr - 0x1000) & 0xffff) >= 0xEE00)
    {
        lc_m_set_special(st, addr, val);
    }
    st->mem[addr] = val;
}

static void lc_m_get_special(struct lc_state* st, int addr)
{
    switch(addr)
    { /* Unfortunately I'm lazy and actually storing the I/O in mem */
      /* Everything is randomized... it's the best way I can enforce
       * polling model without having it take millions of instructions.
       */
      case LC_KBSR:
        if (rand() % 16 < 5)
        {
            st->mem[LC_KBDR] = (lcword_u) lc_input_char(st);
            if (st->mem[LC_KBDR])
                st->mem[LC_KBSR] = 1<<15;
        }
        break;
      case LC_KBDR:
        if (st->mem[LC_KBSR])
        {
            st->mem[LC_KBSR] = 0;
        }
        else
        {
            lc_warn(st, 7, "Reading from keyboard when no data ready");
            st->mem[LC_KBDR] = 0;
        }
        break;
      case LC_DSR:
        if (st->outpos < LC_CONSOLE_SIZE && rand() % 4 < 1)
        {
            st->mem[addr] = 1<<15;
        }
        break;
      case LC_MCR:
        st->mem[addr] = 1<<15;
        break;
      default:
        if (!LC_PRIV(st))
            lc_warn(st, 8, "Reading from reserved memory at x%04X!",
                addr);
    }
}

/*
 * Read from a memory location to a value.
 *
 * Currently this doesn't really do much.  This might be useful later for
 * intercepting certian memory locations or if we decide to actuaooy
 * to implement memory access typpe instructions.
 *
 * @param st - the state whose memory to set
 * @param addr - the memory address
 * @return the memory at that address
 */
static inline lcword_u lc_m_get(struct lc_state *st, lcword_u addr)
{
    if (((addr - 0x1000) & 0xffff) >= 0xEE00)
    {
        lc_m_get_special(st, addr);
    }
    return st->mem[addr];
}


/*
 * Execute a trap.
 *
 * @param st - the machine performing a trap
 * @param number - the trap number
 */
static void lc_trap(struct lc_state *st, int number)
{
    int ptr;
    int val;
    int count;
    
    if (st->real_traps)
    {
        st->reg[7] = st->pc;
        st->pc = st->mem[number];
    }
    else
    {
        st->reg[7] = st->pc;
        switch(number)
        {
         case 0x25: /* halt */
            /* We subtract 1 from pc so that any attempt to restart the
             * machine without changing the PC will result in a halt.
             */
            st->pc--;
            st->halted = 1;
            break;
            
         case 0x20: /* input */
            st->reg[0] = lc_input_char(st);
            break;
            
         case 0x21: /* put character */
            (void) lc_output_char(st, st->reg[0]);
            break;
            
         case 0x22:
            ptr = st->reg[0];
            count = 0;
            while ((val = lc_m_get(st, ptr)) != 0)
            {
                if (lc_output_char(st, val) != 0)
                    break;
                ptr++;
                if (++count > 65536)
                {
                    lc_warn(st, 20, "PUTS string is not terminated");
                    break;
                }
            }
            break;
            
         case 0x23: /* input w/ prompt and echo */
            (void) lc_output_str(st, "Input a character> ");
            st->reg[0] = lc_input_char(st);
            (void) lc_output_char(st, st->reg[0]);
            (void) lc_output_char(st, '\n');
            break;
            
         case 0x24:
            ptr = st->reg[0];
            count = 0;
            while ((val = lc_m_get(st, ptr)))
            {
                if (lc_output_char(st, val & 0xFF) != 0)
                    break;
                if ((val & 0xFF00) != 0)
                    if (lc_output_char(st, val >> 8) != 0)
                        break;
                if (++count > 65536)
                {
                    lc_warn(st, 20, "PUTSP string is not terminated");
                    break;
                }
                ptr++;
            }
            break;
            
         default:
            lc_warn(st, 9, "Unsuppored trap x%02X; assuming HALT", number);
            st->halted = 1;
        }
    }
}

/*
 * Run one instruction in the LC simulator.
 *
 * @param st - the state of the LC machine
 * @return 0 if the machine has halted, 1 in any other case
 */
int lc_run_one(struct lc_state *st)
{
    lcword_u ist;
    int opcode;
    int d;
    int s;
    lcword_u v;
    lcword_u pc;
    
    ist = lc_m_get(st, st->pc);
    pc = ++st->pc;
    opcode = LC_GET_U(ist, 12, 4);
    PROF_ONLY(st->icounts[opcode]++);
    
    d = D_AR;
    
    switch(opcode)
    {
     case IST_TRAP:
        lc_trap(st, ist&0xFF);
        PROF_ONLY(d=-1);
        break;
        
     case IST_ADD:
        if (ist & (1<<5))
            v = D_IMM(5);
        else
            v = st->reg[D_SR2];
        s = D_SR1;
        lc_setflag(st, st->reg[d] = st->reg[s] + v);
        break;
     case IST_AND:
        if (ist & (1<<5))
            v = D_IMM(5);
        else
            v = st->reg[D_SR2];
        s = D_SR1;
        lc_setflag(st, st->reg[d] = st->reg[s] & v);
        break;

     case IST_NOT:
        lc_setflag(st, (st->reg[d] = ~st->reg[D_SR1]));
        /*
        if (ist & (1 << 5))
            v = D_IMM(5);
        else
            v = st->reg[D_SR2];
        d = D_AR;
        s = D_SR1;
        lc_setflag(st, (st->reg[d] = st->reg[s] ^ v));
        */
        break;
        
     case IST_LEA:
        lc_setflag(st, (st->reg[d] = D_OFF(9)));
        break;
        
     case IST_LD:
        lc_setflag(st, (st->reg[d] = lc_m_get(st, D_OFF(9))));
        break;
     case IST_LDI:
        lc_setflag(st,
            (st->reg[d] = lc_m_get(st, lc_m_get(st, D_OFF(9)))));
        break;
     case IST_LDR:
        lc_setflag(st,
            (st->reg[d] = lc_m_get(st, ADD(st->reg[D_SR1], D_IMM(6)))));
        break;
     case IST_ST:
        PROF_ONLY(d=-1);
        lc_m_set(st, D_OFF(9), st->reg[D_AR]);
        break;
     case IST_STI:
        PROF_ONLY(d=-1);
        lc_m_set(st, lc_m_get(st, D_OFF(9)), st->reg[D_AR]);
        break;
     case IST_STR:
        PROF_ONLY(d=-1);
        lc_m_set(st, ADD(st->reg[D_SR1], D_IMM(6)), st->reg[D_AR]);
        break;
     case IST_BR:
        PROF_ONLY(d=-1);
        if ((st->flag & ist) != 0)
        {
            PROF_ONLY(st->branch++); /* we want to know # of branches taken... */
            st->pc = D_OFF(9);
        }
        break;
        
     case IST_JMP:
        st->pc = st->reg[D_SR1];
        PROF_ONLY(d = -1);
        break;
        
     case IST_JSR:
        /* Carefully note order of execution... This JSR does the "right"
         * thing, and the textbook authors note that they plan on changing
         * the book to note that it is implemented this way. */
        if (ist & (1<<11))
            st->pc = D_OFF(11); /* JSR */
        else
            st->pc = st->reg[D_SR1]; /* JSRR */
        st->reg[7] = pc;
        PROF_ONLY(d = 7);
        break;
        
     default:
        lc_warn(st, 10, "Unsupported instruction x%04X!", ist);
        PROF_ONLY(d = -1);
    }
    PROF_ONLY(st->destreg = d);
    return !st->halted;
}

/*
 * Run the code in the LC state.  Note that the program counter must be
 * initialized prior to running this; lc_init will initialize it to 0x3000.
 *
 * These operations are not supported:
 * - Return from interrupt
 * - Some traps.  However, if real_traps is set to true, any traps are
 * allowed.
 *
 * If an instruction is unsupported or illegal the st->warning will be
 * incremented for each such occurence.  It will also be incremented if the
 * execution runs out of time.
 *
 * All output is directed to the output buffer, unless input or output
 * streams are specified.  If the buffer is in use, excess characters are
 * ignored.
 *
 * @param st - the state to run
 * @param max_instructions - the maximum number of instructions to run
 * @return number of instructions executed not including halt, or -1 if
 *         max_instructions was reached before a halt
 */
int lc_run(struct lc_state *st, int max_instructions)
{
    int count;
    
    for (count = 0; count < max_instructions; count++)
    {
        if (!lc_run_one(st))
            return count;
    }
    
    lc_warn(st, 11, "Limit Exceeded (%d instructions, no HALT)", count);
    
    return -1;
}

/*--------------------------------------------------------------------------*/
/* Initialization and Destruction */
/*--------------------------------------------------------------------------*/

/*
 * Initializes an LC state.
 * This allocates the proper amount of memory and fills everything to zero.
 * This will also set the program counter to 0x3000, and set the flag to
 * "zero", ie LC_FL_Z.
 * @param st the state to initialize
 * @return negative on error
 */
int lc_init(struct lc_state *st)
{
    memset(st, 0, sizeof(*st));
    if ((st->mem = calloc(LC_MEM, sizeof(*st->mem))) == NULL)
    {
        return -1;
    }
    PROF_ONLY(st->destreg = -1);
    st->flag = LC_FL_Z; /* zero */
    /* These should be randomized, but for now I'm copying the real
     * machine's behavior... */
    st->reg[1] = 0x7FFF;
    st->reg[7] = 0x0490;
    st->pc = 0x3000; /* for lack of anywhere better to put it */
    st->read_obj = st;
    st->read_func = lc_buffer_read;
    st->write_obj = st;
    st->write_func = lc_buffer_write;
    st->symt = NULL;
    st->printerr_obj = stderr;
    st->printerr_func = lc_printerr;
    return 0;
}

/*
 * Destroy an LC state.
 * This basically frees st->mem (not st itself) and does any other
 * cleaning up that may be necessary.
 * @param st the state to clean
 */
void lc_destroy(struct lc_state *st)
{
    free(st->mem);
}

/*
 * Clone an LC state.
 */
struct lc_state *lc_copy(struct lc_state *dest, const struct lc_state *src)
{
    memcpy(dest, src, sizeof(*dest));
    if ((dest->mem = malloc(LC_MEM * sizeof(*dest->mem))))
    {
        memcpy(dest->mem, src->mem, LC_MEM * sizeof(*dest->mem));
        if (dest->read_obj == src) dest->read_obj = dest;
        if (dest->write_obj == src) dest->write_obj = dest;
        return dest;
    }
    return NULL;
}


/*
 * Read a file that is either a .bin, .hex, or .obj file and place it
 * into memory.  This will set the program counter at the origin specified.
 *
 * @param st - the state to read the code into
 * @param f - the file to read the code from
 * @param readword - the function used to grab a single word from a file;
 *        example for .bin files use lc_getword_bin.
 * @return negative on error; otherwise, the number of instructions read
 */
int lc_read(struct lc_state *st, FILE *f, lc_readword_t readword)
{
    int spot;
    struct lc_parse parser;
    int retval;
    
    parser.readword = readword;
    parser.f = f;
    parser.line = 0;
    parser.error = 0;
    parser.printerr_obj = st->printerr_obj;
    parser.printerr_func = st->printerr_func;
    
    if ((spot = parser.readword(&parser)) < 0)
        return -1;
    
    st->pc = spot;
    
    while ((retval = parser.readword(&parser)) >= 0)
    {
        st->mem[spot] = retval;
        spot = (spot + 1) & 0xffff;
    }
    
    if (parser.error == 0) retval = (spot - st->pc) & 0xffff;
    
    return retval;
}

/*
 * Set input to file.
 *
 * @param st - the lc3 machine
 * @param f - the file to read input from
 */
void lc_set_input(struct lc_state *st, FILE *f)
{
    st->read_func = lc_file_read;
    st->read_obj = f;
}

/*
 * Set output to file.
 *
 * @param st - the lc3 machine
 * @param f - the file to send output to
 */
void lc_set_output(struct lc_state *st, FILE *f)
{
    st->write_func = lc_file_write;
    st->write_obj = f;
}

/*
 * Load the operating system into the LC-3 state.
 * @param st - the system in question
 */
void lc_load_os(struct lc_state *st)
{
    lcword_u save_pc = st->pc;
    memcpy(st->mem + LC3OS_START_ADDR, lc3os_array, sizeof(lc3os_array));
    st->pc = 0x200;
    (void) lc_run(st, 1048576);
    st->pc = save_pc;
}

/*
 * Translate.
 *
 * @param st - the state of the LC machine
 * @return 0 if the machine has halted, 1 in any other case
 */
void lc_ctrans_one(const struct lc_state *st, char *buf, int pc)
{
    lcword_u ist;
    int opcode;
    int d;
    int s;
    lcword_u v;
    
    ist = st->mem[pc];
    opcode = LC_GET_U(ist, 12, 4);
    
    buf += sprintf(buf, "case 0x%04X:L_%04X:", pc, pc);
    
    switch(opcode)
    {
     case IST_TRAP:
        buf += sprintf(buf, "lc_trap(%d);", ist&0xFF);
        break;
        
     case IST_ADD:
     case IST_AND:
     {
        char c = (opcode == IST_ADD) ? '+' : '&';
        d = D_AR;
        s = D_SR1;
        if (ist & (1<<5))
        {
            v = D_IMM(5);
            buf += sprintf(buf, "res=(R%d=R%d%c%d);", d, s, c, v);
        }
        else
        {
            buf += sprintf(buf, "res=(R%d=R%d%cR%d);", d, s, c, D_SR2);
        }
        break;
     }
        
     case IST_NOT:
        d = D_AR;
        buf += sprintf(buf, "res=(R%d=~R%d);", d, D_SR1);
        break;
        
     case IST_LEA:
        d = D_AR;
        buf += sprintf(buf, "res=(R%d=%d);", d, D_OFF(9));
        break;
        
     case IST_LD:
        buf += sprintf(buf, "res=(R%d=mem[%d]);", D_AR, D_OFF(9));
        break;
     case IST_LDI:
        buf += sprintf(buf, "res=(R%d=mem[mem[%d]]);", D_AR, D_OFF(9));
        break;
     case IST_LDR:
        buf += sprintf(buf, "res=(R%d=mem[R%d+%d]);", D_AR, D_SR1, D_IMM(6));
        break;
     case IST_ST:
        buf += sprintf(buf, "mem[%d]=R%d;", D_OFF(9), D_AR);
        break;
     case IST_STI:
        buf += sprintf(buf, "mem[mem[%d]]=R%d;", D_OFF(9), D_AR);
        break;
     case IST_STR:
        buf += sprintf(buf, "mem[R%d+%d]=R%d;", D_SR1, D_IMM(6), D_AR);
        break;
     case IST_BR:
     {
        static const char *conds[] =
         { "0", "res>0", "res==0", "res>=0",
           "res<0", "res!=0", "res<=0", "1" };
        buf += sprintf(buf, "if(%s)goto L_%04X;",
            conds[(ist>>9)&7], D_OFF(9));
        break;
     }
        
     case IST_JMP:
        buf += sprintf(buf, "PC=R%d;break;", D_SR1);
        break;
        
     case IST_JSR:
        buf += sprintf(buf, "R%d=%d;break;", 7, pc);
        if (ist & (1<<11))
        {
            buf += sprintf(buf, "goto L_%04X;", D_OFF(11));
        }
        else
        {
            buf += sprintf(buf, "PC=R%d;break;", D_SR1);
        }
        break;
        
     default:
        buf += sprintf(buf, "abort();");
    }
}

void lc_ctrans(const struct lc_state *st, FILE *f, int min, int max)
{
    int i;
    for (i = min; i <= max; i++)
    {
        char buf[256];
        lc_ctrans_one(st, buf, i);
    }
    
}


