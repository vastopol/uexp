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

#ifndef ASMRULE_H
#define ASMRULE_H

#include "common.h"
#include "ui/reporter.h"
#include "model/symbol.h"
#include "arch/arch.h"
#include "format/format.h"


/**
 * An AsmTokenizer is a tokenizer appropriate for parsing assembly language.
 * It will tokenize only a single line.
 *
 * It has peek and gobble features and complete rollback.  It allows for
 * easy LL(k) parsing on any one particular line, since it stores the entire
 * line of text internally.
 */
class AsmTokenizer
 : public Reporter
{
 public:
    /**
     * Is this an identifier-allowed punctuation character?
     */
    static bool isIdentifierPunctuation(char c)
     { return strchr("_.$%", c) != 0; }
     
    
 public:
    /**
     * @param inFullLine - the line to tokenize
     */
    AsmTokenizer(const String& inFullLine, int lineNum,
        Reporter* inExternalReporter);
    
    /**
     * Get the current token.
     */
    const String& peek()
     { return mCurrentToken; }
    /**
     * Return the current token, progressing to the next.
     */
    String next()
     { String s = mCurrentToken; gobble(); return s; }
    /**
     * Progress to the next token.
     */
    void gobble();
    /**
     * Check if the token is equal to the specified string, and progress
     * to the next token (gobble) if so.
     *
     * @param str - the string to match
     * @return true if there is a match
     */
    bool match(const char* str);
    
    /** Does the current token correspond to a string? */
    bool isString() const { return mCurrentToken[0] == '"'; }
    /**
     * Parse the current string for escape characters.
     */
    String peekString() const;
    /**
     * Is the token a character literal?
     */
    bool isChar() const { return mCurrentToken[0] == '\''; }
    /**
     * Interpret the current token as a character literal and peek its value.
     */
    int peekChar() const;
    
    /**
     * Are there any tokens remaining?
     * @return true iff tokens are remaining
     */
    bool remaining() const
     { return mRemaining; }
    
    /**
     * Get the position of the beginning of the current token.
     * @return the current token's position
     */
    int pos() const
     { return mLineIndex - mCurrentToken.length(); }
    /**
     * Set the position to another position.
     * @param inPos - the new position
     */
    void setPos(int inPos)
     { if (inPos > int(mFullLine.length())) abort();
       mLineIndex = inPos; grabToken(); }
    
    /**
     * Clear contents of the entire line.
     */
    void clear();
    
    /**
     * Have errors been sent to this tokenizer?
     * @return true iff this tokenizer has errors
     */
    bool hasErrors() const
     { return mHasErrors; }
    
    /**
     * Get everything on the line after the current token.
     */
    String rest() const
     { return mFullLine(mLineIndex, -1); }
    
    /**
     * Handle a message (per Reporter interface).
     * This will generate a message to the reporter specifying the line
     * number and current token, then progress the counter.
     * @param type - the message type
     * @param s - the string to report
     */
    void msg(Reporter::MsgType type, const char *s);
    
 private:
    /** Assume mLineIndex is set; compute the current token
     * and increment mLineIndex to the end of the token. */
    void grabToken();
    
    static const char *processChar(const char* str, char* pCh);
    
 private:
    /// whether any errors have been encountered
    bool mHasErrors;
    /// whether any tokens are remaining
    bool mRemaining;
    /// the text of the current token
    String mCurrentToken;
    /// the entire line of text
    String mFullLine;
    /// the index in the line of where to look for the next token
    int mLineIndex;
    /// the line number (for error message printing)
    int mLineNum;
    /// the external reporter to relay messages to
    Reporter* mExternalReporter;
};

RELOCATABLE_TYPE(AsmTokenizer);

/**
 * AsmRule abstracts any assembly-to-bits rule.
 *
 * The design is generic enough to allow almost any assembly language to
 * be expressed for both assembly and disassembly.  The two most important
 * rules, in order to understand how parsing works, are CompoundRule and
 * MuxRule.
 *
 * MEMORY NOTE: There is currently no scheme in place for freeing up a graph
 * of AsmRules.  Since they are generated only once upon loading, this is
 * not a priority.
 */
