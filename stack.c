/*
 * @description   Práce se dynamickým zásobníkem
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "stack.h"

/*
 * Inicializuje se zásobník. Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na zásobník
 * @return  kód chyby
 */
int stackInit (TStack *s) {
   if (s != NULL) {
      s->top = NULL;
   } else return STACK_ERR; // chybný ukazatel

   return STACK_EOK;
}

/*
 * Vyjme ze zásobníku v¹echny polo¾ky.
 * Voláním tento funkce se zásobník stane prázdným.
 * @param   ukazatel na zásobník
 * @return  kód chyby
 */
int stackDelete (TStack *s) {

   if (s != NULL) {
      while (!stackEmpty(s)) {
         stackPop(s);
      }
   } else return STACK_ERR; // chybný ukazatel

   return STACK_EOK;
}

/* konec stack.c */
