#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "table.h"
#include "list.h"
#include "scanner.h"
#include "str.h"

#define NEXT_TOKEN lex = getNextToken(&token); if(lex < 0) return lex;

string token;
int    lex;
TTable *table;

int prsProgram();
int prsDefFunc();
int prsStat();
int prsParams();
int prsParamsN();
int prsDefVar();
int prsStatList();
int prsInit();
int prsLit();
int prsCommand();
int prsExpN();
int prsAssign(TVar*);
int prsVarParams();
int prsVar();
int prsVarN();


int parser(TTable *t){
   table = t;
   strInit(&token);

   int err = prsProgram();
   strFree(&token);
   return err;
}

// <program>
int prsProgram(){
   // 1. <program> -> function <def_func>
   NEXT_TOKEN
   if(lex != KW_FUNCTION) return SYN_ERR;

   return prsDefFunc();
}

// <def_func>
int prsDefFunc(){
   int err;

   NEXT_TOKEN
   switch(lex){
      case KW_MAIN:{
         // 2. <def_func> -> main ( ) <stat> end ; <EOF>
         if( (err = tableInsertFunction(table, token) ) == INS_NODE_EXIST)
            return SEM_ERR;   // tato fce uz v tabulce je (NEMELA BY BYT!!!)
         else if(err != INS_OK)
            return INTR_ERR;  // nepodarilo se vlozit

         // DEBUG
         printf("Instrukce \"%s\"\n", table->lastAddedFunc->name);

         // povedlo vlozit
         NEXT_TOKEN
         if(lex != L_LEFT_BRACKET) return SYN_ERR;

         NEXT_TOKEN
         if(lex != L_RIGHT_BRACKET) return SYN_ERR;

         printf("\tSTACK_E\n");

         err = prsStat();
         if(err != PRS_OK) return err;

         // token uz nacetl prsStat
         if(lex != KW_END) return SYN_ERR;

         NEXT_TOKEN
         if(lex != L_SEMICOLON) return SYN_ERR;

         NEXT_TOKEN;
         if(lex != END_OF_FILE) return SYN_ERR;

         return PRS_OK;
      }break;
      case L_ID: {
         // 3. <def_func> -> idFunc ( <params> ) <stat> end <program>
         if( (err = tableInsertFunction(table, token) ) == INS_NODE_EXIST)
            return SEM_ERR;   // tato fce uz v tabulce je
         else if(err != INS_OK)
            return INTR_ERR;  // nepodarilo se vlozit

         // DEBUG
         printf("Instrukce \"%s\"\n", table->lastAddedFunc->name);

         // povedlo vlozit
         NEXT_TOKEN
         if(lex != L_LEFT_BRACKET) return SYN_ERR;

         err = prsParams();
         if(err != PRS_OK) return err;

         printf("\tSTACK_E\n");

         // dalsi token uz nacetl prsParams
         if(lex != L_RIGHT_BRACKET) return SYN_ERR;

         err = prsStat();
         if(err != PRS_OK) return err;

         // dalsi token uz nacetl prsStat
         if(lex != KW_END) return SYN_ERR;

         return prsProgram();
      }break;
   }
   return SEM_ERR;
}

int prsParams(){
   int err;
   NEXT_TOKEN
   // 4. <params> -> eps
   if(lex == L_RIGHT_BRACKET) return PRS_OK;

   // 5. <params> -> id <params_n>
   if(lex != L_ID) return SYN_ERR;

   // jestli se id jmenuje stejne jako nejaka funkce SEM_ERR
   if(tableSearchFunction(table, token) != NULL) return SEM_ERR;
   // pokusim se id vlozit to tabulky
   err = functionInsertVar(table->lastAddedFunc, token);
   if(err == INS_NODE_EXIST) return SEM_ERR;
   if(err != INS_OK) return INTR_ERR;

   printf("\tSET %s\n", token.str);
   printf("\tPOP %s\n", token.str);

   err = prsParamsN();
   if(err != PRS_OK) return err;

   return PRS_OK;
}

