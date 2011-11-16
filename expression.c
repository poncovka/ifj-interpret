/*
 * @description   Syntaktická analýza výrazù (zdola nahoru)
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "expression.h"

// precedenèní tabulka:
const char precedentTable[MAXTAB][MAXTAB] = {

// tokeny:                      tru fal nil num str id   (   )   +   -   *   /   ^  ..   <  <=   >  >=  ==  ~=             $ 
[KW_TRUE]          = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>',[ENDEXPR]='>'},
[KW_FALSE]         = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>',[ENDEXPR]='>'},
[KW_NIL]           = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>',[ENDEXPR]='>'},
[L_NUMBER]         = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_STRING]         = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_ID]             = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_LEFT_BRACKET]   = {[KW_TRUE]='<','<','<','<','<','<','<','=','<','<','<','<','<','<','<','<','<','<','<','<',[ENDEXPR]= 0 },
[L_RIGHT_BRACKET]  = {[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_ADDITION]       = {[KW_TRUE]= 0 , 0 , 0 ,'<', 0 ,'<','<','>','>','>','<','<','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_SUBTRACTION]    = {[KW_TRUE]= 0 , 0 , 0 ,'<', 0 ,'<','<','>','>','>','<','<','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_MULTIPLICATION] = {[KW_TRUE]= 0 , 0 , 0 ,'<', 0 ,'<','<','>','>','>','>','>','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_DIVISION]       = {[KW_TRUE]= 0 , 0 , 0 ,'<', 0 ,'<','<','>','>','>','>','>','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_POWER]          = {[KW_TRUE]= 0 , 0 , 0 ,'<', 0 ,'<','<','>','>','>','>','>','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_CONCATENATION]  = {[KW_TRUE]= 0 , 0 , 0 , 0 ,'<','<','<','>','<','<','<','<','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_SMALLER]        = {[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_SMALLER_EQUAL]  = {[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_BIGGER]         = {[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_BIGGER_EQUAL]   = {[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_EQUAL]          = {[KW_TRUE]='<','<','<','<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_UNEQUAL]        = {[KW_TRUE]='<','<','<','<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[ENDEXPR]          = {[KW_TRUE]='<','<','<','<','<','<','<', 0 ,'<','<','<','<','<','<','<','<','<','<','<','<',[ENDEXPR]='$'},
}; //precedentTable

// pom výpisy:

void tiskniStack (TStack *s) {

  printf("Stav zásobníku: top\n");
  TSItem *pom = s->top;
  while (pom != NULL) {
    printf("\t%d\n",((TStackData*)(pom->data))->token);
    pom = pom->next;
  }
  printf("_______bottom_________\n\n");
}

const char *iTable[]={
   [I_ADD]="+",      // dst src src       vsechno TVar
   [I_SUB]="-",      // dst src src
   [I_MUL]="*",      // dst src src
   [I_DIV]="/",      // dst src src
   [I_POW]="^",      // dst src src
   [I_CON]="..",      // dst src src

   [I_CMP_L]="<",    // dst src src
   [I_CMP_LE]="<=",   // dst src src
   [I_CMP_G]=">",    // dst src src
   [I_CMP_GE]=">=",   // dst src src
   [I_CMP_E]="==",    // dst src src
   [I_CMP_NE]="~=",   // dst src src
};

void tiskniList (TList *L) {

  printf("Stav seznamu:\n");

  TLItem *pom = L->First;
  TInstr *i = NULL;

  while (pom != NULL) {
    i = (TInstr*)(pom->data);

    if (((TVar*)i->dest)->name == NULL) printf("%d = ", (int)i->dest);
    else printf("%s = ",((TVar*)i->dest)->name);
    if (((TVar*)i->src1)->name == NULL) printf(" %d ", (int)i->src1);
    else printf(" %s ",((TVar*)i->src1)->name);
    printf(" %s ",iTable[i->type]);
    if (((TVar*)i->src2)->name == NULL) printf(" %d", (int)i->src2);
    else printf(" %s",((TVar*)i->src2)->name);
    printf("\n");
    pom = pom->next;
  }
  printf("________________\n\n");
}

void tiskniInst(){
  printf("\n instrukce:\n");
  for (int i = I_ADD; i<= I_CMP_NE; i++){ printf("%d\n",i);}
  printf("\n");
}



TStack Stack;

/////////////////////////////////////////////////////////////
int parseExpression(TTable *table) {
  int err = EOK;

  TList *LTmpVars = &table->lastAddedFunc->tmpVar;
  TList *LInstr = &table->lastAddedFunc->instructions;

  // inicializace
  stackInit(&Stack);
  listFirst(LTmpVars);


  shift(&Stack, ENDEXPR, NULL);
  tiskniStack(&Stack);

  int a ,b;
  char c;
  TInstr instr = {NOINSTR, NULL, NULL, NULL};

  do {

    a = token;                       // aktuální token
    b = getTopTerminal(&Stack);      // najdeme nejvrchnìj¹í terminál
    c = precedentTable[b][a];        // podíváme se do tabulky

    if (c == 0) {                    // chyba
      a = ENDEXPR;                   // token pova¾ujeme za konec výrazu
      c = precedentTable[b][a];      // znovu se podíváme do tabulky
    }

    switch(c) {
      case '=':
      case '<':  err = shift(&Stack, a, NULL);    // push(a)
                 if (err != EOK) break;
                 token = getNextToken(&attr);     // next token
                 if (token < 0) err = token; 
                 break;

      case '>':  err = findRule(&Stack, &instr);  // najdi pravidlo
                 if (err != EOK) break;
                                                  // generuj instrukci
                 err = generateInstr(LInstr, &instr, LTmpVars);
                 if (err != EOK) break;
                                                  // push(výsledek)
                 err = shift(&Stack, EXPRESSION, instr.dest); 
                 break;

      case '$':  break;
      default :  err = SYN_ERR;                   // syntaktická chyba
    };

    printf("\n a = %d b = %d c=%c  err = %d \n\n",a, b, c,err);
    tiskniStack(&Stack);
  }while ((a != ENDEXPR || b != ENDEXPR) && err == EOK);

  // kontrolni vypis:
  tiskniList(LInstr);

  // uklid
  stackDataDelete(&Stack);
  stackDelete(&Stack);
  return err;
}
//////////////////////////////////////////////////////////////////
int shift (TStack *S, int token, TVar *pom) {

  int err = EOK;

  // inicialiazce dat
  if (isId(token)) {
    pom = functionSearchVar(table.lastAddedFunc, attr);
    if (pom == NULL) err = SEM_ERR; // nedefinovaná promìnná */
    token = EXPRESSION; // pravidlo E->id
  }
  else if (isConst(token)) {
/*  err = functionInsertConstatnt(table.lastAddedFunc, &attr);
    if (err == EOK) {
      TVar *pom = getLastAddedConst(TFunction*);
    }
    else err = INTR_ERR; // nedostatek pamìti */
    token = EXPRESSION; // pravidlo E->const
  }

  // vlo¾íme data na zásobník
  if (err == EOK){
    TStackData *data = createStackData(token, pom, &err);
    if (err == EOK) err = stackPush(S, data);
  }
  else err = INTR_ERR; // nedostatek pamìti

  return EOK;
}
//////////////////////////////////////////////////////////////////
int findRule(TStack *S, TInstr *instr) {

  int err = SYN_ERR;
  TStackData *top = NULL;

  if (!stackEmpty(S)){
    top = (TStackData *) stackTopPop(S);

    // pravidlo E -> (E)
    if (top->token == L_RIGHT_BRACKET) {         // pop(pravá závorka)
      instr->type = NOINSTR;                     // nebude se generovat pravidlo
      free(top);
      if (!stackEmpty(S)) {
        top = (TStackData *) stackTopPop(S);  

        if (top->token == EXPRESSION) {          // pop(výraz)
          instr->dest = top->var;
          free(top);
          if (!stackEmpty(S)){
            top = (TStackData *) stackTopPop(S);

            if (top->token == L_LEFT_BRACKET) {  // pop(levá závorka)
              err = EOK;                         // nalezeno pravidlo!
              free(top);
            }
          }
        }
      }
    } // konec pravidla

    // pravidlo E -> E op E
    else if (top->token == EXPRESSION) {        // pop(výraz)
      instr->src2 = top->var;
      free(top);
      if (!stackEmpty(S)) {
        top = (TStackData *) stackTopPop(S);

        if (isOperator(top->token)) {           // pop(operátor) 
          instr->type = top->token + OFFSET;
          free(top);
          if (!stackEmpty(S)) {
            top = (TStackData *) stackTopPop(S);

            if (top->token == EXPRESSION) {    // pop(výraz)
              err = EOK;                       // nalezeno pravidlo!
              instr->src1 = top->var;
              free(top);      
            }
          }
        }
      }
    } // konec pravidla
  }

  // pokud nìkde zhavarovalo
  if (err != EOK && top != NULL) {
    free(top);
  }

  return err;
}
//////////////////////////////////////////////////////////////////
int generateInstr(TList *LInstr, TInstr *instr, TList *LTmpVars) {

  // pravidlo E -> (E)
  if (instr->type == NOINSTR) return EOK;

  // pravidlo E -> E op E
  int err = EOK;
 
  // vytvoø pomocnou promìnnou pro výsledek operace
  TVar *var = createTmpVar(LTmpVars, &err);
  if (err == EOK) {
    instr->dest = var;
    
    // vytvoø novou instrukci a zkopíruj do ní starou
    TInstr *newInstr = NULL;
    if ( (newInstr = (TInstr *)malloc(sizeof(TInstr))) != NULL) {
      newInstr->type = instr->type;
      newInstr->dest = instr->dest;
      newInstr->src1 = instr->src1;
      newInstr->src2 = instr->src2;

      err = listInsertLast(LInstr, newInstr);
    }
    else err = INTR_ERR; // nedostatek pamìti
  }

  return err;
}
//////////////////////////////////////////////////////////////////
int getTopTerminal(TStack *S) {

  // pøedpokládáme, ¾e pøed voláním fce jsme na zás. vlo¾ili $
  TSItem *pom = S->top;

  // hledáme terminál nejblí¾e k zásobníku: 
  while (pom != NULL && ((TStackData*)pom->data)->token == EXPRESSION) {
    pom = pom->next;
  }
  return ((TStackData*)pom->data)->token;

}

