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


typedef struct tElem {   // struktura pro polo¾ku v zásobníku
  struct tElem *ptr;     // ukazatel na dal¹í polo¾ku
  int data;              // data
} *tElemPtr;

typedef struct {         // struktura pro zásobník
  tElemPtr top;          // ukazatel na vrchol zásobníku
} tStack;


// Funkce pro práci se zásobníkem:

void stackInit (tStack *s);
void stackDelete (tStack *s);
void stackTop (tStack *s, int *uk);
void stackPop (tStack *s);
void stackPush (tStack *s, int data );

void stackTopPop (tStack *s, int *uk);
int stackEmpty (tStack *s);


#endif // STACK_H_INCLUDED