int prsParamsN(){
   int err;

   NEXT_TOKEN
   // 6. <params_n> -> eps
   if(lex == L_RIGHT_BRACKET) return PRS_OK;

   // 7. <params_n> -> , id <params_n>
   if(lex != L_COMMA) return SYN_ERR;

   NEXT_TOKEN
   if(lex != L_ID) return SYN_ERR;

   // jestli se id jmenuje stejne jako nejaka funkce SEM_ERR
   if(tableSearchFunction(table, token) != NULL) return SEM_ERR;
   // pokusim se id vlozit to tabulky
   err = functionInsertVar(table->lastAddedFunc, token);
   if(err == INS_NODE_EXIST) return SEM_ERR;
   if(err != INS_OK) return INTR_ERR;

   printf("\tSET %s\n", token.str);
   printf("\tPOP %s\n", token.str);

   return prsParamsN();
}

int prsStat(){
   // 8. <stat> -> <def_var> <stat_list>
   int err;
   err = prsDefVar();
   if(err != PRS_OK) return err;

   err = prsStatList();
   if(err != PRS_OK) return err;

   return PRS_OK;
}

int prsDefVar(){
   int err;

   NEXT_TOKEN
   // 9. <def_var> -> eps
   if(lex == KW_END   || lex == L_ID      || lex == KW_IF   ||
      lex == KW_WHILE || lex == KW_RETURN || lex == KW_WRITE )
      return PRS_OK;

   // 10. <def_var> -> local id <INIT> ; <def_var>
   if(lex != KW_LOCAL) return SYN_ERR;

   NEXT_TOKEN
   if(lex != L_ID) return SYN_ERR;
   // prohledam jestli se nejaka promena nejmenuje stejne jako nejaka funkce
   if(tableSearchFunction(table, token) != NULL) return SEM_ERR;
   // pokusim se id vlozit to tabulky
   err = functionInsertVar(table->lastAddedFunc, token);
   if(err == INS_NODE_EXIST) return SEM_ERR;
   if(err != INS_OK) return INTR_ERR;

   err = prsInit();
   if(err != PRS_OK) return err;

   if(lex != L_SEMICOLON) return SYN_ERR;

   return prsDefVar();
}

int prsInit(){
   int err;
   NEXT_TOKEN
   // 11. <init> -> eps
   if(lex == L_SEMICOLON){
      // inicializuji promenou na nil
      // ve skutecnosti ji inicializuju pomoci konstanty ktera bude ulozena
      // jako prvni v tabulce konstant a bude NIL
      printf("\tSET %s\n", getLastAddedVar(table->lastAddedFunc)->name );
      return PRS_OK;
   }
   // 12. <init> -> = <lit>
   if(lex != L_ASSIGN)  return SYN_ERR;

   NEXT_TOKEN
   err = prsLit();
   if(err != PRS_OK) return err;

      if(lex == L_STRING)
         printf("\tSET %s \"%s\"\n", getLastAddedVar(table->lastAddedFunc)->name, token.str);
      else
         printf("\tSET %s %s\n", getLastAddedVar(table->lastAddedFunc)->name, token.str);

   // musim nacist dalsi token protoze prsDefVar pocita ze je nacteny
   NEXT_TOKEN
   return PRS_OK;
}

int prsLit(){
   // 13. <lit> -> literal // nejaky z literalu
   if( lex == KW_NIL || lex == KW_FALSE || lex == KW_TRUE || lex == L_NUMBER || lex == L_STRING )
         return PRS_OK;
   return SYN_ERR;
}

