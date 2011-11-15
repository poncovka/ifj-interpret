#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "binaryTree.h"
#include "str.h"
#include "list.h"

#define VAR_ALLOC_SIZE 8

typedef struct{
   TBTree variables;       // tabulka promenych
   TList  constants;       // seznam konstant + pomocnych promenyh
   TList  tmpVar;          // pomocne promene
   TList  instructions;    // seznam instrukci
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
   VT_TMP_VAR,
}EVarType;

typedef union{
   int     b;
   double  n;
   string  s;
}UVarValue;

typedef struct{
   EVarDataType type;
   UVarValue    value;
}TVarData;

typedef struct{
   char      *name;
   EVarType   varType;  // CONST nebo VAR
   TVarData  *var;   // pole! bude se alokovat po 8  var[f->cnt]->type =
   int        alloc; // kolik je alokovano
}TVar;

// Instrukce
typedef enum{
   I_LAB,      // --- --- ---
   I_RETURN,   // --- --- ---

   I_POP,      // --- src ---       src je TVar ale na stacku je TVarData
   I_PUSH,     // dst --- ---       dst je TVar ale na stack se vlozi odpovidajici TVarData
   I_STACK_E,

   I_MOV,      // dst --- ---       nastavi odpovidajici TVar -> TVarData[fce->cnt] -> type = NIL
               // dst src ---       odpovidajici dest TVar nastavi podle odpovidajiciho src TVar,
   I_SET,      // dst --- ---       nastavi promenou na nil jako prvni vola varRealloc!!!
               // dst src ---       nastavi na podle src

   I_ADD,      // dst src src       vsechno TVar
   I_SUB,      // dst src src
   I_MUL,      // dst src src
   I_DIV,      // dst src src
   I_POW,      // dst src src
   I_CON,      // dst src src

   I_CMP_L,    // dst src src
   I_CMP_LE,   // dst src src
   I_CMP_G,    // dst src src
   I_CMP_GE,   // dst src src
   I_CMP_E,    // dst src src
   I_CMP_NE,   // dst src src

   I_JMP,      // lab --- ---       lab je nasvesti TItem nebp TLElemPtr (podle impelentace listu)
   I_JMP_Z,    // lab src ---       src je TVar
   I_JMP_NZ,   // lab src ---

   I_WRITE,    // --- src ---       src TVar
   I_READ,     // dst prm ---       dst TVar, prm je parametr read
   I_CALL,     // fce --- ---       fce je TFunction

   I_TYPE,     // --- --- ---
   I_SUBSTR,   // --- --- ---
   I_FIND,     // --- --- ---
   I_SORT,     // --- --- ---
}EInstrType;

typedef struct{
   EInstrType type;
   void *dest;
   void *src1;   // ((TVar *)L->act->data)->var[f->cnt]->type --- VAR CONST TMP_VAR
   void *src2;
}TInstr;

void tableInit(TTable*);

/*
 * naposled vlozena promena do tabulky symbolu ve funkci
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
int functionInsertVar(TFunction*, string);

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
 * @params  po kolikate je volana funkce!
 * @return  1 vse OK, -5 neslo alokovat
 */
int varRealloc(TVar*, int);

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
