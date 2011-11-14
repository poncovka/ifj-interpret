/*
 * @description   Syntaktick� anal�za v�raz� (zdola nahoru)
 * @author        Vendula Poncov� - xponco00
 * @projekt       IFJ11
 * @date
 */

#include "expression.h"

// preceden�n� tabulka:
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
    a = lex;                         // na�teneme token
    b = stackTopTerminal(&Stack);    // najdeme nejvrchn�j�� termin�l
    c = precedentTable[b][a];        // pod�v�me se do tabulky

    if (c == 0) {                    // chyba
      a = ENDEXPR;                   // token pova�ujeme za konec v�razu
      c = precedentTable[b][a];      // znovu se pod�v�me do tabulky
    } 

    switch(c) {
      case '=':  //push(a)
                 //next token
                 break;
      case '<':  //zam�� v z�sobn�ku b za b<
                 //push(a)
                 //next token
                 break;
      case '>':  // vyber ze zasobniku v�e po < v�etn�
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

  // p�edpokl�d�me, �e p�ed vol�n�m fce jsme na z�s. vlo�ili $
  TSItem *pom = S->top;

  // hled�me termin�l nejbl�e k z�sobn�ku: 
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

  // posuneme aktivitu na dal�� prvek seznamu :
  listSucc(L);

  // aktivita se ztratila, je t�eba vytvo�it novou prom�nnou
  if (!listActive(L)) {

    // alokace prom�nn� a dat
    TVar *pom = NULL;
    if ( (pom = malloc(sizeof(TVar))) != NULL) {
       if ( (pom->var = (TVarData*)malloc(sizeof(TVarData))) != NULL ) {

         pom->varType = VT_TMP_VAR;             // inicializace
         pom->var->type = NIL;
         *err = listInsertLast(L, pom);    // vlo�en� do seznamu
         listLast(L);                      // stane se aktivn�m
       }
       else {
         *err = INTR_ERR;                  // chyba p�i alokaci
          free(pom);
       }
    }
    else *err = INTR_ERR;
  }

  //  vr�t� ukazatel na prom�nnou nebo NULL
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