class AsmRule
{
 public:
    /**
     * AsmContext refers to the context in which one particular instruction is
     * being assembled.
     *
     * This is useful especially for symbol lookup and for PC-offset decoding.
     *
     * This class is to avoid having to pass the
     * same parameters to every assembly-related function.  Using this, it
     * means more parameters can be added or removed without affecting all
     * the function prototypes.
     */
    class AsmContext
    {
     public:
        /**
         * Create an AsmContext for a particular address, symbol table, and
         * architecture.
         *
         * @param inAddr - the address
         * @param inSymTable - the symbol table
         * @param inArch - the architecture
         */
        AsmContext(GAddr inAddr, const SymTable* inSymTable, const Arch* inArch)
         : mAddr(inAddr), mSymTable(inSymTable), mArch(inArch)
        {
        }
        
        /// Get the address of the instruction being assembled.
        const GAddr addr() const
         { return mAddr; }
        void setAddr(GAddr inAddr)
         { mAddr = inAddr; }
        /// Get the symbol table in use.
        const SymTable& symTable() const
         { return *mSymTable; }
        /// Get the architecture being assembled for.
        const Arch& arch() const
         { return *mArch; }
        /// Get the symbol name of a particular address.
        const char* getSymbolName(GAddr addr) const
         { return mSymTable->getname(addr); }
        
     private:
        /// the address being assembled to (for pc-relative assembly)
        GAddr mAddr;
        /// the symbol table
        const SymTable* mSymTable;
        /// the architecture
        const Arch* mArch;
    };

 public:
    /** Virtual destructor. */
    virtual ~AsmRule()
     { }
    
    /**
     * Determine if this rule matches a particular integer instruction,
     * for disassembling.
     *
     * @return true iff there is a match
     */
    virtual bool matches(GInstr instr) const;
    /**
     * "Claim" which bits of an instruction belong to what rule.
     * In each element of bitArray will be placed a pointer to the rule
     * that is most interested in a particular bit.  This is used
     * particularly for breaking up the bits visually for user view.
     *
     * @param instr - the instruction
     * @param bitArray - an array of const AsmRule pointers to place the
     *                 claims in
     */
    virtual void claimBits(GInstr instr, AsmRule const ** bitArray) const;
    /**
     * Disassemble an instruction into a string.
     *
     * @param context - the context of the expression
     * @param instr - the instruction being disassembled
     * @return the string disassembly as specified by the tree
     */
    virtual String disassemble(const AsmContext& context, GInstr instr) const;
    /**
     * Try to assemble a sequence of tokens.  This assumes that all
     * labels have already been pulled off the beginning of the line.
     * The tokenizer will be left such that the current token is the
     * last unused token; in the case of a failure, the current token
     * should be the token at fault for failure.
     *
     * @param context - the context being assembled into
     * @param tokenizer - the tokenizer to get tokens from
     * @param pInstr - where to place the instruction
     * @return the success of the assembly
     */
    virtual SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const;
    /**
     * Extract all literal tokens or reserved words from the assembly tree
     * into a table.  This is used in order to differentiate labels at the
     * beginning of a line from instructions, etc.
     */
    virtual void extractTokens(SymTable* table) const;
};

/**
 * A TableRule corresponds to a slice of the instruction where the bits
 * correspond to values in a symbol table.  This is useful for registers,
 * and also for shorthand pseudonyms for instructions (like "halt" stands
 * for trap x0fff in LC-3).
 *
 * A TableRule matches a binary instruction if the particular bits match
 * any of the values in the symbol table.  It will also provide a match for
 * assembly if the current token matches any of the strings in the table.
 */
