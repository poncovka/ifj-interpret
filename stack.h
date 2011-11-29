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

typedef struct tSItem {   // struktura pro polo�ku v z�sobn�ku
   struct tSItem *next;     // ukazatel na dal�� polo�ku
   void *data;             // ukazatel na data
} TSItem;

typedef struct {          // struktura pro z�sobn�k
   TSItem *top;          // ukazatel na vrchol z�sobn�ku
} TStack;


// Funkce pro pr�ci se z�sobn�kem:

int stackInit     (TStack *s);
int stackEmpty    (TStack *s);
void *stackTop    (TStack *s);
int stackPop      (TStack *s);
int stackPush     (TStack *s, void *data);

void *stackTopPop (TStack *s);
int stackDelete   (TStack *s);

#endif // STACK_H_INCLUDED
