#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include "binaryTree.h"
#include "str.h"

typedef struct{
   TBTree variables;      // tabulka promenych
   // seznam constants;
   // seznam instrukci
   char   *name;           // jmeno fce(identifikator)void tableInit(TTable*);
   int    cnt;             // pocet spusteni funkce
}TFunction;

typedef struct{
   TBTree functions;
   TFunction *lastAddedFunc;
}TTable;

typedef enum{
   NIL,
   BOOL,
   NUMBER,
   STRING,
}EVarDataType;

typedef enum{
   VT_VAR,
   VT_CONST,
}EVarType;

typedef union{
   int     b;
   double  n;
   char*   s;
}UVarValue;

typedef struct{
   EVarDataType type;
   UVarValue    value;
}TVarData;

typedef struct{
   char      *name;
   EVarType   type;
   TVarData  *var;   // budem alokovat jenom jednou a ne 2x :) a pujdem po 8
   int        alloc;
}TVar;


void tableInit(TTable*);

/*
 * NENI IMPLEMENTOVANO !!!
 * naposled vllzena promena do tabulky symbolu ve funkci
 * @param   funkce
 * @return  data promene(struct TVar)
 */
TVar *getLastAddedVar(TFunction*);

/*
 * vlozi novou funci do tabulky funkci
 * @param   tabulka funkci
 * @param   klic
 */
int tableInsertFunction (TTable*, string);

/*
 * vlozi novou promenou do tabulky promenych(fce->variables)
 * @param   funkce
 * @param   klic
 */
int fuctionInsertVar(TFunction*, string);

/*
 * NENI IMPLEMENTOVANO !!!
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
TFunction *tableSearchFunction(TTable*, string);

/*
 * vyhleda promenou z tabulky promenych(fce->variables)
 * @param   funkce
 * @param   klic
 */
TVar *functionSearchVar  (TFunction*, string);

/*
 * vycisti celou tabulku funckci se vsim vsudy
 * @param   tabulka funkci
 */
void  tableClear(TTable*);

/*
 * reallocuje *var ve strukture TVar
 * @param   promena(symbol)
 * @return  1 vse OK, 0 neslo alokovat
 */
int varRealloc(TVar*);

//----------------
/*
 * FUNKCE PRO DEBUG
 */

/*
 * tiskne tabulku :)
 */
void tablePrintOrder(TTable);
void printTreeNodeOrder(TBTree*);
void printNode(TNode n, EBTreeDataType t, char *delim);

#endif // TABLE_H_INCLUDED