//////////////////////////////////////////////////////////////////
TStackData *createStackData(int token, TVar *var, int *err) {

  TStackData *data = NULL;
  if ( (data = (TStackData*)malloc(sizeof(TStackData))) != NULL ){
    data->token = token;
    data->var = var;
  }
  else *err = INTR_ERR; // nedostatek pamìti

  return data;
}


//////////////////////////////////////////////////////////////////
TVar *createTmpVar(TList *L, int *err) {

  // posuneme aktivitu na dal¹í prvek seznamu :
  listSucc(L);

  // aktivita se ztratila, je tøeba vytvoøit novou promìnnou
  if (!listActive(L)) {

    // alokace promìnné a dat
    TVar *pom = NULL;
    if ( (pom = malloc(sizeof(TVar))) != NULL) {
       if ( (pom->var = (TVarData*)malloc(sizeof(TVarData))) != NULL ) {

         pom->varType = VT_TMP_VAR;             // inicializace
         pom->name = NULL;
         pom->var->type = NIL;
         *err = listInsertLast(L, pom);    // vlo¾ení do seznamu
         listLast(L);                      // stane se aktivním
       }
       else {
         *err = INTR_ERR;                  // chyba pøi alokaci
          free(pom);
       }
    }
    else *err = INTR_ERR;
  }

  //  vrátí ukazatel na promìnnou nebo NULL
  if (*err == EOK){
    return (TVar*)listGetActive(L)->data;
  }
  else return NULL;
}


////////////////////////////////////////////////////////
void listDataDelete(TList *L) {

  listFirst(L);
  while(listActive(L)) {
    free(((TVar*)listGetActive(L)->data)->var);
    free(((TVar*)listGetActive(L)->data));
    listSucc(L);
  }

}

///////////////////////////////////////////////////////
void stackDataDelete(TStack *S) {

    TSItem *pom = S->top;
    while (pom != NULL) {
      free (pom->data);
      pom = pom->next;
    }
}

//////////////////////////////////////////////////////
void tiskniPrecTab() {
  printf("%d %d", MAXTAB, ENDEXPR);
  printf("\n");
  for (int i = 0; i<MAXTAB; i++){
    for (int j = 0; j<MAXTAB; j++){
      if (precedentTable[i][j] == 0) printf("0 ");
      else printf("%c ",precedentTable[i][j]);
    }
  printf("\n");
  }
}

/* konec expression.c */
