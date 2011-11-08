#include "table.h"
#include "binaryTree.h"
#include "str.h"
#include <stdlib.h>
#include <stdio.h>

#define VAR_ALLOC_SIZE 8

//----------------------------------------------------------------------

void tableInit(TTable *T){
   BTreeInit(&(T->functions), FUNCIONS);
   T->lastAddedFunc = NULL;
}

//----------------------------------------------------------------------

TVar *getLastAddedVar(TFunction *F){
   return (TVar *)(F->variables.lastAdded->data);
}

//----------------------------------------------------------------------

/*
 * @todo    INICIALIZACE !!!
 */
int tableInsertFunction (TTable *T, string s){

   TFunction *f  = malloc(sizeof(TFunction));
   char *newName = strCopyChar(&s);

   if(f == NULL || newName == NULL)
      return INS_MALLOC;

   BTreeInit(&(f->variables), VAR);
   // inicializace seznamu instrukci
   // inicializace seznamu komstant

   f->name = newName;
   f->cnt = -1;

   int err = BTreeInsert(&(T->functions), newName, f);
   if(err)  // probehl insert v poradku?
      T->lastAddedFunc = f;
   // T->lastAddedFunc = ((TFunction *)T->functions.lastAdded->data);
   else{
      free(f);
      free(newName);
   }

   return err;
}

//----------------------------------------------------------------------

int functionInsertVar(TFunction *F, string s){
   TVar     *v   = malloc(sizeof(TVar));
   TVarData *vd  = malloc(sizeof(TVarData)*VAR_ALLOC_SIZE);
   char *newName = strCopyChar(&s);

   if(v == NULL || vd == NULL || newName == NULL)
      return INS_MALLOC;

   v->name  = newName;
   v->alloc = VAR_ALLOC_SIZE;
   v->var   = vd;
   v->type  = VT_VAR;
   int err = BTreeInsert(&(F->variables), newName, v);
   if(!err){
      free(v);
      free(vd);
      free(newName);
   }
   return err;
}

//----------------------------------------------------------------------

TFunction *tableSearchFunction(TTable *T, string s){
   TNode n = BTreeSearch(&(T->functions), s.str);
   return (n != NULL) ? (TFunction *)(n->data) : NULL;
}

//----------------------------------------------------------------------

TVar *functionSearchVar  (TFunction *F, string s){
   TNode n = BTreeSearch(&(F->variables), s.str);
   return (n != NULL) ? (TVar *)(n->data) : NULL;
}

//----------------------------------------------------------------------

/*
 * pomocna fce pro tableClear, maza podle urceneho kontextu
 * bacha na to! maze strom funkci :)
 * @param   uzel strom
 * @param   typ stromu(urcuje predpis mazani dat)
 */
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
            // smazat konstanty
            // smazat seznam instrukci
            free(n->data);  // data jsem asi taky alokovala proto ji smazu
         }break;
         // predpis jak smazat data poku jsou typu TVar *
         case VAR:{
            TVar *temp = ((TVar *)n->data);
            free(temp->var);
            free(n->data);
         }break;
         // nic nedelam
         case DEFAULT:
         default: break;
      }
      free(n->key);
      free(n);
   }
}


void  tableClear(TTable *T){
   clearNode(T->functions.root, T->functions.type);
   tableInit(T);
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


//             uzel     typ stromu       oddelovac
void printNode(TNode n, EBTreeDataType t, char *delim){
   if(n != NULL){
      printNode(n->left, t, delim);

      printf("%s%s\n", delim, n->key);
      switch(t){
         case FUNCIONS:{
               TBTree *temp   = &(((TFunction *)n->data)->variables);
               //printf("     variables:\n");
               printNode( temp->root, temp->type /*VAR*/, "      " );
         }break;
         case VAR:{
            //fprintf(stderr, "\nTisk promene nebo konstanty neni implenetovan!");
         }break;
         case DEFAULT:
         default : break;
      }

      printNode(n->right, t, delim);
   }
}

void printTreeNodeOrder(TBTree *T){
   printNode(T->root, T->type, "   ");
}

void tablePrintOrder(TTable table){
   printTreeNodeOrder(&(table.functions)) ;
}
