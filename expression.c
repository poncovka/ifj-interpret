/*
 * @description   Syntaktická analýza výrazù (zdola nahoru)
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "expression.h"

// precedenèní tabulka:
const char precedentTable[MAXTAB][MAXTAB] = {

// tokeny:                    tru fal nil id  num str  (   )   ^   *   /   +   -   ..  <   <=  >   >=  ~=  ==            $
[KW_TRUE]         ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>',[ENDEXPR]='>'},
[KW_FALSE]        ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>',[ENDEXPR]='>'},
[KW_NIL]          ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>',[ENDEXPR]='>'},
[L_ID]            ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_NUMBER]        ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_STRING]        ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>', 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_LEFT_BRACKET]  ={[KW_TRUE]='<','<','<','<','<','<','<','=','<','<','<','<','<','<','<','<','<','<','<','<',[ENDEXPR]= 0 },
[L_RIGHT_BRACKET] ={[KW_TRUE]= 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_POWER]         ={[KW_TRUE]= 0 , 0 , 0 ,'<','<', 0 ,'<','>','<','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_MULTIPLICATION]={[KW_TRUE]= 0 , 0 , 0 ,'<','<', 0 ,'<','>','<','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_DIVISION]      ={[KW_TRUE]= 0 , 0 , 0 ,'<','<', 0 ,'<','>','<','>','>','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_ADDITION]      ={[KW_TRUE]= 0 , 0 , 0 ,'<','<', 0 ,'<','>','<','<','<','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_SUBTRACTION]   ={[KW_TRUE]= 0 , 0 , 0 ,'<','<', 0 ,'<','>','<','<','<','>','>','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_CONCATENATION] ={[KW_TRUE]= 0 , 0 , 0 ,'<', 0 ,'<','<','>','<','<','<','<','<','>','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_SMALLER]       ={[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_SMALLER_EQUAL] ={[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_BIGGER]        ={[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_BIGGER_EQUAL]  ={[KW_TRUE]= 0 , 0 , 0 ,'<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_UNEQUAL]       ={[KW_TRUE]='<','<','<','<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[L_EQUAL]         ={[KW_TRUE]='<','<','<','<','<','<','<','>','<','<','<','<','<','<','>','>','>','>','>','>',[ENDEXPR]='>'},
[ENDEXPR]         ={[KW_TRUE]='<','<','<','<','<','<','<', 0 ,'<','<','<','<','<','<','<','<','<','<','<','<',[ENDEXPR]= 0 },

}; //precedentTable


TList LTmpVars;
TStack Stack;

/////////////////////////////////////////////////////////////
int parseExpression() {
  int err = EOK;

  // inicializace
  stackInit(&Stack);
  listInit(&LTmpVars);

  TStackData *pom = createStackData(0 ,ENDEXPR, NULL, &err);
  err = stackPush(&Stack, pom);

  int a,b;
  char c;

  do {
    a = lex;                         // naèteneme token
    b = stackTopTerminal(&Stack);    // najdeme nejvrchnìj¹í terminál
    c = precedentTable[b][a];        // podíváme se do tabulky

    if (c == 0) {                    // chyba
      a = ENDEXPR;                   // token pova¾ujeme za konec výrazu
      c = precedentTable[b][a];      // znovu se podíváme do tabulky
    } 

    switch(c) {
      case '=':  //push(a)
                 //next token
                 break;
      case '<':  //zamìò v zásobníku b za b<
                 //push(a)
                 //next token
                 break;
      case '>':  // vyber ze zasobniku v¹e po < vèetnì
                 // najdi pravidlo r:A->y
                 // generuj instrukci
                 // push(A)
                 // jinak chyba
                 break;
      default :  err = SYN_ERR;
    };

  } while(0 && err == EOK);

  // uklid
  listDataDelete(&LTmpVars);
  listDispose(&LTmpVars);
  stackDataDelete(&Stack);
  stackDelete(&Stack);
  return err;
}

//////////////////////////////////////////////////////////////////
int stackTopTerminal(TStack *S) {

  // pøedpokládáme, ¾e pøed voláním fce jsme na zás. vlo¾ili $
  TSItem *pom = S->top;

  // hledáme terminál nejblí¾e k zásobníku: 
  while (pom != NULL && ((TStackData*)pom->data)->token == NONTOKEN) {
    pom = pom->next;
  }
  return ((TStackData*)pom->data)->token;

}

//////////////////////////////////////////////////////////////////
TStackData *createStackData(char c, int token, TVar *var, int *err) {

  TStackData *data = NULL;
  if ( (data = (TStackData*)malloc(sizeof(TStackData))) != NULL ){
    data->c = c;
    data->token = token;
    data->var = var;
  }
  else *err = INTR_ERR;

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
