/*
Copyright (c) 2006, Garrett Franklin Boyer
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


#ifndef FORMAT_H
#define FORMAT_H

#include <cstdio>
#include "common.h"
#include "arch/locid.h"
#include "arch/option.h"

class Machine;
class SymTable;
class Arch;
class Reporter;
class AsmRule;

/**
 * Format is the common class that both the GUI and text interfaces use to
 * format and parse values for various fields on the system.  Typically,
 * Format is extended for each new architecture to implement formatState()
 * and potentially to add extra options to other methods.
 *
 * This class is somewhat beastly; there is probably some way to refactor it,
 * but I haven't found any solutions good enough to warrant so.  The way to
 * think of this class, in the big picture, is the "layer" that separates
 * text from bits.  Thus, the machine simply contains the the bytes, and the
 * Format class interprets those bytes so the UI can function as a simple
 * presentation layer.
 *
 * TODO: For parsing, explicitly pass the Reporter object (to make it clear
 * error messages might be issued).
 */
class Format
{
 public:
    /// Formatting context
    class Context
    {
     public:
        Context(const Machine* inMachine, const SymTable* inSymTable)
         : mMachine(inMachine), mSymTable(inSymTable)
         { }
        ~Context()
         { }
        
        const Machine& machine() const
         { return *mMachine; }
        const SymTable& symTable() const
         { return *mSymTable; }
        
     private:
        const Machine* mMachine;
        const SymTable* mSymTable;
    };

    
    /// Various display formats for formatVal
    enum DisplayFormat
    {
        DisplayHex=1, //< Show as hexadecimal (unsigned)
        DisplayDec=2, //< Show as decimal (signed)
        DisplaySym=4, //< Show as a symbol
        DisplayHexSym=(DisplayHex|DisplaySym), //< Show as a symbol, or hex if no symbol found
        DisplayDecSym=(DisplayDec|DisplaySym), //< Show as a symbol, or decimal if no symbol found
        DisplayFlag=8 //< Show as n, z, or p
    };
    
    /// View modes available at this level for the 'flags' parameters
    enum ViewModes
    {
        ViewColor = 0x10
    };

 public:
    /**
     * Construct a Format object given a particular panel layout.
     *
     * @param panelLayout - the layout of the instructions (see panelLayout())
     */
    Format(const Arch* inArch, const char *inPanelLayout);
    /**
     * Standard destructor.
     */
    virtual ~Format()
     { }
 
 public:
    /**
     * Format a decimal number.
     *
     * @param val - value to format
     * @param bits - number of bits (for two's complement interpretation)
     */
    static String formatDec(GVal val, int bits);
    /**
     * Format a number in hex.
     * 
     * @param val - value to format
     * @param bits - number of bits (for width)
     * @param prefix - the prefix for the hex number
     */
    static String formatHex(GVal val, int bits);

    /**
     * Format a value based on the format.
     * @param val - value to format
     * @param table - the symbol table to get the data from
     * @param flags - the preferred format
     * @param bits - the number of bits to print with
     */
    static String formatVal(GVal val,
        const SymTable& table, int flags = DisplayHex, int bits = 0);

    /**
     * Dump a character onto the end of a string such that plain text is
     * yellow-orange, whitespace is light blue, control codes are dark blue,
     * and unknowns are periods.
     *
     * @param useColor - whether to use color
     * @param pStr - the string to append to
     * @param val - the character (numbers > 256 assumed non-printable)
     */
    static void dumpChar(bool useColor, String* pStr, int val);
    
 public:
    /**
     * Format a particular part of the machine for display.
     *
     * @param context - the relevant context for formatting
     * @param location - the location being printed; i.e. so that different
     *        registers might be printed differently.  this is for formatting
     *        purposes only; the value provided does not have to match with
     *        the value in the machine.
     * @param value - the value being printed/formatted
     * @param flags - extra flags to pass to the formatter (defined by the
     *        subclasses); DisplayFormat codes are recognized here too
     * @return the formatted string version, potentially with ANSI colors
     */
    virtual String format(
        const Context& context, const LocID& location,
        GVal value, int flags = 0) const;
    
    /**
     * Format a particular specified value of the machine.
     *
     * @param context - the relevant context for symbol lookups, etc
     * @param location - the location on the machine
     * @return the formatted string version, potentially with ANSI colors
     */
    String format(const Context& context, const LocID& location) const;
    
    /**
     * Parse a string into a value.
     *
     * @param context - the relevant context for symbol lookups, etc
     * @param location - the location being parsed (i.e. so that a flags
     *        register might be parsed differently from others)
     * @param str - the string to parse
     */
    virtual SuccessT parse(
        const char *str, Reporter* reporter,
        const Context& context,
        const LocID& location, GVal* pVal) const;
    
