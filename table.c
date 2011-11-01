#include "table.h"
#include "binaryTree.h"
#include "str.h"
#include <stdlib.h>

#define VAR_ALLOC_SIZE 8

void tableInit(TTable *T){
   BTreeInit(&(T->functions), FUNCIONS);
   T->lastAddedFunc = NULL;
}

TVar *getLastAddedVar(TFunction F){
   return (TVar *)(F.variables.lastAdded->data);
}

/*
 * @todo    dodelat inicializaci fce
 *          copirovat string to reteyce
 */
int tableInsertFunction (TTable *T, string *s){

   TFunction *f = malloc(sizeof(TFunction));
   //TBTree    *t = malloc(sizeof(TBTree));
   if(f == NULL /*|| t == NULL*/)
      return INS_MALLOC;

   //f->variables = t;
   BTreeInit(&(f->variables), VAR);
   // inicializace seznamu instrukci
//   f->constants = NULL; // inicializace seznamu komstant
   f->name = s->str;
   f->cnt = 0;

   int err = BTreeInsert(&(T->functions), s->str, f);
   if(err){
      //                 respektive f, ale takhle to je vice ramaticke
      T->lastAddedFunc = ((TFunction *)T->functions.lastAdded->data);
   }
   return err;
}

/*
 * @todo    copirovat string to reteyce
 */
int fuctionInsertVar(TFunction *F, string *s){
   TVar     *v   = malloc(sizeof(TVar));
   TVarData **vd = malloc(sizeof(TVarData)*VAR_ALLOC_SIZE);
   if(v == NULL || vd == NULL)
      return INS_MALLOC;

   //vd[0]->type  = T_NIL;
   // hodnotu neinicializuju

   v->name  = s->str; // prekopcit!! ale zatim neni hotova funkce v str.h
   v->alloc = VAR_ALLOC_SIZE;
   v->var = vd;

   return BTreeInsert(&(F->variables), s->str, v);
}


TFunction *tableSearchFunction(TTable T, string s){
   //(divej se na to jako na ukazatel na funckci)((najdi mi uzel)->jeho data)
   return (TFunction *)(BTreeSearch(&(T.functions), s.str)->data);
}

TVar *functionSearchVar  (TFunction F, string s){
   return (TVar *)(BTreeSearch(&(F.variables), s.str)->data);
}

/*
void deleteNode(TNode n, EBTreeDataType type){
   if(n != NULL){
      deleteNode(n->left,  type);
      deleteNode(n->right, type);

      // tady uz muzu smazat polozku
      switch(type){
         // predpis jak smazat data u funkce pokud jsou typu TFunction*
         case FUNCIONS:{
            TBTree *temp = ((TFunction *)n->data)->variables;
            deleteNode( temp->root, temp->type); // type by mel byt VAR
            //free(temp);  // data u funkce sem asi taky alokoval proto je mazu
            // smazat konstanty
            // smazat seznam instrukci
         }break;
         // predpis jak smazat data poku jsou typu xxx
         case VAR:{
            fprintf(stderr, "\nMazani konstanty neni implementovano!");
         }break;
         // nic nedelam
         case DEFAULT:
         default: break;
      }
      free(n);
   }
}
*/
