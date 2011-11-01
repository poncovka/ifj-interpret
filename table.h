#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include "binaryTree.h"

typedef struct{
   TBTree *variables;      // tabulka promenych
   TBTree *constants;      // tabulka konstant -- predelat na seznam!!!
   // seznam instrukci
   char   *name;           // jmeno fce(identifikator)
   int    cnt;             // pocet spusteni funkce
}TFunction;

typedef enum{
   T_CONST_NUM,
   T_CONST_STR,
   T_NIL,
   T_BOOL,
   T_NUMBER,
   T_STRING,
}EVarType;

typedef union{
   int     b;
   double  n;
   char*   s;
}UVarValue;

typedef struct{
   EVarType  type;
   UVarValue value;
}TVarData;

typedef struct{
   char      *name;
   TVarData  *var[8];   // budem alokovat jenom jednou a ne 2x :) a pujdem po 8
   int        alloc;
}TVar;

/*
 * vlozi novou funci do tabulky funkci
 * @param   tabulka funkci
 * @param   klic
 */
int tableInsertFunction (TBTree, char*);

/*
 * vlozi novou promenou do tabulky promenych(fce->variables)
 * @param   funkce
 * @param   klic
 */
int fuctionInsertVar(TFunction, char*);

/*
 * vlozi novou konstantu
 * @param   funkce (fce->constants)
 * @param   klic
 */
int functionInsertConstatnt(/*TList*/);

/*
 * vyhleda funci v tabulce funkci
 * @param   tabulka funkci
 * @param   klic
 */
TFunction tableSearchFunction(TBTree, char*);

/*
 * vyhleda promenou z tabulky promenych(fce->variables)
 * @param   funkce
 * @param   klic
 */
TVar functionSearchVar  (TFunction, char*);

/*
 * vycisti celou tabulku funckci
 */
void  tableClear(TBtree);

#endif // TABLE_H_INCLUDED
