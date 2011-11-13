/*
 * @description   Práce se dynamickým zásobníkem
 * @author        Vendula Poncová - xponco00
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

typedef struct tSElem {   // struktura pro polo¾ku v zásobníku
  struct tSElem *ptr;     // ukazatel na dal¹í polo¾ku
  void *data;             // ukazatel na data
} *tSElemPtr;

typedef struct {          // struktura pro zásobník
  tSElemPtr top;          // ukazatel na vrchol zásobníku
} tStack;


// Funkce pro práci se zásobníkem:

int stackInit     (tStack *s);
int stackEmpty    (tStack *s);
void *stackTop    (tStack *s);
int stackPop      (tStack *s);
int stackPush     (tStack *s, void *data);

void *stackTopPop (tStack *s);
int stackDelete   (tStack *s);

#endif // STACK_H_INCLUDED
