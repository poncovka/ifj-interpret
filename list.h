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

// struktura pro polo�ku seznamu: tElem
// datov� typ tElem a tElemPtr je definov�n ve stack.h

typedef struct {                // struktura pro seznam:
  tElemPtr Act;                 // uk na aktivn� prvek
  tElemPtr First;               // uk na prvn� prvek
  tElemPtr Last;               // uk na posledn� prvek
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

int PostDelete   (tList *L);              // neimplementov�no
int PostInsert   (tList *L, void *data);
int Succ         (tList *L);
void *Copy        (tList *L);	
int Actualize    (tList *L, void *data );
int Active       (tList *L);

tElemPtr GetActive (tList *L);
int SetActive (tList *L, tElemPtr uk);

#endif // LIST_H_INCLUDED
