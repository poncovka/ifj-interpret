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
#define NOINSTR   I_SORT + 1


#define isId(t)       (t == L_ID)
#define isConst(t)    (t == L_NUMBER || t == L_STRING || t == KW_TRUE || t == KW_FALSE || t == KW_NIL)
#define isBracket(t)  (t == L_LEFT_BRACKET || t == L_RIGHT_BRACKET)
#define isOperator(t) (t >= L_POWER && t <= L_EQUAL)


// pomocná struktura pro data na zásobníku:

typedef struct {
  int token;            // token
  TVar *var;            // adresa promìnné
} TStackData;


// funkce pro vyhodnocování výrazù:

int parseExpression();

int shift           (TStack *S, int token, TVar *pom);
int getTopTerminal  (TStack *S);
int findRule        (TStack *S, TInstr *instr);
int generateInstr   (TList *LInstr, TInstr *instr, TList *LTmpVars);

TStackData *createStackData(int token, TVar *var, int *err);
TVar       *createTmpVar(TList *L, int *err);

void listDataDelete (TList *L);
void stackDataDelete(TStack *S);

void tiskniPrecTab();

#endif // EXPRESSION_H_INCLUDED
