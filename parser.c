#include "parser.h"

#define NEXT_TOKEN token = getNextToken(&attr); if(token < 0) return token;

string attr;
int    token;
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
int prsAssign(TVar*); // promenou predavam kvuli generovani instrukci
int prsVarParams();
int prsVar();
int prsVarN();

int parser(TTable *t){
   table = t;
   strInit(&attr);

   int err = prsProgram();
   strFree(&attr);
   return err;
}

// <program>
int prsProgram(){
   // 1. <program> -> function <def_func>
   NEXT_TOKEN
   if(token != KW_FUNCTION) return SYN_ERR;

   return prsDefFunc();
}

TList *instr; // zde je ulozen seznam instrukci naposled vlozene funkce
// <def_func>
int prsDefFunc(){
   int err;

   NEXT_TOKEN
   switch(token){
      case KW_MAIN:{
         // 2. <def_func> -> main ( ) <stat> end ; <EOF>
         if( (err = tableInsertFunction(table, attr) ) == INS_NODE_EXIST)
            return SEM_ERR;   // tato fce uz v tabulce je (NEMELA BY BYT!!!)
         else if(err != INS_OK)
            return INTR_ERR;  // nepodarilo se vlozit

         // povedlo vlozit
         instr = &(table->lastAddedFunc->instructions);
         NEXT_TOKEN
         if(token != L_LEFT_BRACKET) return SYN_ERR;

         NEXT_TOKEN
         if(token != L_RIGHT_BRACKET) return SYN_ERR;

         // vlozim instrukci na vyprazdeni zasobniku protoze main muze byt volany rekuzivne
         // a nejaky sikula by mainu mohl dta parametry
         if( listInsertLast( instr, genInstr(I_STACK_E, NULL, NULL, NULL) ) != LIST_EOK)
            return INTR_ERR;

         err = prsStat();
         if(err != PRS_OK) return err;

         // attr uz nacetl prsStat
         if(token != KW_END) return SYN_ERR;

         NEXT_TOKEN
         if(token != L_SEMICOLON) return SYN_ERR;

         NEXT_TOKEN;
         if(token != END_OF_FILE) return SYN_ERR;

         return PRS_OK;
      }break;
      case L_ID: {
         // 3. <def_func> -> idFunc ( <params> ) <stat> end <program>
         if( (err = tableInsertFunction(table, attr) ) == INS_NODE_EXIST)
            return SEM_ERR;   // tato fce uz v tabulce je
         else if(err != INS_OK)
            return INTR_ERR;  // nepodarilo se vlozit

         // povedlo vlozit
         instr = &(table->lastAddedFunc->instructions);

         NEXT_TOKEN
         if(token != L_LEFT_BRACKET) return SYN_ERR;

         err = prsParams();
         if(err != PRS_OK) return err;

         // vyprazdnim zasobnim kdyby mi nahodou nekdo predal funkci vic parametru nez ocekava
         if( listInsertLast( instr, genInstr(I_STACK_E, NULL, NULL, NULL) ) != LIST_EOK)
            return INTR_ERR;

         // dalsi attr uz nacetl prsParams
         if(token != L_RIGHT_BRACKET) return SYN_ERR;

         err = prsStat();
         if(err != PRS_OK) return err;

         // dalsi attr uz nacetl prsStat
         if(token != KW_END) return SYN_ERR;

         return prsProgram();
      }break;
   }
   return SEM_ERR;
}

int prsParams(){
   int err;
   NEXT_TOKEN
   // 4. <params> -> eps
   if(token == L_RIGHT_BRACKET) return PRS_OK;

   // 5. <params> -> id <params_n>
   if(token != L_ID) return SYN_ERR;

   // jestli se id jmenuje stejne jako nejaka funkce SEM_ERR
   if(tableSearchFunction(table, attr) != NULL) return SEM_ERR;
   // pokusim se id vlozit to tabulky
   err = functionInsertVar(table->lastAddedFunc, attr);
   if(err == INS_NODE_EXIST) return SEM_ERR;
   if(err != INS_OK) return INTR_ERR;

   if( listInsertLast( instr, genInstr(I_SET, getLastAddedVar(table->lastAddedFunc), NULL, NULL) ) != LIST_EOK)
      return INTR_ERR;

   if( listInsertLast( instr, genInstr(I_POP, getLastAddedVar(table->lastAddedFunc), NULL, NULL) ) != LIST_EOK)
      return INTR_ERR;

   err = prsParamsN();
   if(err != PRS_OK) return err;

   return PRS_OK;
}