    /**
     * Parse a string into a specified location on the machine.
     *
     * For instance, if a user wants to change "pc", then this will return
     * LocID::PC; if the user enters a number, most likely it will be
     * some sort of memory address.
     *
     * @param context - the relevant context for symbol lookups, etc
     * @param location - the location being parsed (i.e. so that a flags
     *        register might be parsed differently from others)
     * @param str - the string to parse
     * @param success - whether the parse was successful
     */
    virtual LocID parseDest(const char *str, Reporter* reporter,
        const Context& context) const;
    
    /**
     * Format a location on the system.
     */
    virtual String formatDest(const Context& context, const LocID& dest) const;
    
    /**
     * Format a line.
     *
     * This is more for the purpose of the big memory list view.
     *
     * Only MemByte, MemWord, and MemInstr are possible options for this.
     *
     * @param context - the relevant context for symbol lookups, etc
     * @param location - the memory address and type (byte, word, instruction)
     * @param flags - formatter-specific flags (unrelated to DisplayFormat)
     */
    virtual String formatLine(
        const Context& context, const LocID& location, int flags = 0) const;
    
    /**
     * Make a compressed dump of all the bytes.
     *
     * @param context - the relevant context for symbol lookups, etc
     * @param addr - the address to dump
     */
    virtual String dumpLine(const Context& context, GAddr addr, int flags = 0,
            GAddr highlightAddr = GAddr(-1))
        const;
    
    /**
     * Get the number of bytes per line for dump view.
     *
     * @param context - the relevant context for symbol lookups, etc
     */
    int calcDumpBytes() const;
    
    /**
     * Get all formatting option that are available for this.
     *
     * These options are architecture-specific, and may be passed to any
     * of the "flags" fields for formatLine, printState, or any other
     * virtual function.  They may or may not be honored, depending on
     * the individual implementation.
     *
     * For LC-3, these options include colorful display, and whether to use
     * symbolic disassembly.
     *
     * @return a list of the formatting options
     */
    const Array<Option>& formatOptions() const
     { return mLineOptions; }
    
    /**
     * Format the machine's overall state for output.
     * @param f - the stream to print the state to
     * @param context - the relevant context for symbol lookups, etc
     * @param flags - any formatter-specific flags
     */
    virtual String formatState(
        const Context& context, int flags = 0) const = 0;
    
    /**
     * Returns register panel layout.
     * 
     * The layout is specified in a format like:
     * "3:4:0+,1+,2+,3+|4+,5+,6+,7+|9+,p+,w,e".
     *
     * This specifies 3 rows, four columns.  The numbers specify the
     * register numbers that are being displayed, the commas separate
     * adjacent fields, and the pipes separate lines.  The plus signs
     * indicate that there is enough room for both hex and decimal
     * views.  The extra characters: "p" is PC, "w" is the warning-log
     * checkbox, and "e" is the "executed total" label.
     *
     * @return the register panel layout in string format
     */
    const char *panelLayout() const
     { return mPanelLayout; }
    
 
 public:
    /**
     * Disassemble an instruction at a particular address.
     *
     * @param machine - the machine with the instruction
     * @param symTable - the symbol table to reverse-lookup symbols from
     * @param addr - the address to be disassembled
     * @param flags - optional flags for disassembly (checked by subclasses)
     */
    virtual String disassemble(const Context& context,
        GAddr addr, int flags = 0) const;
    
    /**
     * Format the bits of an instruction such that each separate field as
     * specified by the assembly language is separated.  In effect, the
     * instruction is disassembled, and color sequences are used to label
     * plain MatchRules as black, but alternating between blue and white
     * backgrounds/foregrounds for all other fields.
     *
     * @param arch - the architecture to use for assembly information
     * @param instr - the instruction to format
     * @param useColor - whether to use color (or just print bits)
     */
    virtual String formatInstrBits(GInstr instr,
        bool useColor) const;

 protected:
    /**
     * Helper function to insert a color code into a string.
     *
     * @param useColor - whether to append anything
     * @param str - the string to append
     * @param colorstr - the color, such that "1;32" would be ^[[1;32m
     */
    static void color(bool useColor, String* str, const char* colorstr);
    /**
     * Calculate the number of digits to represent val as an unsigned number.
     *
     * @param val - the number that is pretended to be displayed
     * @param base - the base it will be displayed in
     */
    static int calcDigits(GVal val, int base);

 protected:
    /**
     * A generic expression parser, more implementation-wise.
     *
     * This uses ExprParser (in asm/asmrule.h) to parse the expression
     * so it accepts a wide variety of formats.
     *
     * @param s - the string to parse
     * @param context - the relevant context for symbol lookups, etc
     * @param pVal - where to place the final result
     * @param bits - the bit width of the value
     * @param isAddress - whether to treat it like an address
     *        (at one point this warned about decimal addresses; now, this
     *         parameter does nothing)
     *
     */
    SuccessT parseVal(const char *s, Reporter* reporter,
        const Context& context,
        GVal* pVal, int bits, bool isAddress) const;
    
    /**
     * Get the active assembly rule.
     */
    const AsmRule& asmRule() const;
    
 protected:
    const Arch *mArch;
    const char *mPanelLayout;
    Array<Option> mLineOptions;
};

#endif
