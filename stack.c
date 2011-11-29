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
 * Vrátí nulu, pokud je zásobník prázdný, jinak nenulovou hodnotu.
 * Není o¹etøena platnost ukazatele na zásobník.
 * @param   ukazatel na zásobník
 * @return  true/false
 */
int stackEmpty (TStack *s) {

   return (s->top == NULL);
}

/*
 * Vrátí ukazatel na data z polo¾ky na vrcholu zásobníku.
 * Pokud dojde k chybì, vrátí se NULL.
 * @param   ukazatel na zásobník
 * @return  ukazatel na data nebo NULL
 */
void *stackTop (TStack *s) {

   if (s != NULL && !stackEmpty(s)) {
      return s->top->data;
   } else return NULL;
}

/*
 * Vyjme polo¾ku na vrcholu zásobníku.
 * Pokud je zásobník prázdný, vyvolá se chyba.
 * @param   ukazatel na zásobník
 * @return  kód chyby
 */
int stackPop (TStack *s) {

   if (s != NULL) {
      if (!stackEmpty(s)) {
         TSItem *pom = s->top;
         s->top = s->top->next;
         free(pom);
      } else return STACK_EEMPTY; // pøístup do prázdného zásobníku
   } else return STACK_ERR; // neplatný ukazatel

   return STACK_EOK;
}

/*
 * Vlo¾í na vrchol zásobníku novou polo¾ku s daty.
 * Pokud je nedostatek pamìti, vyvolá se chyba.
 * @param   ukazatel na zásobník
 * @param   ukazatel na data
 * @return  kód chyby
 */
int stackPush (TStack *s, void *data) {

   if (s != NULL) {
      TSItem *elem;

      if ( (elem = (TSItem *)malloc(sizeof(TSItem))) != NULL ) {

         elem->data = data;
         elem->next = s->top;
         s->top = elem;
      } else return STACK_EALLOC; // nedostatek pamìti
   } else return STACK_ERR; // neplatný ukazatel

   return STACK_EOK;
}

/*
 * Vrátí ukazatel na data na vrcholu zásobníku
 * a vyjme polo¾ku z vrcholu zásobníku.
 * Pokud dojde k chybì, vrátí NULL.
 * @param   ukazatel na zásobník
 * @return  ukazatel na data nebo NULL
 */
void *stackTopPop (TStack *s) {

   void *pom = stackTop(s);
   if ((pom == NULL) || (stackPop(s) != STACK_EOK)) {
      return NULL;
   } else return pom;
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