class TableRule
 : public AsmRule
{
 public:
    /**
     * Construct a TableRule with a given symbol table.
     *
     * @param inSymTable - the symbol table to match values and names with
     *        (case sensitivity is specified by the symbol table)
     * @param inShift - the position (in terms of <<) of the least-significant
     *        bit of this field
     * @param inWidth - the number of bits in the field
     */
    TableRule(const SymTable* inSymTable, int inShift, int inWidth)
     : mSymTable(inSymTable), mShift(inShift), mWidth(inWidth)
     { }

    bool matches(GInstr instr) const;
    void claimBits(GInstr instr, AsmRule const ** bitArray) const;
    String disassemble(const AsmContext& context, GInstr instr) const;
    SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const;
    void extractTokens(SymTable* table) const;
 
 private:
    const SymTable* mSymTable;
    int mShift;
    int mWidth;
};

/**
 * A NegateRule wraps another rule in order to simulate negation.
 *
 * For example, many architectures have an "add immediate" instruction but
 * not a "subtract immediate" instruction.  To allow the pseudo-op for
 * subtract, you can declare a NegateRule around an ImmediateRule, and
 * then whenever that ImmRule is matched its result is automatically
 * negated.
 *
 * See mips/archmips.cpp for an example of this.  (Futureproofing Note:
 * The code might have moved to mips/asmformatmips.cpp).
 */
class NegateRule
 : public AsmRule
{
 public:
    /**
     * Create a NegateRule that will pass its data directly to the inner
     * rule, but negate the bits between shift and shift+width-1.
     *
     * @param innerRule - the inner rule being wrapped
     * @param shift - the amount being shifted
     * @param width - the width of the instruction
     */
    NegateRule(AsmRule* innerRule, int shift, int width)
     : mInnerRule(innerRule)
     , mShift(shift)
     , mWidth(width)
     { }
    
    /**
     * Determine if this rule matches a particular integer instruction,
     * for disassembling.
     *
     * @return true iff there is a match
     */
    virtual bool matches(GInstr instr) const
     { return mInnerRule->matches(negate(instr)); }
    /**
     * "Claim" which bits of an instruction belong to what rule.
     * In each element of bitArray will be placed a pointer to the rule
     * that is most interested in a particular bit.  This is used
     * particularly for breaking up the bits visually for user view.
     *
     * @param instr - the instruction
     * @param bitArray - an array of const AsmRule pointers to place the
     *                 claims in
     */
    virtual void claimBits(GInstr instr, AsmRule const ** bitArray) const
     { return mInnerRule->claimBits(negate(instr), bitArray); }
    /**
     * Disassemble an instruction into a string.
     *
     * @param context - the context of the expression
     * @param instr - the instruction being disassembled
     * @return the string disassembly as specified by the tree
     */
    virtual String disassemble(const AsmContext& context, GInstr instr) const
     { return mInnerRule->disassemble(context, negate(instr)); }
    /**
     * Try to assemble a sequence of tokens.  This assumes that all
     * labels have already been pulled off the beginning of the line.
     * The tokenizer will be left such that the current token is the
     * last unused token; in the case of a failure, the current token
     * should be the token at fault for failure.
     *
     * @param context - the context being assembled into
     * @param tokenizer - the tokenizer to get tokens from
     * @param pInstr - where to place the instruction
     * @return the success of the assembly
     */
    virtual SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const
     { SuccessT rv = mInnerRule->assemble(context, tokenizer, pInstr);
       *pInstr = negate(*pInstr);
       return rv; }
    /**
     * Extract all literal tokens from the assembly tree into a table.
     * This is used in order to differentiate labels at the beginning
     * of a line from instructions, etc.
     */
    virtual void extractTokens(SymTable* table) const
     { return mInnerRule->extractTokens(table); }
    
    /**
     * Perform the act of negation.
     */
    GInstr negate(GInstr instr) const
     { GInstr mask = Bits::mask<GInstr>(mWidth) << mShift;
       return (instr & ~mask) | ((((instr & mask) ^ mask) + (1 << mShift)) & mask); }
    
 private:
    AsmRule *mInnerRule;
    int mShift;
    int mWidth;
};


