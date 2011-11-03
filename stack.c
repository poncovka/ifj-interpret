/*
 * @description   Pr�ce se dynamick�m z�sobn�kem
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "stack.h"

// co s chybama?
#define error() printf("Chyba!\n")

/*
 * Inicializuje se z�sobn�k. Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na z�sobn�k
 * @return  void
 */
void stackInit (tStack *s) {
  if (s != NULL) {
    s->top = NULL;
  }
  else error();

}

/*
 * Vr�t� nulu, pokud je z�sobn�k pr�zdn�, jinak nenulovou hodnotu.
 * @param   ukazatel na z�sobn�k
 * @return  true/false
 */
int stackEmpty (tStack *s) {

  return (s->top == NULL);
}

/*
 * Vr�t� data z polo�ky na vrcholu z�sobn�ku.
 * Pokud je z�sobn�k pr�zdn�, vyvol� se chyba.
 * @param   ukazatel na z�sobn�k
 * @param   ukazatel, kam se data ulo��
 * @return  void
 */
void stackTop (tStack *s, int *uk) {

  if (!stackEmpty(s)) {
    *uk = s->top->data;
  }
  else error();

}

/*
 * Vyjme polo�ku na vrcholu z�sobn�ku.
 * Pokud je z�sobn�k pr�zdn�, vyvol� se chyba.
 * @param   ukazatel na z�sobn�k
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
 * Vlo�� na vrchol z�sobn�ku novou polo�ku s daty.
 * Pokud je nedostatek p�mti, vyvol� se chyba.
 * @param   ukazatel na z�sobn�k
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
 * Vyjme ze z�sobn�ku v�echny polo�ky.
 * Vol�n�m tento funkce se z�sobn�k stane pr�zdn�m.
 * @param   ukazatel na z�sobn�k
 * @return  void
 */
void stackDelete (tStack *s) {
  
  while (!stackEmpty(s)) {
    stackPop(s);
  }

}

/*
 * P�e�te a vyjme polo�ku na vrcholu z�sobn�ku.
 * @param   ukazatel na z�sobn�k
 * @param   ukazatel, kam se ulo�� data
 * @return  void
 */

void stackTopPop (tStack *s, int *uk) {

  stackTop(s, uk);
  stackPop(s);
}


/* konec stack.c */
