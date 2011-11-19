/*
 * @description   Knihovna pro práci s datovým typem string
 * @author        pøevzato z jednoduchy_interpret/str.h
                  Vendula Poncová - xponco00
 *                Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <string.h>
#include <malloc.h>
#include <stdio.h>

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla

#define STR_ERROR   1
#define STR_SUCCESS 0

// struktura pro datový typ STRING:

typedef struct
{
  char* str;		  // misto pro dany retezec ukonceny znakem '\0'
  int length;		  // skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string;


// funkce pro práci se stringem:

int  strInit       (string *s);
int  strInitLen    (string *s, int len);
void strFree       (string *s);
void strClear      (string *s);
int  strIsNull     (string *s);

int  strAddChar    (string *s1, char c);
char *strCopyChar  (string *s);
int  strCopyString (string *s1, string *s2);
int  strCmpString  (string *s1, string *s2);

string strConcatenation (string *s1, string *s2);

string strCreateString      (string *str);
string strCreateConstString (char *str);

string strReadNChar(FILE *f, int n);
string strReadLine (FILE *f);
string strReadAll  (FILE *f);

/* BUDE SMAZANO
int strCmpConstStr(string *s1, char *s2);
char *strGetStr(string *s);
int strGetLength(string *s);
*/

#endif //STR_H_INCLUDED
