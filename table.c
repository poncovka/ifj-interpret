#include "table.h"
#include "binaryTree.h"
#include "str.h"
#include <stdlib.h>

#define VAR_ALLOC_SIZE 8

//----------------------------------------------------------------------

void tableInit(TTable *T){
   BTreeInit(&(T->functions), FUNCIONS);
   T->lastAddedFunc = NULL;
}

//----------------------------------------------------------------------

TVar *getLastAddedVar(TFunction F){
   return (TVar *)(F.variables.lastAdded->data);
}

//----------------------------------------------------------------------

/*
 * @todo    dodelat inicializaci fce
 *          copirovat string to reteyce
 */
int tableInsertFunction (TTable *T, string *s){

   TFunction *f = malloc(sizeof(TFunction));
   if(f == NULL)
      return INS_MALLOC;

   BTreeInit(&(f->variables), VAR);
   // inicializace seznamu instrukci
   //f->constants = NULL; // inicializace seznamu komstant
   f->name = s->str; // PREKOPCIT!! ale zatim neni hotova funkce v str.h
   f->cnt = 0;

   int err = BTreeInsert(&(T->functions), s->str, f);
   if(err){
      //                 respektive f, ale takhle to je vice dramaticke
      //T->lastAddedFunc = ((TFunction *)T->functions.lastAdded->data);
      T->lastAddedFunc = f;
   }
   return err;
}

//----------------------------------------------------------------------

/*
 * @todo    copirovat string to retezce
 */
int fuctionInsertVar(TFunction *F, string *s){
   TVar     *v   = malloc(sizeof(TVar));
   TVarData **vd = malloc(sizeof(TVarData)*VAR_ALLOC_SIZE);
   if(v == NULL || vd == NULL)
      return INS_MALLOC;

   //vd[0]->type  = T_NIL;
   // hodnotu neinicializuju

   v->name  = s->str; // PREKOPCIT!! ale zatim neni hotova funkce v str.h
   v->alloc = VAR_ALLOC_SIZE;
   v->var = vd;

   return BTreeInsert(&(F->variables), s->str, v);
}

//----------------------------------------------------------------------

TFunction *tableSearchFunction(TTable T, string s){
   TNode n = BTreeSearch(&(T.functions), s.str);
   return (n != NULL) ? (TFunction *)(n->data) : NULL;
}

//----------------------------------------------------------------------

TVar *functionSearchVar  (TFunction F, string s){
   TNode n = BTreeSearch(&(F.variables), s.str);
   return (n != NULL) ? (TVar *)(n->data) : NULL;
}

//----------------------------------------------------------------------


void clearNode(TNode n, EBTreeDataType type){
   if(n != NULL){
      clearNode(n->left,  type);
      clearNode(n->right, type);

      // tady uz muzu smazat polozku
      switch(type){
         // predpis jak smazat data u funkce pokud jsou typu TFunction*
         case FUNCIONS:{
            TBTree *temp = &(((TFunction *)n->data)->variables);
            clearNode( temp->root, temp->type); // type by mel byt VAR
            // free(temp->name);
            // smazat konstanty
            // smazat seznam instrukci
            free(temp);  // data u funkce sem asi taky alokovala proto je mazu
         }break;
         // predpis jak smazat data poku jsou typu xxx
         case VAR:{
            TVar *temp = ((TVar *)n->data);
            free(temp->var);
            free(temp);
         }break;
         // nic nedelam
         case DEFAULT:
         default: break;
      }
      free(n);
   }
}


void  tableClear(TTable *T){
   clearNode(T->functions.root, T->functions.type);
   tableInit(T);
}