/**
 * An ImmRule corresponds to an immediate value.
 *
 * These are parsed like expressions.
 */
class ImmRule
 : public AsmRule
{
 public:
    enum DecodeType
    {
        Unsigned = 0x00,
        Signed = 0x01,
        PCOff = 0x02
    };
    
 public:
    /**
     * Construct an ImmRule with the specified types.
     * NOTE: Due for a redesign.
     */
    ImmRule(
        int inDestBits, Format::DisplayFormat inDisplayType,
        int inDecType, int inShift, int inWidth,
        int inOffset = 0, int inFactor = 1)
     : mDestBits(inDestBits), mDisplayType(inDisplayType)
     , mDecType(inDecType), mShift(inShift)
     , mWidth(inWidth), mOffset(inOffset), mFactor(inFactor)
     { }
    
    bool matches(GInstr instr) const;
    void claimBits(GInstr instr, AsmRule const ** bitArray) const;
    String disassemble(const AsmContext& context, GInstr instr) const;
    
    /**
     * Parse however many tokens are necessary to provide an immediate
     * value.
     *
     * This will handle +, -, *, and / expressions.  One interesting
     * feature (whether this be expected or unexpected) is that PC-offset
     * computations are done only wherever a label is found, not on the
     * overall expression.
     */
    SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const;
    
    long extend(GInstr instr) const;
    
 private:
    int mDestBits;
    Format::DisplayFormat mDisplayType;
    int mDecType;
    int mShift;
    int mWidth;
    int mOffset;
    int mFactor;
};

/**
 * A match rule is simply a rule that requires certain bits to have certain
 * values.  They correspond to no assembly tokens and are simply a disassembly
 * constraint.
 *
 * A match rule will typically say: "the instruction ANDed with 0xf007 must
 * be 0x3002" or something to that extent.
 */ 
class MatchRule
 : public AsmRule
{
 public:
    /**
     * Turns a string such as "0101 0... ..01 1..." into a mask and value.
     */
    static SuccessT parseBitSpec(const char* bitspec,
        GInstr* maskp, GInstr* valp);
    
 public:
    /**
     * Construct a MatchRule with specified values.
     * @param inMask - the value to mask the input with
     * @param inVal - the value to expect after the masking
     */
    MatchRule(GInstr inMask, GInstr inVal)
     : mMask(inMask), mVal(inVal)
     { }
    /**
     * Construct a MatchRule for a particular value at a particular position.
     * @param inVal - the value to match
     * @param inShift - the shift of the value
     * @param inWidth - how many bits to match
     */
    MatchRule(GInstr inVal, GInstr inShift, GInstr inWidth)
     : mMask(Bits::mask<GInstr>(inWidth) << inShift), mVal(inVal << inShift)
     { }
    /**
     * Construct a MatchRule from a bit specification.
     * @param bitspec - the specification such as "0000 01.. ...1 00.."
     */
    MatchRule(const char *bitspec)
     { parseBitSpec(bitspec, &mMask, &mVal); }
    
    bool matches(GInstr instr) const
     { return (instr & mMask) == mVal; }
    void claimBits(GInstr instr, AsmRule const ** bitArray) const;
    String disassemble(const AsmContext& context, GInstr instr) const
     { return ""; }
    SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const
     { *pInstr = mVal; return SuccessPass; }
    
    GInstr mask() const { return mMask; }
    GInstr val() const { return mVal; }
    
 private:
    GInstr mMask;
    GInstr mVal;
};

/**
 * A token rule corresponds to a requirement that a particular token be
 * present; it has no effect on the assembled value.  If the token is found,
 * it will be gobbled and the result will be successful.
 */
