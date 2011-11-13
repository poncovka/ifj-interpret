/*
 * @description   Pr�ce se dynamick�m z�sobn�kem
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "stack.h"

/*
 * Inicializuje se z�sobn�k. Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na z�sobn�k
 * @return  k�d chyby
 */
int stackInit (tStack *s) {
  if (s != NULL) {
    s->top = NULL;
  }
  else return STACK_ERR; // chybn� ukazatel

  return STACK_EOK;
}

/*
 * Vr�t� nulu, pokud je z�sobn�k pr�zdn�, jinak nenulovou hodnotu.
 * Nen� o�et�ena platnost ukazatele na z�sobn�k.
 * @param   ukazatel na z�sobn�k
 * @return  true/false
 */
int stackEmpty (tStack *s) {

  return (s->top == NULL);
}

/*
 * Vr�t� ukazatel na data z polo�ky na vrcholu z�sobn�ku.
 * Pokud dojde k chyb�, vr�t� se NULL.
 * @param   ukazatel na z�sobn�k
 * @return  ukazatel na data nebo NULL
 */
void *stackTop (tStack *s) {

  if (s != NULL && !stackEmpty(s)) {
     return s->top->data;
  }
  else return NULL;
}

/*
 * Vyjme polo�ku na vrcholu z�sobn�ku.
 * Pokud je z�sobn�k pr�zdn�, vyvol� se chyba.
 * @param   ukazatel na z�sobn�k
 * @return  k�d chyby
 */
int stackPop (tStack *s) {

  if (s != NULL) {
    if (!stackEmpty(s)) {
      tSElemPtr pom = s->top;
      s->top = s->top->ptr;
      free(pom);
    }
    else return STACK_EEMPTY; // p��stup do pr�zdn�ho z�sobn�ku
  }
  else return STACK_ERR; // neplatn� ukazatel

  return STACK_EOK;
}

/*
 * Vlo�� na vrchol z�sobn�ku novou polo�ku s daty.
 * Pokud je nedostatek pam�ti, vyvol� se chyba.
 * @param   ukazatel na z�sobn�k
 * @param   ukazatel na data
 * @return  k�d chyby
 */
int stackPush (tStack *s, void *data) {

  if (s != NULL) {
    tSElemPtr elem;

    if ( (elem = (tSElemPtr)malloc(sizeof(struct tSElem))) != NULL ) {

      elem->data = data;
      elem->ptr = s->top;
      s->top = elem;
    }
    else return STACK_EALLOC; // nedostatek pam�ti
  }
  else return STACK_ERR; // neplatn� ukazatel

  return STACK_EOK;
}

/*
 * Vr�t� ukazatel na data na vrcholu z�sobn�ku
 * a vyjme polo�ku z vrcholu z�sobn�ku.
 * Pokud dojde k chyb�, vr�t� NULL.
 * @param   ukazatel na z�sobn�k
 * @return  ukazatel na data nebo NULL
 */
void *stackTopPop (tStack *s) {

  void *pom = stackTop(s);
  if ((pom == NULL) || (stackPop(s) != STACK_EOK)) {
    return NULL;
  }
  else return pom;
}

/*
 * Vyjme ze z�sobn�ku v�echny polo�ky.
 * Vol�n�m tento funkce se z�sobn�k stane pr�zdn�m.
 * @param   ukazatel na z�sobn�k
 * @return  k�d chyby
 */
int stackDelete (tStack *s) {
  
  if (s != NULL) {
    while (!stackEmpty(s)) {
      stackPop(s);
    }
  }
  else return STACK_ERR; // chybn� ukazatel

  return STACK_EOK;
}

/* konec stack.c */