int prsParamsN(){
   int err;

   NEXT_TOKEN
   // 6. <params_n> -> eps
   if(token == L_RIGHT_BRACKET) return PRS_OK;

   // 7. <params_n> -> , id <params_n>
   if(token != L_COMMA) return SYN_ERR;

   NEXT_TOKEN
   if(token != L_ID) return SYN_ERR;

   // jestli se id jmenuje stejne jako nejaka funkce SEM_ERR
   if(tableSearchFunction(table, attr) != NULL) return SEM_ERR;
   // pokusim se id vlozit to tabulky
   err = functionInsertVar(table->lastAddedFunc, attr);
   if(err == INS_NODE_EXIST) return SEM_ERR;
   if(err != INS_OK) return INTR_ERR;

   if( listInsertLast( instr, genInstr(I_SET, getLastAddedVar(table->lastAddedFunc), NULL, NULL) ) != LIST_EOK)
      return INTR_ERR;

   if( listInsertLast( instr, genInstr(I_POP, getLastAddedVar(table->lastAddedFunc), NULL, NULL) ) != LIST_EOK)
      return INTR_ERR;

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
   if(token == KW_END   || token == L_ID      || token == KW_IF   ||
      token == KW_WHILE || token == KW_RETURN || token == KW_WRITE )
      return PRS_OK;

   // 10. <def_var> -> local id <INIT> ; <def_var>
   if(token != KW_LOCAL) return SYN_ERR;

   NEXT_TOKEN
   if(token != L_ID) return SYN_ERR;
   // prohledam jestli se nejaka promena nejmenuje stejne jako nejaka funkce
   if(tableSearchFunction(table, attr) != NULL) return SEM_ERR;
   // pokusim se id vlozit to tabulky
   err = functionInsertVar(table->lastAddedFunc, attr);
   if(err == INS_NODE_EXIST) return SEM_ERR;
   if(err != INS_OK) return INTR_ERR;

   err = prsInit();
   if(err != PRS_OK) return err;

   if(token != L_SEMICOLON) return SYN_ERR;

   return prsDefVar();
}

int prsInit(){
   int err;
   NEXT_TOKEN
   // 11. <init> -> eps
   if(token == L_SEMICOLON){
      // inicializuji promenou na nil
      // ve skutecnosti ji inicializuju pomoci konstanty ktera bude ulozena
      // jako prvni v tabulce konstant a bude NIL
      if( listInsertLast( instr, genInstr(I_SET, getLastAddedVar(table->lastAddedFunc), NULL, NULL) ) != LIST_EOK)
         return INTR_ERR;
      return PRS_OK;
   }
   // 12. <init> -> = <lit>
   if(token != L_ASSIGN)  return SYN_ERR;

   NEXT_TOKEN
   err = prsLit();
   if(err != PRS_OK) return err;

   // vytvorim konstantu
   if(functionInsertConstatnt(table->lastAddedFunc, attr, token) != INS_OK)
      return INTR_ERR;

   // vlozim instrukci
   TVar *src = table->lastAddedFunc->constants.Last->data;
   if( listInsertLast( instr, genInstr(I_SET, getLastAddedVar(table->lastAddedFunc), src, NULL) ) != LIST_EOK)
      return INTR_ERR;

   // musim nacist dalsi attr protoze prsDefVar pocita ze je nacteny
   NEXT_TOKEN
   return PRS_OK;
}