class TokRule
 : public AsmRule
{
 public:
    /**
     * Construct a token rule.
     * @param inTokText - the text that must be matched
     * @param inCaseSensitive - whether the token match is case sensitive
     */
    TokRule(const char* inTokText, bool inCaseSensitive = false)
     : mTokText(inTokText), mCaseSensitive(inCaseSensitive)
     { }
    
    bool matches(GInstr instr) const
     { return true; }
    String disassemble(const AsmContext& context, GInstr instr) const;
    SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const;
    void extractTokens(SymTable* table) const;
    
 private:
    String mTokText;
    bool mCaseSensitive;
};

/**
 * A MultiRule is the generic base class for rules with multiple elements.
 * It is not meant to be used directly, but instead to be reimplemented.
 */
class MultiRule
 : public AsmRule
{
 public:
    MultiRule();
    virtual ~MultiRule();
    
 protected:
    void addRule(AsmRule* rule)
     { mRules.push_back(rule); }
    void addRules(
        AsmRule* r1, AsmRule* r2 = 0, AsmRule* r3 = 0, AsmRule* r4 = 0,
        AsmRule* r5 = 0, AsmRule* r6 = 0, AsmRule* r7 = 0, AsmRule* r8 = 0);
    void extractTokens(SymTable* table) const;
    
 protected:
    Array<AsmRule*> mRules;
};

/**
 * A compound rule is the second most interesting rule.  It is a combination
 * of rules where each rule comes after the other in assembly language and
 * must all be matched independently for disassembly.
 *
 * For instance, a compound rule might contain a TokRule for "add", a
 * MatchRule such that the top four bits are 1011, and three registers
 * appear at different bit locations.
 *
 * The order of the rules specifies the order they appear in assembly
 * language.  For the add instruction, the TokRule for add must come first,
 * and then the three registers (separated by comments).
 *
 * The bit positions of each register, however, are specified
 * only by the bits set up by the TableRule.  Thus, the assembly and the
 * bitwise representation of a rule can be completely different.
 */
class CompoundRule
 : public MultiRule
{
 public:
    /**
     * Construct a CompoundRule with no rules.
     */
    CompoundRule() { }
    /**
     * Construct a CompoundRule with several sub-rules.
     * Null arguments are ignored.
     */
    CompoundRule(
        AsmRule* r1, AsmRule* r2 = 0, AsmRule* r3 = 0, AsmRule* r4 = 0,
        AsmRule* r5 = 0, AsmRule* r6 = 0, AsmRule* r7 = 0, AsmRule* r8 = 0);
    
    /**
     * Add a subsequent part to the CompoundRule.
     * @param rule - the rule to add
     */
    void addPart(AsmRule* rule)
     { addRule(rule); }
    
    bool matches(GInstr instr) const;
    void claimBits(GInstr instr, AsmRule const ** bitArray) const;
    String disassemble(const AsmContext& context, GInstr instr) const;
    SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const;
};

/**
 * A "multiplexor rule" means to match any one of the rules.  The order
 * of the rules is always such that the earlier ones take priority over
 * the later ones.
 *
 * For assembly, each rule is tried one by one, and if a rule fails,
 * the parsing starts over wherever the tokenizer was at the start of
 * calling this rule's assemble method.
 */
class MuxRule
 : public MultiRule
{
 public:
    /**
     * Construct a MuxRule with no rules.
     */
    MuxRule() { }
    /**
     * Construct a MuxRule with several options.  Null arguments are ignored.
     */
    MuxRule(
        AsmRule* r1, AsmRule* r2 = 0, AsmRule* r3 = 0, AsmRule* r4 = 0,
        AsmRule* r5 = 0, AsmRule* r6 = 0, AsmRule* r7 = 0, AsmRule* r8 = 0);
    
    /**
     * Add another option for the MuxRule.
     * @param rule - the rule to add an option for
     */ 
    void addOption(AsmRule* rule)
     { addRule(rule); }
    
    bool matches(GInstr instr) const;
    void claimBits(GInstr instr, AsmRule const ** bitArray) const;
    String disassemble(const AsmContext& context, GInstr instr) const;
    SuccessT assemble(const AsmContext& context,
        AsmTokenizer* tokenizer, GInstr* pInstr) const;
};

