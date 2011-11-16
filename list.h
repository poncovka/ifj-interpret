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

#define LIST_EOK     0
#define LIST_ERR    -1
#define LIST_EALLOC -5

typedef struct tLItem {      // struktura pro polo�ku seznamu
  struct tLItem *next;       // ukazatel na dal�� polo�ku
  void *data;                // ukazatel na data
} TLItem;

typedef struct {             // struktura pro seznam:
  TLItem *Act;               // uk na aktivn� prvek
  TLItem *First;             // uk na prvn� prvek
  TLItem *Last;              // uk na posledn� prvek
} TList;


// Funkce pro pr�ci se seznamem:

int listInit         (TList *L);
int listDispose      (TList *L);

int listInsertFirst  (TList *L, void *data);
int listInsertLast   (TList *L, void *data);
int listFirst        (TList *L);
int listLast         (TList *L);
void *listCopyFirst  (TList *L);
void *listCopyLast   (TList *L);
int listDeleteFirst  (TList *L);

int listPostDelete   (TList *L);  // neimplementov�no

int listPostInsert   (TList *L, void *data);
int listSucc         (TList *L);
void *listCopy       (TList *L);	
int listActualize    (TList *L, void *data );
int listActive       (TList *L);

TLItem *listGetActive (TList *L);
int listSetActive (TList *L, TLItem *uk);

#endif // LIST_H_INCLUDED
