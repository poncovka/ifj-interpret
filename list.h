/*
 * @description   Pr�ce s jednosm�rn�m seznamem
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LIST_EOK     0
#define LIST_ERR    -1
#define LIST_EALLOC -5

typedef struct tLItem {      // struktura pro polo�ku seznamu
   struct tLItem *next;       // ukazatel na dal�� polo�ku
   void *data;                // ukazatel na data
} TLItem;

typedef struct {             // struktura pro seznam:
   TLItem *Act;               // uk na aktivn� prvek
   TLItem *First;             // uk na prvn� prvek
   TLItem *Last;              // uk na posledn� prvek
} TList;


// Funkce pro pr�ci se seznamem:

int listInit         (TList *L);
int listDispose      (TList *L);

int listInsertFirst  (TList *L, void *data);
int listInsertLast   (TList *L, void *data);
int listPostInsert   (TList *L, void *data);

int listDeleteFirst  (TList *L);
int listPostDelete   (TList *L);  // neimplementov�no

int listActualize    (TList *L, void *data );

inline int   listFirst     (TList *L);
inline int   listLast      (TList *L);
inline int   listSucc      (TList *L);
inline void *listCopy      (TList *L);
inline void *listCopyFirst (TList *L);
inline void *listCopyLast  (TList *L);
inline int   listActive    (TList *L);

inline TLItem *listGetActive (TList *L);
inline int listSetActive (TList *L, TLItem *uk);

/////////////////////////////////////////////////////////////////
/* inline funkce */


/*
 * Aktivitu seznamu nastav� na prvn� polo�ku.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
inline int listFirst (TList *L) {

   if (L != NULL) {
      L->Act = L->First;
      return LIST_EOK;
   } 
   else {
      return LIST_ERR;
   }
}

/*
 * Aktivitu seznamu nastav� na posledn� polo�ku.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
inline int listLast (TList *L) {

   if (L != NULL) {
      L->Act = L->Last;
      return LIST_EOK;
   } 
   else { 
      return LIST_ERR;
   }
}

/*
 * Posune aktivitu na n�sleduj�c� polo�ku seznamu.
 * Neplatn� ukazatel na seznam vyvol� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  k�d chyby
 */
inline int listSucc (TList *L) {

   if (L != NULL) {
      if (L->Act != NULL) {
         L->Act = L->Act->next;
      }
     return LIST_EOK;
   }
   else {
     return LIST_ERR;
   }
}

/*
 * Vr�t� ukazatel na data z aktivn� polo�ky.
 * Neplatn� ukazatel na seznam a neaktivn� seznam vr�t� NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
inline void *listCopy (TList *L) {

   if (L != NULL && L->Act != NULL) {
      return L->Act->data;  // ok, vrac� se uk na data
   } 
   else {
      return NULL;      // chyba, vrac� se NULL
   }
}

/*
 * Vr�t� ukazatel na data v prvn� polo�ce.
 * Pokud je ukazatel na seznam neplatn�,
 * nebo je seznam pr�zdn�, vr�t� se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
inline void *listCopyFirst (TList *L) {

   if (L != NULL && L->First != NULL) {
      return L->First->data;  // v po��dku, vrac� uk na data
   } else return NULL;       // chyba, neplatn� ukazatel

}

/*
 * Vr�t� ukazatel na data v posledn� polo�ce.
 * Pokud je ukazatel na seznam neplatn�,
 * nebo je seznam pr�zdn�, vr�t� se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
inline void *listCopyLast (TList *L) {

   if (L != NULL && L->Last != NULL) {
      return L->Last->data;   // v po��dku, vrac� uk na data
   } else return NULL;       // chyba, neplatn� ukazatel
}

/*
 * Vr�t� true, pokud se seznam aktivn�, jinak false.
 * Nehl�d� platnost ukazatele!
 * @param   ukazatel na seznam
 * @return  true/false
 */
inline int listActive (TList *L) {
   return (L->Act != NULL);
}

/*
 * Vr�t� ukazatel na aktivn� prvek.
 * Neplatnost ukazatele na seznam vrac� NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na prvek seznamu
 */
inline TLItem *listGetActive (TList *L) {
   if (L != NULL) {
      return L->Act;
   }
   return NULL;
}

/*
 * Nastav� dan� prvek na aktivn�.
 * Neplatnost ukazatele na seznam vrac� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na prvek seznamu
 * @return  k�d chyby
 */
inline int listSetActive (TList *L, TLItem *uk) {
   if (L != NULL ) {
      L->Act = uk;
      return LIST_EOK;
   } 
   else {
      return LIST_ERR;
   }
}

#endif // LIST_H_INCLUDED
