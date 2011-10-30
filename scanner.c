/*-------------------------------------lexikalni analyzator---------------------------------------
 * @author: Tomas Trkal, xtrkal00
 * @date: 	11.11.2011
 */


#include <stdio.h>
#include <ctype.h>
#include "scanner.h"
#include "strings.h"

/*promenna pro ulozeni vstupniho souboru*/
FILE *source;
void setSourceFile(FILE *f) {
	source = f;
}

/*hlavni funkce lexikalniho analyzatoru*/
int getNextToken(void) {

	/*nasteveni vychoziho stavu*/
	int state = S_DEFAULT; 
	int c;

	/*konecny automat*/
	while (TRUE) { 
		c = getc(source);
		switch (state) {
		
			/*vychozi stav automatu*/ 
			case S_DEFAULT:
				if (isspace(c)) state = S_DEFAULT; 	//ignorace bilych mist
				else if (c == EOF) return END_OF_FILE; 
				else if (c == '(') return L_LEFT_BRACKET;
				else if (c == ')') return L_RIGHT_BRACKET;
				else if (c == ';') return L_SEMICOLON;
				else if (c == '^') return L_POWER;
				else if (c == '*') return L_MULTIPLICATION;
				else if (c == '/') return	L_DIVISION;
				else if (c == '+') return L_ADDITION;
				else if (c == ',') return L_COMMA;
				else if (c == '.') state = S_POINT; 	// .. 
				else if (c == '<') state = S_SMALLER;	// < || <=
				else if (c == '>') state = S_BIGGER;	// > || >=				
				else if (c == '~') state = S_UNEQUAL;	// ~=
				else if (c == '=') state = S_EQUAL;		// = || == 
				else if (c == '-') state = S_DASH;		// - || -- || --[[
				else if (c == '"') state = S_STRING; 	//retezec
				else if (isalpha(c)) { 								//ID || klicove slovo 		
					printf("%c",c); //predelat
					state = S_ID;
				}
				else if (isdigit(c)) { // ID
					printf("%c",c); //predelat
					state = S_NUMBER;	
				}
				else return LEX_ERROR; //jiny znak
			break;			

			/*S_POINT*/
			case S_POINT:
				if (c == '.') return L_CONCATENATION;
				else return LEX_ERROR;
			break;

			/*S_SMALLER*/
			case S_SMALLER: 
				if (c == '=') return L_SMALLER_EQUAL;
				else {
					ungetc(c,source);
					return L_SMALLER;
				}
			break;

			/*S_BIGGER*/
			case S_BIGGER:
				if (c == '=') return L_BIGGER_EQUAL;
				else {
					ungetc(c,source);
					return L_BIGGER;
				}
			break;

			/*S_UNEQUAL*/
			case S_UNEQUAL:
				if (c == '=') return L_UNEQUAL;
				else return LEX_ERROR;
			break;

			/*S_EQUAL*/
			case S_EQUAL:
				if (c == '=') return L_EQUAL;
				else {
					ungetc(c,source);
					return L_ASSIGN;
				}
			break;

			/*S_DASH*/
			case S_DASH:
				if (c == '-') state = S_COMMENT;
				else {
					ungetc(c,source);
					return L_SUBTRACTION;
				}
			break;
			
			/*stavy pro retezce*/
			/*S_STRING*/
			case S_STRING:
				if (c == EOF) return LEX_ERROR;
				else if (c == '"') {
					printf("\n"); //predelat
					return L_STRING;
				}
				else if (c == '\\') {
					printf("%c",c); //predelat
					state = S_ESCAPE;
				}
				else {
				  printf("%c",c);	//predelat
				}
			break;

			/*S_ESCAPE*/
			case S_ESCAPE:
				if (c == EOF) return LEX_ERROR;
				printf("%c",c); //predelat
				state = S_STRING;
			break;

			/*stavy pro cisla*/
			/*S_NUMBER*/
			case S_NUMBER:
			  if (isdigit(c)) {
				  printf("%c",c); //predelat
				}
				else if (c == '.') {
					printf("%c",c);	//predelat
					state = S_DECIMAL_POINT;
				}
				else if ((c == 'e') || (c == 'E')) {
				  printf("%c",c); //predelat
					state = S_EXPONENT;
				}			
		    else {
					printf("\n"); //predelat
					ungetc(c,source);
					return L_NUMBER;
				}	
			break;

			/*S_DECIMAL_POINT*/
			case S_DECIMAL_POINT:
				if (isdigit(c)) {
				  printf("%c",c); //predelat
					state = S_DECIMAL_NUMBER;
				}
				else return LEX_ERROR; 
			break;

			/*S_DECIMAL_NUMBER*/
			case S_DECIMAL_NUMBER:
				if (isdigit(c)) {
					printf("%c",c); //predelat
				}
				else if ((c == 'e') || (c == 'E')) {
				  printf("%c",c); //predelat
					state = S_EXPONENT;
				}
				else {
					printf("\n"); //predelat
					ungetc(c,source);
					return L_NUMBER;
				}
			break;

			/*S_EXPONENT*/
			case S_EXPONENT:
			  if (isdigit(c) || (c == '+') || (c == '-')) {
					printf("%c",c); //predelat
					state = S_EXPONENT_END;
				}
				else return LEX_ERROR;
			break;

			/*S_EXPONENT_END*/
			case S_EXPONENT_END: 
				if (isdigit(c)) {
					printf("%c",c); //predelat
				}
				else {
			    printf("\n"); //predelat
					ungetc(c,source);
					return L_NUMBER;
				}
			break;
			
			/*stavy pro identifikatory*/
			/*S_ID*/
			case S_ID:
				if (isdigit(c) || isalpha(c) || (c == '_')) {
					printf("%c",c); //predelat
				}
				else {
					printf("\n"); //predelat
					ungetc(c,source);
					// dodelat rozhodovani mezi KEY WORDs a IDs
					return L_ID; 
				}
			break;

			/*stavy pro komentare*/
			/*S_COMMENT*/
			case S_COMMENT:
				if (c == EOF) return END_OF_FILE;
				else if (c == '\n') state = S_DEFAULT;
				else if (c == '[') state = S_COMMENT_BLOCK;
			  else state = S_COMMENT_ROW;	
			break;

			/*S_COMMENT_BLOCK*/
			case S_COMMENT_BLOCK:
				if (c == EOF) return END_OF_FILE;
				else if (c == '[') state = S_COMMENT_BEGIN;
				else state = S_COMMENT_ROW;
			break;

			/*S_COMMENT_BEGIN*/
			case S_COMMENT_BEGIN:
				if (c == EOF) return LEX_ERROR;
				if (c == ']') state = S_COMMENT_END;
			break;

			/*S_COMMENT_END*/
			case S_COMMENT_END: 
				if (c == EOF) return LEX_ERROR;
				else if (c == ']') state = S_DEFAULT;
				else state = S_COMMENT_BEGIN; 
			break;

			/*S_COMMENT_ROW*/
			case S_COMMENT_ROW:
				if (c == EOF) return END_OF_FILE;
				if (c == '\n') state = S_DEFAULT;
			break;

		} /*konec switch*/
	} /*konec while*/
} /*konec getNextToken*/
