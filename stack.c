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
int stackInit (TStack *s) {
   if (s != NULL) {
      s->top = NULL;
   } else return STACK_ERR; // chybn� ukazatel

   return STACK_EOK;
}

/*
 * Vyjme ze z�sobn�ku v�echny polo�ky.
 * Vol�n�m tento funkce se z�sobn�k stane pr�zdn�m.
 * @param   ukazatel na z�sobn�k
 * @return  k�d chyby
 */
int stackDelete (TStack *s) {

   if (s != NULL) {
      while (!stackEmpty(s)) {
         stackPop(s);
      }
   } else return STACK_ERR; // chybn� ukazatel

   return STACK_EOK;
}

/* konec stack.c */
