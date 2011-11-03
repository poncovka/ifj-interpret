/*
 * @description   Práce se dynamickým zásobníkem
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "stack.h"

// co s chybama?
#define error() printf("Chyba!\n")

/*
 * Inicializuje se zásobník. Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na zásobník
 * @return  void
 */
void stackInit (tStack *s) {
  if (s != NULL) {
    s->top = NULL;
  }
  else error();

}

/*
 * Vrátí nulu, pokud je zásobník prázdný, jinak nenulovou hodnotu.
 * @param   ukazatel na zásobník
 * @return  true/false
 */
int stackEmpty (tStack *s) {

  return (s->top == NULL);
}

/*
 * Vrátí data z polo¾ky na vrcholu zásobníku.
 * Pokud je zásobník prázdný, vyvolá se chyba.
 * @param   ukazatel na zásobník
 * @param   ukazatel, kam se data ulo¾í
 * @return  void
 */
void stackTop (tStack *s, int *uk) {

  if (!stackEmpty(s)) {
    *uk = s->top->data;
  }
  else error();

}

/*
 * Vyjme polo¾ku na vrcholu zásobníku.
 * Pokud je zásobník prázdný, vyvolá se chyba.
 * @param   ukazatel na zásobník
 * @return  void
 */
void stackPop (tStack *s) {

  if (!stackEmpty(s)) {
    tElemPtr pom = s->top;
    s->top = s->top->ptr;
    free(pom);
  }
  else error();

}

/*
 * Vlo¾í na vrchol zásobníku novou polo¾ku s daty.
 * Pokud je nedostatek pìmti, vyvolá se chyba.
 * @param   ukazatel na zásobník
 * @param   data
 * @return  void
 */
void stackPush (tStack *s, int data ) {

  tElemPtr elem;

  if ( (elem = (tElemPtr)malloc(sizeof(struct tElem))) != NULL ) {

    elem->data = data;
    elem->ptr = s->top;
    s->top = elem;
  }
  else error();

}

/*
 * Vyjme ze zásobníku v¹echny polo¾ky.
 * Voláním tento funkce se zásobník stane prázdným.
 * @param   ukazatel na zásobník
 * @return  void
 */
void stackDelete (tStack *s) {
  
  while (!stackEmpty(s)) {
    stackPop(s);
  }

}

/*
 * Pøeète a vyjme polo¾ku na vrcholu zásobníku.
 * @param   ukazatel na zásobník
 * @param   ukazatel, kam se ulo¾í data
 * @return  void
 */

void stackTopPop (tStack *s, int *uk) {

  stackTop(s, uk);
  stackPop(s);
}


/* konec stack.c */