int prsStatList(){
   // TOKEN UZ JE NACTENY
   int err;
   // 14. <stat_list> -> eps
   if(lex == KW_END || lex == KW_ELSE) return PRS_OK;

   // 15. <stat_list> -> <commad> ; <stat_list>
   if(lex != L_ID    && lex != KW_IF   && lex != KW_WHILE   && lex != KW_RETURN  && lex != KW_WRITE )
      return SYN_ERR;

   err = prsCommand();
   if(err != PRS_OK) return err;

   // token uz je nacteny NEXT_TOKEN
   if(lex != L_SEMICOLON) return SYN_ERR;

   NEXT_TOKEN // <stat_list> ceka uz nacteny token
   return prsStatList();
}

int expJump(){
   //int i = 1;
   printf("\t---\n\texp magic\n\t---\n");
   while(lex != END_OF_FILE){
      if(lex == L_COMMA  || lex == L_SEMICOLON || lex == L_RIGHT_BRACKET || lex == KW_THEN)
         break;
      NEXT_TOKEN
   }
   return 1;
}

int cnt = 0;
int prsCommand(){
   // TOKEN UZ JE NACTENY
   switch(lex){
      // 22. <command> -> id = <assign>
      case L_ID:{
         // je id v tabulce symbolu pro tuhle funkci?
         TVar *tmp;
         if( (tmp = functionSearchVar(table->lastAddedFunc, token) ) == NULL ) return SEM_ERR;
         NEXT_TOKEN
         if(lex != L_ASSIGN) return SYN_ERR;

         int err = prsAssign(tmp);
         if(err != PRS_OK) return err;

         return PRS_OK;
      }break;
      // 16. <command> -> if expression then <stat_list> else <stat_list> end
      case KW_IF:{
         int err;
         NEXT_TOKEN
         expJump();
         if(lex != KW_THEN) return SYN_ERR;

         int tmp = ++cnt;
         printf("\tJMP_Z tmp else_%d\n", tmp);
         err = prsStat();
         printf("\tJMP if_end_%d\n", tmp);
         if(err != PRS_OK) return err;

         if(lex != KW_ELSE) return SYN_ERR;

         printf("\tLAB else_%d\n", tmp);
         err = prsStat();
         if(err != PRS_OK) return err;

         if(lex != KW_END) return SYN_ERR;
         printf("\tLAB if_end_%d\n", tmp);
         NEXT_TOKEN
         return PRS_OK;
      }break;
      // 17. <command> -> while expression then <stat_list> end
      case KW_WHILE:{
         int tmp = ++cnt;
         printf("\tLAB while_%d\n", tmp);
         NEXT_TOKEN
         expJump();
         printf("\tJMP_Z tmp while_end_%d\n", tmp);
         if(lex != KW_THEN) return SYN_ERR;

         int err = prsStat();
         if(err != PRS_OK) return err;

         if(lex != KW_END) return SYN_ERR;
         printf("\tJMP while_%d\n",tmp);
         printf("\tLAB while_end_%d\n",tmp);
         NEXT_TOKEN
         return PRS_OK;
      }break;
      // 18. <command> -> return expression
      case KW_RETURN:{
         // preskocim vyraz a vratim ze bylo vse OK
         NEXT_TOKEN
         expJump();
         printf("\tPOP tmp\n");
         printf("\tRETURN\n");
         return PRS_OK;
      }break;
      // 19. <command> -> write ( expression <expression_n> )
      case KW_WRITE:{
         int err;
         NEXT_TOKEN
         if(lex != L_LEFT_BRACKET) return SYN_ERR;

         NEXT_TOKEN
         expJump();
         printf("\tWRITE tmp\n");

         err = prsExpN();
         if(err != PRS_OK ) return err;

         if(lex != L_RIGHT_BRACKET) return SYN_ERR;

         NEXT_TOKEN
         return PRS_OK;
      }break;
   }
   return SYN_ERR;
}

