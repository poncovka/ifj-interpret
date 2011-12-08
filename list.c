/*
 * @description   Práce se jednosmìrným seznamem
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "list.h"

/*
 * Inicializuje se seznam.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int listInit (TList *L) {

   if (L != NULL) {      // inicializace
      L->Act = NULL;
      L->First = NULL;
      L->Last = NULL;
   } else return LIST_ERR; // neplatný uk
   return LIST_EOK;
}

/*
 * Zru¹í v¹echny prvky seznamu a inicializuje seznam.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int listDispose (TList *L) {

   if (L != NULL) {
      TLItem *pom = NULL;
      L->Act = NULL;
      L->Last = NULL;

      while (L->First != NULL) {  // mazání prvkù
         pom = L->First;
         L->First = L->First->next;
         free(pom);
      }
   } else return LIST_ERR;       // neplatný uk
   return LIST_EOK;

}

/*
 * Vlo¾í polo¾ku s ukazatelem na data na zaèátek seznamu.
 * Ukazatel na NULL a nedostatek pamìti vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int listInsertFirst  (TList *L, void *data) {

   if (L != NULL) {
      TLItem *pom = NULL;        // alokace nového prvku

      if ( (pom = (TLItem*)malloc(sizeof(TLItem))) != NULL ) {
         pom->data = data;         // inicializace
         pom->next = L->First;
         L->First = pom;
         // první prvek je i poslední
         if (L->First->next == NULL) L->Last = pom;
      } else return LIST_EALLOC;  // nedostatek pamìti
   } else return LIST_ERR;       // neplatný uk
   return LIST_EOK;

}

/*
 * Vlo¾í polo¾ku s ukazatelem na data na konec seznamu.
 * Ukazatel na NULL a nedostatek pamìti vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int listInsertLast  (TList *L, void *data) {

   if (L != NULL) {
      TLItem *pom = NULL;        // alokace nového prvku

      if ( (pom = (TLItem*)malloc(sizeof(TLItem))) != NULL ) {
         pom->data = data;         // inicializace
         pom->next = NULL;

         if (L->First == NULL) {   // seznam byl prázdný
            L->First = pom;         // nový prvek je prvním
         } else {                  // v seznamu alespoò 1 prvek
            L->Last->next = pom;
         }

         L->Last = pom;            // nový prvek je poslední

      } else return LIST_EALLOC;  // nedostatek pamìti
   } else return LIST_ERR;       // neplatný uk
   return LIST_EOK;

}

/*
 * Vlo¾í novou polo¾ku s ukazatelem na data za aktivní.
 * Neplatný ukazatel na seznam a nedostatek pamìti vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int listPostInsert (TList *L, void *data) {

   if (L != NULL) {
      if (L->Act != NULL) {        // nìjaký prvek je aktivní

         TLItem *pom = NULL;       // alokace
         if ( (pom = (TLItem*)malloc(sizeof(TLItem))) != NULL ) {

            pom->data = data;        // inicializace
            pom->next = L->Act->next;
            L->Act->next = pom;
            // nastavení posledního prvku
            if (L->Act == L->Last) L->Last = pom;
         } else return LIST_EALLOC; // nedostatek pamìti
      }
   } else return LIST_ERR;        // neplatný uk na seznam
   return LIST_EOK;

}

/*
 * Zru¹í první polo¾ku seznamu, pokud byla aktivní, aktivita se zru¹í.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int listDeleteFirst (TList *L) {

   if (L != NULL) {
      if (L->First != NULL) {     // seznam není prázdný

         if (L->First == L->Act) { // první prvek je aktivní
            L->Act = NULL;          // zru¹ aktivitu
         }

         if (L->First == L->Last) { // první prvek je poslední
            L->Last = NULL;         // poslední zru¹íme
         }

         TLItem *pom = L->First;  // zru¹ prvek
         L->First = L->First->next;
         free (pom);
      }
   }

   else return LIST_ERR;         // neplatný uk na seznam
   return LIST_EOK;
}

/*
 * Pøepí¹e ukazatel na data aktivní polo¾ky seznamu.
 * Neplatný ukazatel na seznam vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int listActualize (TList *L, void *data ) {

   if (L != NULL || data == NULL) {

      if (L->Act != NULL) {
         L->Act->data = data; // pøepí¹e uk na data
      }
   } else return LIST_ERR;  // neplatný ukazatel
   return LIST_EOK;
}

/* konec list.c */
