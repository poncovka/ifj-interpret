/*
 * @description   Práce s jednosmìrným seznamem
 * @author        Vendula Poncová - xponco00
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

typedef struct tLItem {      // struktura pro polo¾ku seznamu
  struct tLItem *next;       // ukazatel na dal¹í polo¾ku
  void *data;                // ukazatel na data
} TLItem;

typedef struct {             // struktura pro seznam:
  TLItem *Act;               // uk na aktivní prvek
  TLItem *First;             // uk na první prvek
  TLItem *Last;              // uk na poslední prvek
} TList;


// Funkce pro práci se seznamem:

int listInit         (TList *L);
int listDispose      (TList *L);

int listInsertFirst  (TList *L, void *data);
int listInsertLast   (TList *L, void *data);
int listFirst        (TList *L);
int listLast         (TList *L);
void *listCopyFirst  (TList *L);
void *listCopyLast   (TList *L);
int listDeleteFirst  (TList *L);

int listPostDelete   (TList *L);  // neimplementováno

int listPostInsert   (TList *L, void *data);
int listSucc         (TList *L);
void *listCopy       (TList *L);	
int listActualize    (TList *L, void *data );
int listActive       (TList *L);

TLItem *listGetActive (TList *L);
int listSetActive (TList *L, TLItem *uk);

#endif // LIST_H_INCLUDED
