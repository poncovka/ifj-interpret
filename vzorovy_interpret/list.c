/*
 * @description   Pr�ce se jednosm�rn�m seznamem
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "list.h"

/*
 * Inicializuje se seznam.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
int listInit (TList *L) {

  if (L != NULL) {      // inicializace
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
  }
  else return LIST_ERR; // neplatn� uk
  return LIST_EOK;
}

/*
 * Zru�� v�echny prvky seznamu a inicializuje seznam.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
int listDispose (TList *L) {
  
  if (L != NULL) {
    TLElemPtr pom = NULL;
    L->Act = NULL;
    L->Last = NULL;

    while (L->First != NULL) {  // maz�n� prvk�
      pom = L->First;
      L->First = L->First->next;
      free(pom);
    }
  }
  else return LIST_ERR;         // neplatn� uk
  return LIST_EOK;

}

/*
 * Vlo�� polo�ku s ukazatelem na data na za��tek seznamu.
 * Ukazatel na NULL a nedostatek pam�ti vyvol� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  k�d chyby
 */
int listInsertFirst  (TList *L, void *data) {

  if (L != NULL) {
    TLElemPtr pom = NULL;        // alokace nov�ho prvku

    if ( (pom = (TLElemPtr)malloc(sizeof(struct TLElem))) != NULL ) {
      pom->data = data;         // inicializace
      pom->next = L->First;
      L->First = pom;
                                // prvn� prvek je i posledn�
      if (L->First->next == NULL) L->Last = pom; 
    }
    else return LIST_EALLOC;    // nedostatek pam�ti
  }
  else return LIST_ERR;         // neplatn� uk
  return LIST_EOK;

}

/*
 * Vlo�� polo�ku s ukazatelem na data na konec seznamu.
 * Ukazatel na NULL a nedostatek pam�ti vyvol� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  k�d chyby
 */
int listInsertLast  (TList *L, void *data) {

  if (L != NULL) {
    TLElemPtr pom = NULL;        // alokace nov�ho prvku

    if ( (pom = (TLElemPtr)malloc(sizeof(struct TLElem))) != NULL ) {
      pom->data = data;         // inicializace
      pom->next = NULL;

      if (L->First == NULL) {   // seznam byl pr�zdn�
        L->First = pom;         // nov� prvek je prvn�m
      }
      else {                    // v seznamu alespo� 1 prvek
        L->Last->next = pom;
      }

      L->Last = pom;            // nov� prvek je posledn�

    }
    else return LIST_EALLOC;    // nedostatek pam�ti
  }
  else return LIST_ERR;         // neplatn� uk
  return LIST_EOK;

}

/*
 * Aktivitu seznamu nastav� na prvn� polo�ku.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
int listFirst (TList *L) {

  if (L != NULL) {
    L->Act = L->First;
  }
  else return LIST_ERR;
  return LIST_EOK;

}

/*
 * Aktivitu seznamu nastav� na posledn� polo�ku.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
int listLast (TList *L) {

  if (L != NULL) {
    L->Act = L->Last;
  }
  else return LIST_ERR;
  return LIST_EOK;

}

/*
 * Vr�t� ukazatel na data v prvn� polo�ce.
 * Pokud je ukazatel na seznam neplatn�,
 * nebo je seznam pr�zdn�, vr�t� se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
void *listCopyFirst (TList *L) {

  if (L != NULL && L->First != NULL) {
    return L->First->data;  // v po��dku, vrac� uk na data
  }
  else return NULL;         // chyba, neplatn� ukazatel

}

/*
 * Vr�t� ukazatel na data v posledn� polo�ce.
 * Pokud je ukazatel na seznam neplatn�,
 * nebo je seznam pr�zdn�, vr�t� se NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
void *listCopyLast (TList *L) {

  if (L != NULL && L->Last != NULL) {
    return L->Last->data;   // v po��dku, vrac� uk na data
  }
  else return NULL;         // chyba, neplatn� ukazatel

}


/*
 * Zru�� prvn� polo�ku seznamu, pokud byla aktivn�, aktivita se zru��.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
int listDeleteFirst (TList *L) {
  
  if (L != NULL) {
    if (L->First != NULL) {     // seznam nen� pr�zdn�

      if (L->First == L->Act){  // prvn� prvek je aktivn�
        L->Act = NULL;          // zru� aktivitu
      }

      if (L->First == L->Last){ // prvn� prvek je posledn�
        L->Last = NULL;         // posledn� zru��me
      }

      TLElemPtr pom = L->First;  // zru� prvek
      L->First = L->First->next;
      free (pom);
    }
  }

  else return LIST_ERR;         // neplatn� uk na seznam
  return LIST_EOK;
}

/*
 * Vlo�� novou polo�ku s ukazatelem na data za aktivn�.
 * Neplatn� ukazatel na seznam a nedostatek pam�ti vyvol� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  k�d chyby
 */
int listPostInsert (TList *L, void *data) {

  if (L != NULL) {
    if (L->Act != NULL) {        // n�jak� prvek je aktivn�

      TLElemPtr pom = NULL;       // alokace
      if ( (pom = (TLElemPtr)malloc(sizeof(struct TLElem))) != NULL ) {

        pom->data = data;        // inicializace
        pom->next = L->Act->next;
        L->Act->next = pom;
                                 // nastaven� posledn�ho prvku
        if (L->Act == L->Last) L->Last = pom;
      }
      else return LIST_EALLOC;   // nedostatek pam�ti
    }
  }
  else return LIST_ERR;          // neplatn� uk na seznam
  return LIST_EOK;

}

/*
 * Posune aktivitu na n�sleduj�c� polo�ku seznamu.
 * Neplatn� ukazatel na seznam vyvol� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  k�d chyby
 */
int listSucc (TList *L) {

  if (L != NULL) {

    if (L->Act != NULL) {
      L->Act = L->Act->next;
    }
  }
  else return LIST_ERR;
  return LIST_EOK;
}

/*
 * Vr�t� ukazatel na data z aktivn� polo�ky.
 * Neplatn� ukazatel na seznam a neaktivn� seznam vr�t� NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
void *listCopy (TList *L) {

  if (L != NULL && L->Act != NULL) {
    return L->Act->data;  // ok, vrac� se uk na data
  }
  else return NULL;       // chyba, vrac� se NULL
}

/*
 * P�ep�e ukazatel na data aktivn� polo�ky seznamu.
 * Neplatn� ukazatel na seznam vyvol� chybu.
 * @param   ukazatel na seznam
 * @param   ukazatel na data
 * @return  k�d chyby
 */
int listActualize (TList *L, void *data ) {
  
  if (L != NULL) {

    if (L->Act != NULL) {
      L->Act->data = data; // p�ep�e uk na data 
    }
  }
  else return LIST_ERR;    // neplatn� ukazatel
  return LIST_EOK;
}

/*
 * Vr�t� true, pokud se seznam aktivn�, jinak false.
 * Nehl�d� platnost ukazatele!
 * @param   ukazatel na seznam
 * @return  true/false
 */
int listActive (TList *L) {
  return (L->Act != NULL);
}

/*
 * Vr�t� ukazatel na aktivn� prvek.
 * Neplatnost ukazatele na seznam vrac� NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na prvek seznamu
 */
TLElemPtr listGetActive (TList *L) {
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
int listSetActive (TList *L, TLElemPtr uk) {
  if (L != NULL ) {
    L->Act = uk;
  }
  else return LIST_ERR;
  return LIST_EOK;
}


/* konec list.c */
