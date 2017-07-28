package jsnobol3;

enum TokenType
{
    NULL, EOF, EOL,
    NAME, FCNNAME, PATFCNNAME, LABEL, STRING, WHITESPACE, FLAG,
    // provide markers for legal non-alpha, non-digit characters
    PLUS, // +
    MINUS, // -
    EQUAL, // =
    LPAREN, // (
    RPAREN, // )
    STAR, // *
    MULT, // * - distinguished by lexer
    COMMA, // ,
    SLASH, // /
    DIV, // / - distinguished by lexer
    DOT, // .
    DOLLAR, // $
    SQUOTE, // '
    DQUOTE, // "
    BAR, // |

    // Keywords
    SUCCESS, // s
    FAIL, // f
    DEFINE, // DEFINE
    RETURN, // return
    FRETURN, // freturn
    END; // end

    static final String DELIMITERS = "/+-=()*,/$|";
    
    static TokenType delimiterFor(StringBuilder text)
    {
	switch (text.charAt(0)) {
	case '+': return TokenType.PLUS;
	case '-': return TokenType.MINUS;
	case '=': return TokenType.EQUAL;
	case '(': return TokenType.LPAREN;
	case ')': return TokenType.RPAREN;
	case '*': return TokenType.STAR;
	case ',': return TokenType.COMMA;
	case '/': return TokenType.SLASH;
	case '$': return TokenType.DOLLAR;
	case '.': return TokenType.DOT; // technically never a delimiter
	case '\'': return TokenType.SQUOTE; // technically never a delimiter
	case '"': return TokenType.DQUOTE; // technically never a delimiter
	case '|': return TokenType.BAR;
	case '\n': return TokenType.EOL;
	}
	return TokenType.NULL;
    }

}