int prsLit(){
   // 13. <lit> -> literal // nejaky z literalu
   if( token == KW_NIL || token == KW_FALSE || token == KW_TRUE || token == L_NUMBER || token == L_STRING )
         return PRS_OK;
   return SYN_ERR;
}

int prsStatList(){
   // TOKEN UZ JE NACTENY
   int err;
   // 14. <stat_list> -> eps
   if(token == KW_END || token == KW_ELSE) return PRS_OK;

   // 15. <stat_list> -> <commad> ; <stat_list>
   if(token != L_ID    && token != KW_IF   && token != KW_WHILE   && token != KW_RETURN  && token != KW_WRITE )
      return SYN_ERR;

   err = prsCommand();
   if(err != PRS_OK) return err;

   // attr uz je nacteny NEXT_TOKEN
   if(token != L_SEMICOLON) return SYN_ERR;

   NEXT_TOKEN // <stat_list> ceka uz nacteny attr
   return prsStatList();
}

int expJump(){
   //int i = 1;
   printf("\t---\n\texp magic\n\t---\n");
   while(token != END_OF_FILE){
      if(token == L_COMMA  || token == L_SEMICOLON || token == L_RIGHT_BRACKET || token == KW_THEN)
         break;
      NEXT_TOKEN
   }
   return 1;
}

int cnt = 0;
int prsCommand(){
   // TOKEN UZ JE NACTENY
   switch(token){
      // 22. <command> -> id = <assign>
      case L_ID:{
         // je id v tabulce symbolu pro tuhle funkci?
         TVar *tmp;
         if( (tmp = functionSearchVar(table->lastAddedFunc, attr) ) == NULL ) return SEM_ERR;
         NEXT_TOKEN
         if(token != L_ASSIGN) return SYN_ERR;

         int err = prsAssign(tmp);
         if(err != PRS_OK) return err;

         return PRS_OK;
      }break;
      // 16. <command> -> if expression then <stat_list> else <stat_list> end
      case KW_IF:{
         int err;
         NEXT_TOKEN
         expJump();
         if(token != KW_THEN) return SYN_ERR;

         int tmp = ++cnt;
         printf("\tJMP_Z tmp else_%d\n", tmp);
         err = prsStat();
         printf("\tJMP if_end_%d\n", tmp);
         if(err != PRS_OK) return err;

         if(token != KW_ELSE) return SYN_ERR;

         printf("\tLAB else_%d\n", tmp);
         err = prsStat();
         if(err != PRS_OK) return err;

         if(token != KW_END) return SYN_ERR;
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
         if(token != KW_THEN) return SYN_ERR;

         int err = prsStat();
         if(err != PRS_OK) return err;

         if(token != KW_END) return SYN_ERR;
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
         if(token != L_LEFT_BRACKET) return SYN_ERR;

         NEXT_TOKEN
         expJump();
         printf("\tWRITE tmp\n");

         err = prsExpN();
         if(err != PRS_OK ) return err;

         if(token != L_RIGHT_BRACKET) return SYN_ERR;

         NEXT_TOKEN
         return PRS_OK;
      }break;
   }
   return SYN_ERR;
}

int prsExpN(){
   // 20. <expression_n> -> eps
   if(token == L_RIGHT_BRACKET) return PRS_OK;

   // 21. <expression_n> -> , expression <expression_n>
   if(token != L_COMMA) return SYN_ERR;
   NEXT_TOKEN
   expJump();
   printf("\tWRITE tmp\n");

   return prsExpN();
}