/**
 * An ExprParser is a simple recursive-descent parser to parse integer
 * expressions.
 *
 * If an ExprParser has a symbol table, look-up will be performed on
 * symbols.  Additionally, if a machine is available, register and memory
 * reads will be available.
 *
 * TODO: Make bounds-checking abstract here, rather than having it taken
 * care of by callers.
 */
class ExprParser
{
 public:
    /**
     * Construct an ExprParser with an optional symbol table and machine.
     *
     * @param inSymTable - symbol table to use for label loookup
     * @param inMachine - machine to use for register and memory reads
     */
    ExprParser(
        const SymTable* inSymTable = 0, const Machine* inMachine = 0)
     : mSymTable(inSymTable)
     , mMachine(inMachine)
     , mSymbolOffset(0)
     { }
    /**
     * Construct an ExprParser from an assembly context.
     *
     * @param inContext - the assembly context
     */
    ExprParser(const AsmRule::AsmContext& context)
     : mSymTable(&context.symTable())
     , mMachine(0)
     , mSymbolOffset(0)
     { }
    
    /**
     * Parse a general expression, reading from the tokenizer.
     *
     * Validation performed by this method on the value is extremely
     * limited.  For instance, you may want to check to make sure there are
     * no remaining tokens left in the tokenizer.  Additionally, you will
     * have to check for overflow based on your target size.
     *
     * @param tokenizer - where to get the tokens from
     * @param pVal - where to put the values
     */
    SuccessT parseExpr(AsmTokenizer* tokenizer, GVal *pVal) const;
    /**
     * Parse a single multiplicative term.  A term might be something like
     * "3*5/7", but addition is not parsed.
     * @param tokenizer - where to get the tokens from
     * @param pVal - where to put the values
     */
    SuccessT parseTerm(AsmTokenizer* tokenizer, GVal *pVal) const;
    /**
     * Parse a single factor.
     * @param tokenizer - where to get the tokens from
     * @param pVal - where to put the values
     */
    SuccessT parseFactor(AsmTokenizer* tokenizer, GVal *pVal) const;
    /**
     * Parse a single number literal.
     * @param reporter - used to report error messages
     * @param pVal - where to put the values
     */
    SuccessT parseNumber(Reporter* reporter, const char* str,
        int base, GVal *pVal) const;
    /**
     * Parse a location identifier for some location on the machine.
     *
     * This will resolve "pc" to the program counter and register names to
     * the appropriate register.  Memory addresses have three types:
     * the asterisk (*) for words, the at-sign (@) for bytes, and the
     * caret (^) for instructions.  If a simple number is given, a memory
     * word is assumed.
     *
     * @param tokenizer - where to get tokens from
     */
    LocID parseLocation(AsmTokenizer* tokenizer, bool mustParse = true) const;
    
    /**
     * Set the offset that will be subtracted whenever a symbol is read in.
     */
    void setSymbolOffset(long inSymbolOffset)
     { mSymbolOffset = inSymbolOffset; }
    
 private:
    const SymTable* mSymTable;
    const Machine* mMachine;
    long mSymbolOffset;
};

/**
 * An AsmRuleGenerator instance is a helper to create lots of similar
 * rules.
 *
 * Using bind, you will assign names to various rules.  Then, in makeRule,
 * you can refer to those textual names and combine them.
 */
class AsmRuleGenerator
{
 public:
    /**
     * Default constructor.
     */
    AsmRuleGenerator()
     : mRoot(new MuxRule())
     { }
    
