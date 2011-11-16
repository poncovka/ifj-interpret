#include "table.h"

#define VAR_ALLOC_SIZE 8

TInstr *genInstr(EInstrType t, void *d, void *s1, void *s2){
   TInstr *i = malloc(sizeof(TInstr));
   if(i == NULL)
      return NULL;
   i->type = t;
   i->dest = d;
   i->src1 = s1;
   i->src2 = s2;

   return i;
}

int varRealloc(TVar *v, int cnt){
   if((cnt = v->alloc - cnt) >= 1)
      return INS_OK;
   cnt *=-1;
   cnt = cnt / VAR_ALLOC_SIZE + 1;

   v->alloc += cnt*VAR_ALLOC_SIZE;
   if( ( v->varData = realloc(v->varData, v->alloc) ) == NULL)
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
   listInit (&(f->tmpVar));

   f->name = newName;
   f->cnt = -1;

   int err = BTreeInsert(&(T->functions), newName, f);
   if(err <= 0){  // neprobehl insert v poradku?
      free(f);
      free(newName);
      return INS_MALLOC;
   }

   T->lastAddedFunc = f;
   // T->lastAddedFunc = ((TFunction *)T->functions.lastAdded->data);
   return INS_OK;
}

//----------------------------------------------------------------------

int functionInsertVar(TFunction *F, string s){
   TVar     *v   = malloc(sizeof(TVar));
   if(v == NULL)
      return INS_MALLOC;

   TVarData *vd  = malloc(sizeof(TVarData)*VAR_ALLOC_SIZE);
   if(vd == NULL){
      free(v);
      return INS_MALLOC;
   }

   char *newName = strCopyChar(&s);
   if(newName == NULL){
      free(v);
      free(vd);
      return INS_MALLOC;
   }

   v->name  = newName;
   v->alloc = VAR_ALLOC_SIZE;
   v->varData  = vd;
   v->varType  = VT_VAR;
   int err = BTreeInsert(&(F->variables), newName, v);
   if(err <= 0){
      free(v);
      free(vd);
      free(newName);
      return INS_MALLOC;
   }
   return INS_OK;
}

//----------------------------------------------------------------------

