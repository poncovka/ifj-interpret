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
int InitList (tList *L) {

  if (L != NULL) {      // inicializace
    L->Act = NULL;
    L->First = NULL;
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
int DisposeList (tList *L) {
  
  if (L != NULL) {
    tElemPtr pom = NULL;
    L->Act = NULL;

    while (L->First != NULL) {  // maz�n� prvk�
      pom = L->First;
      L->First = L->First->ptr;
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
int InsertFirst  (tList *L, void *data) {

  if (L != NULL) {
    tElemPtr pom = NULL;        // alokace nov�ho prvku

    if ( (pom = (tElemPtr)malloc(sizeof(struct tElem))) != NULL ) {
      pom->data = data;         // inicializace
      pom->ptr = L->First;
      L->First = pom;
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
int First (tList *L) {

  if (L != NULL) {
    L->Act = L->First;
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
void *CopyFirst (tList *L) {

  if (L != NULL && L->First != NULL) {
    return L->First->data;  // v po��dku, vrac� uk na data
  }
  else return NULL;         // chyba, neplatn� ukazatel

}

/*
 * Zru�� prvn� polo�ku seznamu, pokud byla aktivn�, aktivita se zru��.
 * Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na seznam
 * @return  k�d chyby
 */
int DeleteFirst (tList *L) {
  
  if (L != NULL) {
    if (L->First != NULL) {     // seznam nen� pr�zdn�

      if (L->First == L->Act){  // prvn� prvek je aktivn�
        L->Act = NULL;          // zru� aktivitu
      }

      tElemPtr pom = L->First;  // zru� prvek
      L->First = L->First->ptr;
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
int PostInsert (tList *L, void *data) {

  if (L != NULL) {
    if (L->Act != NULL) {        // n�jak� prvek je aktivn�

      tElemPtr pom = NULL;       // alokace
      if ( (pom = (tElemPtr)malloc(sizeof(struct tElem))) != NULL ) {

        pom->data = data;        // inicializace
        pom->ptr = L->Act->ptr;
        L->Act->ptr = pom;
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
 * Vr�t� ukazatel na data z aktivn� polo�ky.
 * Neplatn� ukazatel na seznam a neaktivn� seznam vr�t� NULL.
 * @param   ukazatel na seznam
 * @return  ukazatel na data nebo NULL
 */
int *Copy (tList *L) {

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
int Actualize (tList *L, void *data ) {
  
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
int Active (tList *L) {
  return (L->Act != NULL);
}


/* konec list.c */
