/*
 * @description   Pr�ce se dynamick�m z�sobn�kem
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define STACK_EOK     0
#define STACK_ERR    -1
#define STACK_EEMPTY -2
#define STACK_EALLOC -5

typedef struct tSElem {   // struktura pro polo�ku v z�sobn�ku
  struct tSElem *ptr;     // ukazatel na dal�� polo�ku
  void *data;             // ukazatel na data
} *tSElemPtr;

typedef struct {          // struktura pro z�sobn�k
  tSElemPtr top;          // ukazatel na vrchol z�sobn�ku
} tStack;


// Funkce pro pr�ci se z�sobn�kem:

int stackInit     (tStack *s);
int stackEmpty    (tStack *s);
void *stackTop    (tStack *s);
int stackPop      (tStack *s);
int stackPush     (tStack *s, void *data);

void *stackTopPop (tStack *s);
int stackDelete   (tStack *s);

#endif // STACK_H_INCLUDED
