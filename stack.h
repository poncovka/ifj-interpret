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


typedef struct tElem {   // struktura pro polo�ku v z�sobn�ku
  struct tElem *ptr;     // ukazatel na dal�� polo�ku
  int data;              // data
} *tElemPtr;

typedef struct {         // struktura pro z�sobn�k
  tElemPtr top;          // ukazatel na vrchol z�sobn�ku
} tStack;


// Funkce pro pr�ci se z�sobn�kem:

void stackInit (tStack *s);
void stackDelete (tStack *s);
void stackTop (tStack *s, int *uk);
void stackPop (tStack *s);
void stackPush (tStack *s, int data );

void stackTopPop (tStack *s, int *uk);
int stackEmpty (tStack *s);


#endif // STACK_H_INCLUDED
