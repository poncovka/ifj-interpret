/*---------------------------------hlavicka lexikalniho analyzatoru-------------------------------
 * @author: Tomas Trkal, xtrkal00
 * @date: 	11.11.2011
 */


/*vycet typu lexem*/
enum ELexem { 	
	KW_DO,		KW_ELSE,		KW_END,		KW_FALSE,		KW_FUNCTION,	
	KW_IF,		KW_LOCAL,		KW_NIL,		KW_READ,		KW_RETURN,
	KW_THEN,	KW_TRUE,		KW_WHILE,	KW_WRITE,   //14

	RESERVED_WORD,				// rezervovane slovo
	END_OF_FILE,					// EOF

	L_ID,									// identifikator
	L_NUMBER,							// cislo
	L_LEFT_BRACKET,				// ( 
	L_RIGHT_BRACKET,			// ) 
	L_SEMICOLON,					// ; 
	L_POWER,							// ^
	L_MULTIPLICATION,			// * 
 	L_DIVISION,						// / 
	L_ADDITION,						// + 
	L_SUBTRACTION,				// -
  L_COMMA,							// , 
	L_CONCATENATION,			// .. 
	L_SMALLER,						// <
	L_SMALLER_EQUAL,			// <=
	L_BIGGER,							// >
	L_BIGGER_EQUAL,				// >=
	L_UNEQUAL,						// ~=
	L_EQUAL,							// ==
	L_ASSIGN,							// =
	L_STRING							// ".."
};

/*vycet stavu*/
enum EState {
	S_DEFAULT, 	
	S_POINT, 
	S_BIGGER,		
	S_SMALLER,	
	S_UNEQUAL,
	S_EQUAL,		
	S_DASH,		
	S_COMMENT,	
	S_COMMENT_ROW,
	S_COMMENT_BLOCK,
	S_COMMENT_BEGIN,
	S_COMMENT_END,
	S_STRING,
	S_ESCAPE,
	S_ID,
	S_NUMBER,
	S_EXPONENT,
	S_EXPONENT_END,
	S_DECIMAL_POINT,
	S_DECIMAL_NUMBER
};

/*false = 0, true = 1*/
enum {FALSE, TRUE};

/*definice chyb*/
#define LEX_ERROR -1
#define LEX_ERR_MALLOC -10

/*hlavicky funkci*/
void setSourceFile(FILE *f);
int getNextToken(void);

/*tabulka s rezervovanymi a klicovymi slovy => to cely nacpu do scanner.c
const char *reservedWords[] = {"and", "break", "elseif", "for", "in", "not", "or", "repeat", "until"};
const char *keyWords[] = {"do",		"else",		"end",		"false",	"function",
													"if",		"local",	"nil",		"read",		"return",
													"then",	"true",		"while",	"write"};*/	
