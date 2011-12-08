/*
 * @description   Práce s jednosmìrným seznamem
 * @author        Vendula Poncová - xponco00
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

typedef struct tLItem {      // struktura pro polo¾ku seznamu
   struct tLItem *next;       // ukazatel na dal¹í polo¾ku
   void *data;                // ukazatel na data
} TLItem;

typedef struct {             // struktura pro seznam:
   TLItem *Act;               // uk na aktivní prvek
   TLItem *First;             // uk na první prvek
   TLItem *Last;              // uk na poslední prvek
} TList;


// Funkce pro práci se seznamem:

int listInit         (TList *L);
int listDispose      (TList *L);

int listInsertFirst  (TList *L, void *data);
int listInsertLast   (TList *L, void *data);
int listPostInsert   (TList *L, void *data);

int listDeleteFirst  (TList *L);
int listPostDelete   (TList *L);  // neimplementováno

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
 * Aktivitu seznamu nastaví na první polo¾ku.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
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
 * Aktivitu seznamu nastaví na poslední polo¾ku.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
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
 * Posune aktivitu na následující polo¾ku seznamu.
 * Neplatný ukazatel na seznam vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
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
 * Vrátí ukazatel na data z aktivní polo¾ky.
 * Neplatný ukazatel na seznam a neaktivní seznam vrátí NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
inline void *listCopy (TList *L) {

   if (L != NULL && L->Act != NULL) {
      return L->Act->data;  // ok, vrací se uk na data
   } 
   else {
      return NULL;      // chyba, vrací se NULL
   }
}

/*
 * Vrátí ukazatel na data v první polo¾ce.
 * Pokud je ukazatel na seznam neplatný,
 * nebo je seznam prázdný, vrátí se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
inline void *listCopyFirst (TList *L) {

   if (L != NULL && L->First != NULL) {
      return L->First->data;  // v poøádku, vrací uk na data
   } else return NULL;       // chyba, neplatný ukazatel

}

/*
 * Vrátí ukazatel na data v poslední polo¾ce.
 * Pokud je ukazatel na seznam neplatný,
 * nebo je seznam prázdný, vrátí se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
inline void *listCopyLast (TList *L) {

   if (L != NULL && L->Last != NULL) {
      return L->Last->data;   // v poøádku, vrací uk na data
   } else return NULL;       // chyba, neplatný ukazatel
}

/*
 * Vrátí true, pokud se seznam aktivní, jinak false.
 * Nehlídá platnost ukazatele!
 * @param   ukazatel na seznam
 * @return  true/false
 */
inline int listActive (TList *L) {
   return (L->Act != NULL);
}

/*
 * Vrátí ukazatel na aktivní prvek.
 * Neplatnost ukazatele na seznam vrací NULL.
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
 * Nastaví daný prvek na aktivní.
 * Neplatnost ukazatele na seznam vrací chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na prvek seznamu
 * @return  kód chyby
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