int prsAssign(TVar *var){

   NEXT_TOKEN
   if(token == KW_READ){
      // 24. <assign> -> read ( <lit> )
      NEXT_TOKEN
      if(token != L_LEFT_BRACKET) return SYN_ERR;

      NEXT_TOKEN
      if(token != L_STRING && token != L_NUMBER ) return SYN_ERR;

      printf("\tREAD %s \"%s\"\n",var->name, attr.str);
      /*if( listInsertLast( instr, genInstr(I_READ, var, NULL, NULL) ) != LIST_EOK)
         return INTR_ERR;*/

      NEXT_TOKEN
      if(token != L_RIGHT_BRACKET) return SYN_ERR;

      NEXT_TOKEN
      return PRS_OK;
   }

   int tokenTmp = token; // ulozim si attr abych potom vedel jakou instrukci generovat
   TFunction *Ftmp = tableSearchFunction(table, attr);

   if(Ftmp == NULL && (token != KW_TYPE && token != KW_SUBSTR && token != KW_FIND && token != KW_SORT) ){
      // 23. <assign> -> expression
      expJump();
      printf("\tMOV %s tmp\n", var->name);
      return PRS_OK;
   }

   // 25. <assign> -> idFunc ( <params> )
   NEXT_TOKEN
   if(token != L_LEFT_BRACKET) return SYN_ERR;

   int err = prsVarParams();
   if(err != PRS_OK) return err;

   if(token != L_RIGHT_BRACKET) return SYN_ERR;

   switch(tokenTmp){
      case L_ID:{
            err = listInsertLast( instr, genInstr(I_CALL, Ftmp, NULL, NULL) );
         }break;
      case KW_TYPE:{
            err = listInsertLast( instr, genInstr(I_TYPE, NULL, NULL, NULL) );
         }break;
      case KW_SUBSTR:{
            err = listInsertLast( instr, genInstr(I_SUBSTR, NULL, NULL, NULL) );
         }break;
      case KW_FIND:{
            err = listInsertLast( instr, genInstr(I_FIND, NULL, NULL, NULL) );
         }break;
      case KW_SORT:{
            err = listInsertLast( instr, genInstr(I_SORT, NULL, NULL, NULL) );
         }break;
   }
   if(err != LIST_EOK)
      return INTR_ERR;

   if( listInsertLast( instr, genInstr(I_POP, var, NULL, NULL) ) != LIST_EOK)
      return INTR_ERR;

   NEXT_TOKEN
   return PRS_OK;
}

int prsVarParams(){
   NEXT_TOKEN
   // 26. <var_params> -> eps
   if(token == L_RIGHT_BRACKET) return PRS_OK;

   // 27. <var_params> -> <var> <var_n>
   // parametry musim dat v opacnem poradi
   // nastavim aktivitu na posledni prvek seznamu instrukci
   listLast(instr);
   // vsechny dalsi instrukce dava za aktivni prvek!!!

   int err = prsVar();
   if(err != PRS_OK) return err;

   err = prsVarN();
   if(err != PRS_OK) return err;

   if(token == L_RIGHT_BRACKET) return PRS_OK;

   return SYN_ERR;
}

int prsVar(){
   // 28. <var> -> <lit>
   int err = prsLit();
   if(err == PRS_OK){
      // konstantka
      // vztvorim konstantu
      if(functionInsertConstatnt(table->lastAddedFunc, attr, token ) != INS_OK )
         return INTR_ERR;
      // vytvorim instrukci
      TVar *con = table->lastAddedFunc->constants.Last->data;
      if( listPostInsert ( instr, genInstr(I_PUSH, con, NULL, NULL) ) != LIST_EOK)
         return INTR_ERR;

   }
   if(err == SYN_ERR){
      // 29. <var> -> id
      if(token != L_ID) return SYN_ERR;
      TVar *tmp = functionSearchVar(table->lastAddedFunc, attr);
      if(tmp == NULL)   return SEM_ERR;

      if( listPostInsert( instr, genInstr(I_PUSH, tmp, NULL, NULL) ) != LIST_EOK)
         return INTR_ERR;

      return PRS_OK;
   }
   return err;
}

int prsVarN(){
   NEXT_TOKEN
   // 30. <var_n> -> eps
   if(token == L_RIGHT_BRACKET) return PRS_OK;
   // 31. <var_n> -> , <var> <var_n>
   if(token != L_COMMA) return SYN_ERR;

   NEXT_TOKEN
   int err = prsVar();
   if(err != PRS_OK) return err;

   err = prsVarN();
   if(err != PRS_OK) return err;

   return PRS_OK;
}
