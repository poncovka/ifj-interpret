/*
 * @description   Syntaktická analýza výrazù (zdola nahoru)
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "list.h"
#include "table.h"
#include "scanner.h"
#include "parser.h"

#define EOK       0
#define ENDEXPR   END_OF_FILE + 1    // ukonèovaè øetìzce a dno zásobníku
#define MAXTAB    ENDEXPR + 1        // rozmìr precedenèní tabulky
#define EXPRESSION  -1               // není token
#define NOINSTR   100                // negenerovat instrukci
#define OFFSET    I_ADD - L_ADDITION // posun pro generování instrukcí

#define isId(t)       (t == L_ID)
#define isBracket(t)  (t == L_LEFT_BRACKET || t == L_RIGHT_BRACKET)
#define isConst(t)    (t >= KW_TRUE        && t <= L_STRING)
#define isOperator(t) (t >= L_ADDITION     && t <= L_UNEQUAL)

#define isMathOperation(i) (i >= I_ADD && i <= I_CMP_NE)
#define DATTYPE   4                  // pocet datovych typu

// tabulka pro kontrolu semantiky
extern const int semTable[][DATTYPE];

// pomocná struktura pro data na zásobníku:

typedef struct {
  int token;            // token
  TVar *var;            // adresa promìnné
} TStackData;


// funkce pro vyhodnocování výrazù:

int parseExpression(TTable *t, TVar **ptrResult);

int shift           (TStack *S, int token, TVar *pom);
int getTopTerminal  (TStack *S);

int findRule        (TStack *S, TInstr *instr);
int checkRule       (TInstr *instr);
int checkSemErr     (TInstr *instr, TVar *var);

int insertInstruction(TInstr *instr, TTable *table);
int returnResult    (TStack *S, TVar **ptrResult);

TStackData *createStackData(int token, TVar *var, int *err);
TVar       *createTmpVar(TList *L, int *err);

void listDataDelete (TList *L);
void stackDataDelete(TStack *S);

// pomocné výpisy:

void tiskniStack (TStack *s);
void tiskniList (TList *L);
void tiskniInst();
void tiskniPrecTab();

#endif // EXPRESSION_H_INCLUDED
