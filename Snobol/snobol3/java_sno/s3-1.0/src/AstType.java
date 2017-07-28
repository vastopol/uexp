package jsnobol3;

enum AstType
{
    NONE,
    LINENO,
    NAME,
    LABEL,
    STRING,
    ADD,
    SUBTRACT,
    NEGATE,
    MULT,
    DIV,
    CONCAT,

    PROGRAM,
    STATEMENT,
    PATTERN,
    REFERENCE,
    FCNCALL,
    ARGLIST,
    DEFINITION,
    NAMELIST,
    BRANCH,
    ARB,
    LEN,
    BALANCE,
    PATFCN;
}

