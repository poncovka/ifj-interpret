//=================================================================================================>
//-----------------------------LEXIKALNI ANALYZATOR - HLAVICKOVY SOUBOR---------------------------->
//=================================================================================================>
/* @author: Tomas Trkal, xtrkal00@stud.fit.vutbr.cz
 * @date: 	11.11.2011
 */

#ifndef SCANNER_H_INCLUDED                                                                                                           │
#define SCANNER_H_INCLUDED
#endif

/*vycet typu lexem*/
enum ELexem { 	
	KW_END, 			KW_LOCAL,		KW_WRITE,
	KW_RETURN,		KW_IF,			KW_THEN,
	KW_FUNCTION,	KW_READ,		KW_DO,	
	KW_ELSE,			KW_WHILE,		KW_NIL,		
	KW_FALSE,			KW_TRUE, 		//14

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
	S_CONCATENATION, 
	S_BIGGER,		
	S_SMALLER,	
	S_UNEQUAL,
	S_EQUAL,		
	S_SUBTRACTION,		
	S_COMMENT,	
	S_COMMENT_ROW,
	S_COMMENT_BLOCK,
	S_COMMENT_BEGIN,
	S_COMMENT_END,
	S_STRING,
	S_ESCAPE,
	S_ESCAPE_NUMERAL,
	S_ESCAPE_DDD,
	S_ID,
	S_NUMBER,
	S_EXPONENT,
	S_EXPONENT_END,
	S_DECIMAL_POINT,
	S_DECIMAL_NUMBER
};

#define LEX_ERROR -1
#define ERR_MALLOC -5

/*deklarace promennych*/
extern int countOfRows;

/*deklarace funkci*/
void setSourceFile(FILE *f);
int getNextToken(string *attr);
