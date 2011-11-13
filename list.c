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
int InitList (tList *L) {

  if (L != NULL) {      // inicializace
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
  }
  else return LIST_ERR; // neplatný uk
  return LIST_EOK;
}

/*
 * Zru¹í v¹echny prvky seznamu a inicializuje seznam.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int DisposeList (tList *L) {
  
  if (L != NULL) {
    tLElemPtr pom = NULL;
    L->Act = NULL;
    L->Last = NULL;

    while (L->First != NULL) {  // mazání prvkù
      pom = L->First;
      L->First = L->First->ptr;
      free(pom);
    }
  }
  else return LIST_ERR;         // neplatný uk
  return LIST_EOK;

}

/*
 * Vlo¾í polo¾ku s ukazatelem na data na zaèátek seznamu.
 * Ukazatel na NULL a nedostatek pamìti vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int InsertFirst  (tList *L, void *data) {

  if (L != NULL) {
    tLElemPtr pom = NULL;        // alokace nového prvku

    if ( (pom = (tLElemPtr)malloc(sizeof(struct tLElem))) != NULL ) {
      pom->data = data;         // inicializace
      pom->ptr = L->First;
      L->First = pom;
                                // první prvek je i poslední
      if (L->First->ptr == NULL) L->Last = pom; 
    }
    else return LIST_EALLOC;    // nedostatek pamìti
  }
  else return LIST_ERR;         // neplatný uk
  return LIST_EOK;

}

/*
 * Vlo¾í polo¾ku s ukazatelem na data na konec seznamu.
 * Ukazatel na NULL a nedostatek pamìti vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int InsertLast  (tList *L, void *data) {

  if (L != NULL) {
    tLElemPtr pom = NULL;        // alokace nového prvku

    if ( (pom = (tLElemPtr)malloc(sizeof(struct tLElem))) != NULL ) {
      pom->data = data;         // inicializace
      pom->ptr = NULL;

      if (L->First == NULL) {   // seznam byl prázdný
        L->First = pom;         // nový prvek je prvním
      }
      else {                    // v seznamu alespoò 1 prvek
        L->Last->ptr = pom;
      }

      L->Last = pom;            // nový prvek je poslední

    }
    else return LIST_EALLOC;    // nedostatek pamìti
  }
  else return LIST_ERR;         // neplatný uk
  return LIST_EOK;

}

/*
 * Aktivitu seznamu nastaví na první polo¾ku.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int First (tList *L) {

  if (L != NULL) {
    L->Act = L->First;
  }
  else return LIST_ERR;
  return LIST_EOK;

}

/*
 * Aktivitu seznamu nastaví na poslední polo¾ku.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int Last (tList *L) {

  if (L != NULL) {
    L->Act = L->Last;
  }
  else return LIST_ERR;
  return LIST_EOK;

}

/*
 * Vrátí ukazatel na data v první polo¾ce.
 * Pokud je ukazatel na seznam neplatný,
 * nebo je seznam prázdný, vrátí se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
void *CopyFirst (tList *L) {

  if (L != NULL && L->First != NULL) {
    return L->First->data;  // v poøádku, vrací uk na data
  }
  else return NULL;         // chyba, neplatný ukazatel

}

/*
 * Vrátí ukazatel na data v poslední polo¾ce.
 * Pokud je ukazatel na seznam neplatný,
 * nebo je seznam prázdný, vrátí se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
void *CopyLast (tList *L) {

  if (L != NULL && L->Last != NULL) {
    return L->Last->data;   // v poøádku, vrací uk na data
  }
  else return NULL;         // chyba, neplatný ukazatel

}


/*
 * Zru¹í první polo¾ku seznamu, pokud byla aktivní, aktivita se zru¹í.
 * Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na seznam
 * @return  kód chyby
 */
int DeleteFirst (tList *L) {
  
  if (L != NULL) {
    if (L->First != NULL) {     // seznam není prázdný

      if (L->First == L->Act){  // první prvek je aktivní
        L->Act = NULL;          // zru¹ aktivitu
      }

      if (L->First == L->Last){ // první prvek je poslední
        L->Last = NULL;         // poslední zru¹íme
      }

      tLElemPtr pom = L->First;  // zru¹ prvek
      L->First = L->First->ptr;
      free (pom);
    }
  }

  else return LIST_ERR;         // neplatný uk na seznam
  return LIST_EOK;
}

/*
 * Vlo¾í novou polo¾ku s ukazatelem na data za aktivní.
 * Neplatný ukazatel na seznam a nedostatek pamìti vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int PostInsert (tList *L, void *data) {

  if (L != NULL) {
    if (L->Act != NULL) {        // nìjaký prvek je aktivní

      tLElemPtr pom = NULL;       // alokace
      if ( (pom = (tLElemPtr)malloc(sizeof(struct tLElem))) != NULL ) {

        pom->data = data;        // inicializace
        pom->ptr = L->Act->ptr;
        L->Act->ptr = pom;
                                 // nastavení posledního prvku
        if (L->Act == L->Last) L->Last = pom;
      }
      else return LIST_EALLOC;   // nedostatek pamìti
    }
  }
  else return LIST_ERR;          // neplatný uk na seznam
  return LIST_EOK;

}

/*
 * Posune aktivitu na následující polo¾ku seznamu.
 * Neplatný ukazatel na seznam vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int Succ (tList *L) {

  if (L != NULL) {

    if (L->Act != NULL) {
      L->Act = L->Act->ptr;
    }
  }
  else return LIST_ERR;
  return LIST_EOK;
}

/*
 * Vrátí ukazatel na data z aktivní polo¾ky.
 * Neplatný ukazatel na seznam a neaktivní seznam vrátí NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
void *Copy (tList *L) {

  if (L != NULL && L->Act != NULL) {
    return L->Act->data;  // ok, vrací se uk na data
  }
  else return NULL;       // chyba, vrací se NULL
}

/*
 * Pøepí¹e ukazatel na data aktivní polo¾ky seznamu.
 * Neplatný ukazatel na seznam vyvolá chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  kód chyby
 */
int Actualize (tList *L, void *data ) {
  
  if (L != NULL) {

    if (L->Act != NULL) {
      L->Act->data = data; // pøepí¹e uk na data 
    }
  }
  else return LIST_ERR;    // neplatný ukazatel
  return LIST_EOK;
}

/*
 * Vrátí true, pokud se seznam aktivní, jinak false.
 * Nehlídá platnost ukazatele!
 * @param   ukazatel na seznam
 * @return  true/false
 */
int Active (tList *L) {
  return (L->Act != NULL);
}

/*
 * Vrátí ukazatel na aktivní prvek.
 * Neplatnost ukazatele na seznam vrací NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na prvek seznamu
 */
tLElemPtr GetActive (tList *L) {
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
int SetActive (tList *L, tLElemPtr uk) {
  if (L != NULL ) {
    L->Act = uk;
  }
  else return LIST_ERR;
  return LIST_EOK;
}


/* konec list.c */
