/*
 * @description   Pr�ce s jednosm�rn�m seznamem
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

#define LIST_EOK     0
#define LIST_ERR    -1
#define LIST_EALLOC -2

typedef struct tLElem {        // struktura pro polo�ku seznamu
  struct tLElem *ptr;          // ukazatel na dal�� polo�ku
  void *data;                  // ukazatel na data
} *tLElemPtr;

typedef struct {               // struktura pro seznam:
  tLElemPtr Act;               // uk na aktivn� prvek
  tLElemPtr First;             // uk na prvn� prvek
  tLElemPtr Last;              // uk na posledn� prvek
} tList;


// Funkce pro pr�ci se seznamem:

int InitList     (tList *L);
int DisposeList  (tList *L);

int InsertFirst  (tList *L, void *data);
int InsertLast   (tList *L, void *data);
int First        (tList *L);
int Last         (tList *L);
void *CopyFirst  (tList *L);
void *CopyLast   (tList *L);
int DeleteFirst  (tList *L);

int PostDelete   (tList *L);  // neimplementov�no

int PostInsert   (tList *L, void *data);
int Succ         (tList *L);
void *Copy        (tList *L);	
int Actualize    (tList *L, void *data );
int Active       (tList *L);

tLElemPtr GetActive (tList *L);
int SetActive (tList *L, tLElemPtr uk);

#endif // LIST_H_INCLUDED
