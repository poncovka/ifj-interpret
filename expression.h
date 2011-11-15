/*
 * @description   Syntaktick� anal�za v�raz� (zdola nahoru)
 * @author        Vendula Poncov� - xponco00
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
#define ENDEXPR   END_OF_FILE + 1    // ukon�ova� �et�zce a dno z�sobn�ku
#define MAXTAB    ENDEXPR + 1        // rozm�r preceden�n� tabulky
#define EXPRESSION  -1               // nen� token
#define NOINSTR   I_SORT + 1


#define isId(t)       (t == L_ID)
#define isConst(t)    (t == L_NUMBER || t == L_STRING || t == KW_TRUE || t == KW_FALSE || t == KW_NIL)
#define isBracket(t)  (t == L_LEFT_BRACKET || t == L_RIGHT_BRACKET)
#define isOperator(t) (t >= L_POWER && t <= L_EQUAL)


// pomocn� struktura pro data na z�sobn�ku:

typedef struct {
  int token;            // token
  TVar *var;            // adresa prom�nn�
} TStackData;


// funkce pro vyhodnocov�n� v�raz�:

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
