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
#define NONTOKEN  -1                 // není token


// pomocná struktura pro data na zásobníku:

typedef struct {
  char c;              	// znak terminálu, nonterminálu, operace, $
  int token;            // token
  TVar *var;            // adresa promìnné
} TStackData;


// funkce pro vyhodnocování výrazù:

int parseExpression();
int  stackTopTerminal(TStack *S);

TStackData *createStackData(char c, int token, TVar *var, int *err);
TVar *createTmpVar(TList *L, int *err);

void listDataDelete(TList *L);
void stackDataDelete(TStack *S);

void tiskniPrecTab();

#endif // EXPRESSION_H_INCLUDED
