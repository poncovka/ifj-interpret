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
#define NONTOKEN  -1                 // nen� token


// pomocn� struktura pro data na z�sobn�ku:

typedef struct {
  char c;              	// znak termin�lu, nontermin�lu, operace, $
  int token;            // token
  TVar *var;            // adresa prom�nn�
} TStackData;


// funkce pro vyhodnocov�n� v�raz�:

int parseExpression();
int  stackTopTerminal(TStack *S);

TStackData *createStackData(char c, int token, TVar *var, int *err);
TVar *createTmpVar(TList *L, int *err);

void listDataDelete(TList *L);
void stackDataDelete(TStack *S);

void tiskniPrecTab();

#endif // EXPRESSION_H_INCLUDED