    /**
     * Create a rule based on the given operands and the textual
     * specification.
     *
     * This will construct a compound rule based on the operands.
     *
     * The mask and val are the restrictions imposed by this rule, i.e.
     * that the instr & mask must equal val, creating a new MatchRule
     * as one of the elements of the compound rule.
     *
     * The operands parameter must be tokenizable by AsmTokenizer.  For
     * each token, if the token matches one of the bound tokens, it will
     * add that token's binding to the compound rule; otherwise, a TokRule
     * will be added to match that literal token.
     *
     * For instance, "lw reg9,imm5(reg5)" would add a literal "lw" token,
     * the substitution for reg9, a literal comma, the substitution for
     * imm5, an open parenthesis, the substitution for reg5, and the close
     * parenthesis.
     *
     * @param mask - the mask to match with
     * @param val - the value to expect after matching
     * @param operands - the textual operands
     * @param rule - the compound rule to work with, or 0 if one should be
     *        allocated
     */
    AsmRule* makeRule(GInstr mask, GInstr val,
        const char* operands, CompoundRule* rule = 0) const;
    /**
     * Similar to the previous makeRule except that the matching is
     * specified differently (see MatchRule constructors).
     *
     * @param bitspec - the bit specification (see MatchRule)
     * @param operands - the operand string (see the other makeRule)
     * @param rule - the rule to work with (0 to allocate one)
     */
    AsmRule* makeRule(const char *bitspec,
        const char* operands, CompoundRule* rule = 0) const;
    /**
     * Similar to the previous makeRule except that the matching is
     * specified differently (see MatchRule constructors).
     *
     * @param val - the value to expect after shifting and masking
     * @param shift - how much the instruction must be shifted right before
     *        masking
     * @param width - the number of bits being matched
     * @param operands - the operand string (see the other makeRule)
     * @param rule - the rule to work with (0 to allocate one)
     */
    AsmRule* makeRule(GInstr val, int shift, int width,
        const char* operands, CompoundRule* rule = 0) const
     { return makeRule(Bits::mask<GInstr>(width) << shift, val << shift,
         operands); }
    /**
     * Similar to the other makeRule aliases, except there is no mask to
     * match.
     *
     * @param operands - the operands to parse (see other makeRule)
     * @param rule - the rule to start with, or 0 to allocate a new one
     */
    AsmRule* makeRule(const char* operands, CompoundRule* rule = 0) const
     { return makeRule(0, 0, operands, rule); }
    
    void add(GInstr mask, GInstr val,
       const char* operands, CompoundRule* rule = 0)
     { add(makeRule(mask, val, operands, rule)); }
    void add(const char *bitspec,
       const char* operands, CompoundRule* rule = 0)
     { add(makeRule(bitspec, operands, rule)); }
    void add(GInstr val, int shift, int width,
       const char* operands, CompoundRule* rule = 0)
     { add(makeRule(val, shift, width, operands, rule)); }
    void add(const char* operands, CompoundRule* rule = 0)
     { add(makeRule(operands, rule)); }
    void add(AsmRule* rule)
     { mRoot->addOption(rule); }
       
    
    /**
     * Bind a rule to a name.
     *
     * @param name - the name to bind with
     * @param rule - the rule being bound
     */
    void bind(const char* name, AsmRule* rule);
    /**
     * Bind a rule to a combination of other rules (using makeRule).
     *
     * @param name - the name to bind
     * @param operands - the operands to parse
     */
    void bind(const char* name, const char* operands)
     { return bind(name, makeRule(operands)); }
    
    /**
     * Look up a particular binding.
     *
     * @param name - the name to look up
     */
    AsmRule* lookup(const char* name) const;
    
    /**
     * Return the root rule that has been built.
     *
     * This erases the current rule and starts fresh.
     *
     * @return a new MuxRule ready for use
     */
    MuxRule* extractRoot()
     { MuxRule* tmp = mRoot; mRoot = new MuxRule(); return tmp; }
    
 private:
    /// A mapping of the names to array index in mRules.
    SymTable mNames;
    /// An array of all the available rules.
    Array<AsmRule*> mRules;
    /// Root rule
    MuxRule* mRoot;
};

#endif
