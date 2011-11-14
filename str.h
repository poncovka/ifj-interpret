#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED
//hlavickovy soubor pro praci s nekonecne dlouhymi retezci

#include <string.h>
#include <malloc.h>

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla

#define STR_ERROR   1
#define STR_SUCCESS 0

typedef struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string;


int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);

char *strCopyChar(string *s);
string strCreateString (char *str);

#endif //STR_H_INCLUDED
