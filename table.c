

#include "table.h"

int varRealloc(TVar *v, int cnt){
   if((cnt = v->alloc - cnt) >= 1)
      return INS_OK;
   cnt *=-1;
   cnt = cnt / VAR_ALLOC_SIZE + 1;

   v->alloc += cnt*VAR_ALLOC_SIZE;
   if( ( v->var = realloc(v->var, v->alloc) ) == NULL)
      return INS_MALLOC;
   return INS_OK;
}
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


int tableInsertFunction (TTable *T, string s){

   TFunction *f  = malloc(sizeof(TFunction));
   char *newName = strCopyChar(&s);

   if(f == NULL || newName == NULL)
      return INS_MALLOC;

   BTreeInit(&(f->variables), VAR);
   listInit (&(f->constants));
   listInit (&(f->instructions));

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

void freeConstList(TList *l){
   listFirst(l);
   while( l->Act != NULL ){
      free( ((TVar *)l->Act->data)->name );
      free( ((TVar *)l->Act->data)->var );
      free( l->Act->data );

      listDeleteFirst(l);
      listFirst(l);
   }
}

void freeInstrList(TList *l){
   listFirst(l);
   while( l->Act != NULL ){
      free( l->Act->data );

      listDeleteFirst(l);
      listFirst(l);
   }
}

/*
 * pomocna fce pro tableClear, maze podle urceneho kontextu
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
            TFunction *f = ((TFunction *)n->data);

            clearNode( f->variables.root, f->variables.type); // type by mel byt VAR
            freeConstList( &(f->constants) );
            freeInstrList( &(f->instructions) );
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