int functionInsertConstatnt(TFunction *f, string attr, int token){
   // vytvorim promenou
   TVar     *v   = malloc(sizeof(TVar));
   if(v == NULL)
      return INS_MALLOC;

   // vytvorim jeji data
   TVarData *vd  = malloc(sizeof(TVarData));
   if(vd == NULL){
      free(v);
      return INS_MALLOC;
   }

   int err = INS_OK;
   // zjistim co mam ulozit a pak to tak ulozim
   switch(token){
      case L_NUMBER:{
            vd->type = NUMBER;
            vd->value.n = atof ( attr.str );
         }break;
      case L_STRING:{
            vd->type = STRING;
            strInit( &(vd->value.s) );
            if( strCopyString( &(vd->value.s), &attr ) == STR_ERROR)
               err = INS_MALLOC;
         }break;
      case KW_TRUE:{
            vd->type = BOOL;
            vd->value.b = 1;
         }break;
      case KW_FALSE:{
            vd->type = BOOL;
            vd->value.b = 0;
         }break;
      case KW_NIL:{
            vd->type = NIL;
         }break;
   }

   v->name  = NULL;
   v->alloc = 1;
   v->varType = VT_CONST;
   v->varData = vd;

   if(listInsertLast(&f->constants, v) != LIST_EOK || err != INS_OK){
      // kdyz se nepovedlo vlozit nebo pokud predtim u vytdareni dat
      // doslo k chybe musim vechno smazat
      free(v);
      if(vd->type == STRING)
         strFree(&(vd->value.s));
      free(vd);
      return INS_MALLOC;
   }

   return INS_OK;
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

void freeConstTmpVarList(TList *l){
   listFirst(l);
   while( l->Act != NULL ){
      TVar *tmp = (TVar *)l->Act->data;

      if(tmp->varType == VT_VAR){
         // jestli to byla promena tak musim projit cele pole a smazat vsechny stringy + jmeno
         for(int i = 0; i < tmp->alloc; i++){
            if(tmp->varData[i].type == STRING)
               free(tmp->varData[i].value.s.str);
         }
         // jmeno jsem taky alokoval
         free(tmp->name);
      }
      else if(tmp->varType == VT_CONST || tmp->varType == VT_TMP_VAR){
         // jestlize je to konstanta nebo pomocna promena tak overim jestli je string a pripadne smazu
         if(tmp->varData->type == STRING)
            free(tmp->varData->value.s.str);
      }
      // protoze sem musel pole alokovat tak ho musim smazat
      free(tmp->varData );
      free(tmp); // data jsem asi zrejmne taky alokoval tak je mazu

      listDeleteFirst(l);
      listFirst(l);
   }
}

void freeInstrList(TList *l){
   listFirst(l);
   while( l->Act != NULL ){
      free( l->Act->data );   // u instrukce byla data alokovana musim je smazat

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
            freeConstTmpVarList( &(f->constants) );
            freeConstTmpVarList( &(f->tmpVar) );
            freeInstrList( &(f->instructions) );
            free(n->data);  // data jsem asi taky alokovala proto ji smazu
         }break;
         // predpis jak smazat data poku jsou typu TVar *
         case VAR:{
            TVar *temp = ((TVar *)n->data);
            free(temp->varData);
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
// PRO DEBUG!!

void listConstTmpPrint(TList *l){
   listFirst(l);
   while(listActive(l)){
      TVarData *tmp = ((TVar *)l->Act->data)->varData;
      printf("         ");

      switch( tmp->type ){
         case STRING:{
               printf("string \"%s\"", tmp->value.s.str);
            }break;
         case NUMBER:{
               printf("number %g", tmp->value.n);
            }break;
         case BOOL:{
               printf("bool %s", tmp->value.b == 1 ? "true" : "false");
            }break;
         case NIL:{
               printf("nil");
            }break;
         default: printf("unknow const");
      }
      printf("\n");
      listSucc(l);
   }
}

void printVar(TVar *src){
   if(src != NULL){
      if(src->varType == VT_VAR)
         printf(" %s",src->name);
      else{
         switch(src->varData->type){
         case STRING: printf(" \"%s\"", src->varData->value.s.str);break;
         case NUMBER: printf(" %g", src->varData->value.n);break;
         case BOOL:   printf(" %s", src->varData->value.b == 1 ? "true" : "false");break;
         case NIL:    printf(" nil");break;
      }
      }
   }
}
void listInstrPrint(TList *l){
   listFirst(l);
   while(listActive(l)){
      TInstr *tmp = ((TInstr *)l->Act->data);
      TVar *dst  = ((TVar *)tmp->dest);
      TVar *src1 = ((TVar *)tmp->src1);
      TVar *src2 = ((TVar *)tmp->src2);
      printf("         ");

      switch (tmp->type) {
         case I_LAB: break;
         case I_RETURN: break;
         case I_POP: {
               printf("POP");
               printVar(dst);
            }break;
         case I_PUSH: {
               printf("PUSH");
               printVar(dst);
            }break;
         case I_STACK_E: {
               printf("STACK_E");
            }break;
         case I_MOV: break;
         case I_SET: {
               printf("SET");
               printVar(dst);
               printVar(src1);
            }break;
         case I_ADD: {
               printf("ADD");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_SUB:{
               printf("SUB");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_MUL: {
               printf("MUL");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_DIV:{
               printf("DIV");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_POW: {
               printf("POW");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CON: {
               printf("CON");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CMP_L:{
               printf("CMP_L");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CMP_LE:{
               printf("CMP_LE");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CMP_G:{
               printf("CMP_G");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CMP_GE:{
               printf("CMP_GE");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CMP_E:{
               printf("CMP_E");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_CMP_NE:{
               printf("CMP_NE");
               printVar(dst);
               printVar(src1);
               printVar(src2);
            }break;
         case I_JMP: {
               printf("JMP %d", tmp->dest);
            }break;
         case I_JMP_Z: {
               printf("JMP_Z");
               printVar(dst);
               printf(" %d", tmp->dest);
            }break;
         case I_JMP_NZ: {
               printf("JMP_NZ");
               printVar(dst);
               printf(" %d", tmp->dest);
            }break;
         case I_WRITE: {
               printf("WRITE");
               printVar(dst);
            }break;
         case I_READ: {
               printf("READ");
               printVar(dst);
            }break;

         case I_CALL: {
               printf("CALL");
               printf(" %s", ((TFunction *)tmp->dest)->name );
            }break;
         case I_TYPE:{
               printf("TYPE");
            }break;
         case I_SUBSTR:{
               printf("SUBSTR");
            }break;
         case I_FIND: {
               printf("FIND");
            }break;
         case I_SORT: {
               printf("SORT");
            }break;
      }

      printf("\n");
      listSucc(l);
   }
}

//             uzel     typ stromu       oddelovac
void printNode(TNode n, EBTreeDataType t, char *delim){
   if(n != NULL){
      printNode(n->left, t, delim);

      switch(t){
         case FUNCIONS:{
               printf("\n %s%s\n    {\n", delim, n->key);

               TFunction *tmp   = n->data;
               printf("     variables:\n");
                  printNode( tmp->variables.root, tmp->variables.type /*VAR*/, "         " );
               printf("     constants:\n");
                  listConstTmpPrint(&tmp->constants);
               printf("     tmp_var:\n");
                  listConstTmpPrint(&tmp->tmpVar);
               printf("     instruction:\n");
                  listInstrPrint(&tmp->instructions);

               printf("    }\n");
         }break;
         case VAR:{
            printf("%s%s\n", delim, n->key);
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