int prsExpN(){
   // 20. <expression_n> -> eps
   if(lex == L_RIGHT_BRACKET) return PRS_OK;

   // 21. <expression_n> -> , expression <expression_n>
   if(lex != L_COMMA) return SYN_ERR;
   NEXT_TOKEN
   expJump();
   printf("\tWRITE tmp\n");

   return prsExpN();
}

int prsAssign(TVar *var){

   NEXT_TOKEN
   if(lex == KW_READ){
      // 24. <assign> -> read ( <lit> )
      NEXT_TOKEN
      if(lex != L_LEFT_BRACKET) return SYN_ERR;

      NEXT_TOKEN
      if(lex != L_STRING && lex != L_NUMBER ) return SYN_ERR;

      printf("\tREAD %s \"%s\"\n",var->name, token.str);

      NEXT_TOKEN
      if(lex != L_RIGHT_BRACKET) return SYN_ERR;

      NEXT_TOKEN
      return PRS_OK;
   }

   int lexTmp = lex; // ulozim si token abych potom vedel jakou instrukci generovat
   TFunction *Ftmp = tableSearchFunction(table, token);

   if(Ftmp == NULL && (lex != KW_TYPE && lex != KW_SUBSTR && lex != KW_FIND && lex != KW_SORT) ){
      // 23. <assign> -> expression
      expJump();
      printf("\tMOV %s tmp\n", var->name);
      return PRS_OK;
   }

   // 25. <assign> -> idFunc ( <params> )
   char *tmp = NULL;
   if(lex == L_ID)
      tmp = Ftmp->name;

   NEXT_TOKEN
   if(lex != L_LEFT_BRACKET) return SYN_ERR;

   int err = prsVarParams();
   if(err != PRS_OK) return err;

   if(lex != L_RIGHT_BRACKET) return SYN_ERR;

   switch(lexTmp){
      case L_ID:{
            printf("\tCALL %s\n", tmp);
         }break;
      case KW_TYPE:{
            printf("\tTYPE\n");
         }break;
      case KW_SUBSTR:{
            printf("\tSUBSTR\n");
         }break;
      case KW_FIND:{
            printf("\tFIND\n");
         }break;
      case KW_SORT:{
            printf("\tSORT\n");
         }break;
   }

   printf("\tPOP %s\n", var->name);

   NEXT_TOKEN
   return PRS_OK;
}

int prsVarParams(){
   NEXT_TOKEN
   // 26. <var_params> -> eps
   if(lex == L_RIGHT_BRACKET) return PRS_OK;

   // 27. <var_params> -> <var> <var_n>
   printf("\t---\n");
   int err = prsVar();
   if(err != PRS_OK) return err;

   err = prsVarN();
   if(err != PRS_OK) return err;

   printf("\t---\n");

   if(lex == L_RIGHT_BRACKET) return PRS_OK;

   return SYN_ERR;
}

int prsVar(){
   // 28. <var> -> <lit>
   int err = prsLit();
   if(err == PRS_OK){
      // konstantka
      if(lex == L_STRING)
         printf("\tPUSH \"%s\"\n", token.str);
      else
         printf("\tPUSH %s\n", token.str);
      return PRS_OK;
   }
   if(err == SYN_ERR){
      // 29. <var> -> id
      if(lex != L_ID) return SYN_ERR;
      TVar *tmp = functionSearchVar(table->lastAddedFunc, token);
      if(tmp == NULL)   return SEM_ERR;
      printf("\tPUSH %s\n", tmp->name);

      return PRS_OK;
   }
   return err;
}

int prsVarN(){
   NEXT_TOKEN
   // 30. <var_n> -> eps
   if(lex == L_RIGHT_BRACKET) return PRS_OK;
   // 31. <var_n> -> , <var> <var_n>
   if(lex != L_COMMA) return SYN_ERR;

   NEXT_TOKEN
   int err = prsVar();
   if(err != PRS_OK) return err;

   err = prsVarN();
   if(err != PRS_OK) return err;

   return PRS_OK;
}
