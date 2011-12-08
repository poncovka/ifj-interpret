//=================================================================================================>
//---------------------------------------------INTERPRET------------------------------------------->
//=================================================================================================>
/* @author: Tomas Trkal, xtrkal00@stud.fit.vutbr.cz
 * @date:   14.11.2011
 */

#include "interpret.h"

/*globalni promenna zasobnik*/
TStack stack;

/*deklarace funkci*/
int executor(TFunction *fce);
int stackDeleteDataDelete(TStack *s);

//=================================================================================================>
//------------------------------TVarData *interpret(TFunction *fce);------------------------------->
//=================================================================================================>
/* @description vykona interpretaci funkce
 * @param aktualni funkce
 * @return chybovy kod
 */
int interpret(TFunction *fce) {
   int result;
   stackInit(&stack);              // inicializace zasobniku
   result = executor(fce);         // vykonani interpretace
   stackDeleteDataDelete(&stack);  // uvolneni zasobniku
   return result;
}

//=================================================================================================>
//-----------------------------void printTVarData(TVarData *data);--------------------------------->
//=================================================================================================>
/* @desciption vypise typ dat a hodnotu (pozn.: debug)
 * @param data
 */
void printTVarData(TVarData *data) {
   if (data == NULL) {
      printf("NULL\n");
      return;
   }
   switch (data->type) {
   case NUMBER:
      printf("NUMBER: %g\n",data->value.n);
      break;
   case STRING:
      printf("STRING: %s\n",data->value.s.str);
      break;
   case BOOL:
      printf("BOOL: %d\n",data->value.b);
      break;
   case NIL:
      printf("NIL\n");
      break;
   }
}

//=================================================================================================>
//----------------------------TVarData stackPopVarData(TStack *stack);----------------------------->
//=================================================================================================>
/* @description pokud neni zasobnik prazdny vrati data, jinak vrati NULL
 * @param zasobnik
 */
inline TVarData *stackPopVarData(TStack *stack) {
   // zasobnik neni prazdny, vrat ukazatel na data
   if (!stackEmpty(stack)) {
      return ((TVarData *) stackTopPop(stack));
   }
   // zasobnik je prazdny, vrat NULL
   else return NULL;
}

//=================================================================================================>
//----------------int cmpData(TVarData *data1, TVarData *data2, EInstrType instr);----------------->
//=================================================================================================>
/* @description vyhodnoti vyraz
 * @param prvni data
 * @param druha data
 * @param porovnavaci instrukce
 * @return TRUE/FALSE/ERR_INTERPRET
 */
inline int cmpData(TVarData *data1, TVarData *data2, EInstrType instr) {
   if ((data1->type == STRING) && (data2->type == STRING)) {
      switch (instr) {
      case I_CMP_L:
         return (strCmpString(&data1->value.s,&data2->value.s) < 0) ? TRUE : FALSE;
         break;
      case I_CMP_LE:
         return (strCmpString(&data1->value.s,&data2->value.s) <= 0) ? TRUE : FALSE;
         break;
      case I_CMP_G:
         return (strCmpString(&data1->value.s,&data2->value.s) > 0) ? TRUE : FALSE;
         break;
      case I_CMP_GE:
         return (strCmpString(&data1->value.s,&data2->value.s) >= 0) ? TRUE: FALSE;
         break;
      default:
         break;
      }
   }

   else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
      switch (instr) {
      case I_CMP_L:
         return (data1->value.n < data2->value.n) ? TRUE : FALSE;
         break;
      case I_CMP_LE:
         return (data1->value.n <= data2->value.n) ? TRUE : FALSE;
         break;
      case I_CMP_G:
         return (data1->value.n > data2->value.n) ? TRUE : FALSE;
         break;
      case I_CMP_GE:
         return (data1->value.n >= data2->value.n) ? TRUE : FALSE;
         break;
      default:
         break;
      }
   }

   else return ERR_INTERPRET;
   return FALSE;
}

//=================================================================================================>
//-------------------------TVarData *copyData(TVarData *dest, TVarData *src);---------------------->
//=================================================================================================>

inline int copyData(TVarData *dest, TVarData *src, int copyString) {
   /*pokud prepisovana hodnota je retezec, uvolni*/
   freeVarData(dest);

   /*pokud je ukazatel nulovy*/
   if (src == NULL) {
      dest->type = NIL;
      return EXIT_SUCCESS;
   }

   /*nastavi typ a src promenne*/
   dest->type = src->type;
   switch (src->type) {
   case BOOL:
      dest->value.b = src->value.b;
      break;
   case NUMBER:
      dest->value.n = src->value.n;
      break;
   case STRING:
      if (copyString == TRUE) {
         dest->value.s = strCreateString(&src->value.s);
         if (strIsNull(&dest->value.s))
            return EXIT_FAILURE;
      }
      else {
         dest->value.s = src->value.s;
         dest->value.s.str = src->value.s.str;
      }
      break;
   case NIL:
      break;
   }
   return EXIT_SUCCESS;

}
//=================================================================================================>
//------------------void saveData(TVarData *data, TInstr *instr, TFunction *fce);------------------>
//=================================================================================================>
/* @description ulozi data do struktury /co, kam, fce/
 * @param nova data
 * @param uloziste
 * @param aktualni funkce
 * @return chybovy kod
 */
inline int saveData(TVarData *data, void *dest, TFunction *fce) {
   int index = (((TVar *)dest)->varType == VT_VAR) ? fce->cnt : 0;
   TVarData *tempVar = &((TVar *)dest)->varData[index];
   return copyData(tempVar, data, TRUE);
}

//=================================================================================================>
//-------------------------TVarData *giveMeData(void *data, TFunction *fce);----------------------->
//=================================================================================================>
/* @description vrati ukazatel na data
 * @correction Marek Salat
 * @param dest/src1/src2
 * @param aktualni funkce
 * @return ukazatel na data
 */
inline TVarData *giveMeData(void *data, TFunction *fce) {
   return &(((TVar *)data)->varData[(((TVar *)data)->varType == VT_VAR) ? fce->cnt : 0]);
}

//=================================================================================================>
//-------------------------------------int executor(TFunction *fce);------------------------------->
//=================================================================================================>
/* @description vykona interpretaci funkce
 * @param aktualni funkce
 * @return chybovy kod
 */
int executor(TFunction *fce) {

   TInstr   *instr;

   // pomocne promenne
   int result;
   TVarData *dest;
   TVarData *data1, *data2;
   TVarData *param, *param2, *param3;

   TVarData newData;
   TVarData nilData = {.type = NIL};

   /*nastavi ukazatel na prvni instrukci*/
   fce->cnt++;
   if (listFirst(&fce->instructions) == LIST_ERR)
      return ERR_INTERNAL;

   /*cyklus provede vykonani vsech instrukci ze seznamu*/
   while (listActive(&fce->instructions)) {

      /*precte aktualni instrukci ze seznamu*/
      if ((instr = (TInstr*) listCopy(&fce->instructions)) == NULL)
         return ERR_INTERNAL;

      /*kontrola semantiky matematickych a porovnavacich operaci*/

      if (isMathOperation(instr->type)) {
         if (checkSemErr(instr, giveMeData((TVar*)instr->src1, fce))) return ERR_INTERPRET;
         if (checkSemErr(instr, giveMeData((TVar*)instr->src2, fce))) return ERR_INTERPRET;
      }

      /*rozpozna typ instrukce a vykona ji*/
      switch (instr->type) {
      case I_LAB:
         break;
      case I_RETURN:
         if (listLast(&fce->instructions) != LIST_EOK)
            return ERR_INTERNAL;
         break;

         /*instrukce pro praci se zasobnikem*/
         /*===========================================I_POP==========================================*/
      case I_POP: {
         data1 = stackPopVarData(&stack);

         if (saveData(data1,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;

         if (data1 != NULL) {
            free(data1);
         }
      }
      break;

      /*==========================================I_PUSH==========================================*/
      case I_PUSH: {
         data1 = giveMeData(instr->dest,fce);

         TVarData *copyTmp = malloc(sizeof(TVarData));
         if (copyTmp == NULL)
            return ERR_INTERNAL;
         copyTmp->type = NIL;

         if (copyData(copyTmp, data1, FALSE) != EXIT_SUCCESS)
            return ERR_INTERNAL;

         if (stackPush(&stack,copyTmp) != STACK_EOK)
            return ERR_INTERNAL;
      }
      break;

      /*=========================================I_STACK_E========================================*/
      case I_STACK_E: {
         if(stackDeleteDataDelete(&stack) != STACK_EOK)
            return ERR_INTERNAL;
      }
      break;

      /*instrukce pro inicializace, presuny*/
      /*===========================================I_MOV==========================================*/
      case I_MOV:
         data1 = giveMeData(instr->src1,fce);
         if (saveData(data1,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*===========================================I_SET==========================================*/
      case I_SET: {
         // realokace pokud nedostatek mista
         if( ( ((TVar*)instr->dest)->alloc - fce->cnt) < 1) {
           if (varRealloc(instr->dest,fce->cnt) != INS_OK)
              return ERR_INTERNAL;
         }

         TVarData *tmp = giveMeData(instr->dest, fce);
         freeVarData(tmp);
         if (instr->src1 != NULL) {
            data1 = giveMeData(instr->src1,fce);
            if (saveData(data1,instr->dest,fce) == EXIT_FAILURE)
               return ERR_INTERNAL;
         }
      }
      break;

      /*instrukce pro aritmeticke operace*/
      /*===========================================I_ADD==========================================*/
      case I_ADD:
         newData.type = NUMBER;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         newData.value.n = data1->value.n + data2->value.n;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*===========================================I_SUB==========================================*/
      case I_SUB:
         newData.type = NUMBER;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         newData.value.n = data1->value.n - data2->value.n;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*===========================================I_MUL==========================================*/
      case I_MUL:
         newData.type = NUMBER;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         newData.value.n = data1->value.n * data2->value.n;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*===========================================I_DIV==========================================*/
      case I_DIV:
         newData.type = NUMBER;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         if (data2->value.n == 0) return ERR_INTERPRET; // deleni nulou
         else newData.value.n = data1->value.n / data2->value.n;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*===========================================I_POW==========================================*/
      case I_POW:
         newData.type = NUMBER;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         newData.value.n = pow(data1->value.n,data2->value.n);
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*===========================================I_CON==========================================*/
      case I_CON:
         newData.type = STRING;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         newData.value.s = strConcatenation(&data1->value.s,&data2->value.s);
         if (strIsNull(&newData.value.s)) return ERR_INTERNAL;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE) {
            strFree(&newData.value.s);
            return ERR_INTERNAL;
         }
         strFree(&newData.value.s);
         break;

         /*instrukce pro porovnani vyrazu*/
         /*==========================================I_CMP_L=========================================*/
      case I_CMP_L:
         newData.type = BOOL;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         switch (cmpData(data1,data2,instr->type)) {
         case TRUE:
            newData.value.b = TRUE;
            break;
         case FALSE:
            newData.value.b = FALSE;
            break;
         case ERR_INTERPRET:
            return ERR_INTERPRET;
            break;
         }
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*=========================================I_CMP_LE=========================================*/
      case I_CMP_LE:
         newData.type = BOOL;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         switch (cmpData(data1,data2,instr->type)) {
         case TRUE:
            newData.value.b = TRUE;
            break;
         case FALSE:
            newData.value.b = FALSE;
            break;
         case ERR_INTERPRET:
            return ERR_INTERPRET;
            break;
         }
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*==========================================I_CMP_G=========================================*/
      case I_CMP_G:
         newData.type = BOOL;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         switch (cmpData(data1,data2,instr->type)) {
         case TRUE:
            newData.value.b = TRUE;
            break;
         case FALSE:
            newData.value.b = FALSE;
            break;
         case ERR_INTERPRET:
            return ERR_INTERPRET;
            break;
         }
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*=========================================I_CMP_GE=========================================*/
      case I_CMP_GE:
         newData.type = BOOL;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);
         switch (cmpData(data1,data2,instr->type)) {
         case TRUE:
            newData.value.b = TRUE;
            break;
         case FALSE:
            newData.value.b = FALSE;
            break;
         case ERR_INTERPRET:
            return ERR_INTERPRET;
            break;
         }
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;
         break;

         /*=========================================I_CMP_E==========================================*/
      case I_CMP_E:
         newData.type = BOOL;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);

         if (data1->type == data2->type) {
            switch (data1->type) {
            case STRING:
               newData.value.b = (strCmpString(&data1->value.s,&data2->value.s) == 0) ? TRUE : FALSE;
               break;
            case NUMBER:
               newData.value.b = (data1->value.n == data2->value.n) ? TRUE : FALSE;
               break;
            case BOOL:
               newData.value.b = (data1->value.b == data2->value.b) ? TRUE : FALSE;
               break;
            case NIL:
               newData.value.b = TRUE;
               break;
            }
         }

         else newData.value.b = FALSE;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*========================================I_CMP_NE==========================================*/
      case I_CMP_NE:
         newData.type = BOOL;
         data1 = giveMeData(instr->src1,fce);
         data2 = giveMeData(instr->src2,fce);

         if (data1->type == data2->type) {
            switch (data1->type) {
            case STRING:
               newData.value.b = (strCmpString(&data1->value.s,&data2->value.s) != 0) ? TRUE : FALSE;
               break;
            case NUMBER:
               newData.value.b = (data1->value.n != data2->value.n) ? TRUE : FALSE;
               break;
            case BOOL:
               newData.value.b = (data1->value.b != data2->value.b) ? TRUE : FALSE;
               break;
            case NIL:
               newData.value.b = FALSE;
               break;
            }
         }

         else newData.value.b = TRUE;
         if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
            return ERR_INTERNAL;
         break;

         /*instrukce pro skoky*/
         /*=========================================I_JMP============================================*/
      case I_JMP:
         listSetActive(&fce->instructions,(TLItem *)instr->dest);
         break;

         /*========================================I_JMP_Z===========================================*/
      case I_JMP_Z:
         data1 = giveMeData(instr->src1,fce);
         if (((data1->type == BOOL) && (data1->value.b == FALSE)) || (data1->type == NIL)) {
            listSetActive(&fce->instructions,(TLItem *)instr->dest);
         }
         break;

         /*========================================I_JMP_NZ==========================================*/
      case I_JMP_NZ:
         data1 = giveMeData(instr->src1,fce);
         if (((data1->type == BOOL) && (data1->value.b == TRUE)) ||
               (data1->type == STRING) || (data1->type == NUMBER)) {
            listSetActive(&fce->instructions,(TLItem *)instr->dest);
         }
         break;

         /*instrukce write, read, call*/
         /*========================================I_WRITE===========================================*/
      case I_WRITE:
         data1 = giveMeData(instr->dest,fce);
         switch (data1->type) {
         case STRING:
            printf("%s",data1->value.s.str);
            break;
         case NUMBER:
            printf("%g",data1->value.n);
            break;
         default:
            return ERR_SEM;
            break;
         }
         break;

         /*========================================I_READ============================================*/
      case I_READ:
         data1 = giveMeData(instr->src1,fce);
         dest = giveMeData(instr->dest,fce);
         freeVarData(dest);

         /*cislo*/
         if (data1->type == STRING) {
            if (strncmp(data1->value.s.str,"*n",2) == 0) {
               result = scanf("%lf",&dest->value.n); //dodelat osetreni
               if (result != 1) return ERR_SEM;
               dest->type = NUMBER;
            }

            /*do konce radky*/
            else if (strncmp(data1->value.s.str,"*l",2) == 0) {
               dest->value.s = strReadLine(stdin);
               if (strIsNull(&dest->value.s))
                  return ERR_INTERNAL;
               dest->type = STRING;
            }

            /*dokud neni EOF*/
            else if (strncmp(data1->value.s.str,"*a",2) == 0) {
               dest->value.s = strReadAll(stdin);
               if (strIsNull(&dest->value.s))
                  return ERR_INTERNAL;
               dest->type = STRING;
            }

            else return ERR_SEM;
         }

         /*pocet znaku*/
         else if (data1->type == NUMBER) {
            dest->value.s = strReadNChar(stdin,data1->value.n);
            if (strIsNull(&dest->value.s))
               return ERR_INTERNAL;
            dest->type = STRING;
         }

         else return ERR_SEM;
         break;

         /*========================================I_CALL============================================*/
      case I_CALL: {
         TLItem *tmp  = fce->instructions.Act;
         switch (executor((TFunction *)instr->dest)) {
         case ERR_INTERPRET:
            return ERR_INTERPRET;
            break;
         case ERR_INTERNAL:
            return ERR_INTERNAL;
            break;
         case ERR_SEM:
            return ERR_SEM;
            break;
         case INTERPRET_OK:
            break;
         }
         fce->instructions.Act = tmp;
      }
      break;

      /*instrukce pro vestavene funkce*/
      /*========================================I_TYPE============================================*/
      case I_TYPE: {
         // ziska parametry funkce
         dest  = giveMeData(instr->dest,fce);
         param = stackPopVarData(&stack);
         if (param == NULL) param = &nilData;

         // zavola funkci
         if (type(dest, param) == ERR)
            return ERR_INTERNAL;

         // kdyby predal vic parametru tak vyprazdnim zasobnik
         if(stackDeleteDataDelete(&stack) != STACK_EOK)
            return ERR_INTERNAL;

         // musim uvolnit parametr protze byl zkopirovany na zasobnik
         if (param != &nilData) {
            free(param);
         }
      }
      break;

      /*========================================I_SUBSTR==========================================*/
      case I_SUBSTR: {
         // ziskam parametry funkce
         dest = giveMeData(instr->dest,fce);
         param   = stackPopVarData(&stack);
         if (param == NULL) param = &nilData;
         param2  = stackPopVarData(&stack);
         if (param2 == NULL) param2 = &nilData;
         param3  = stackPopVarData(&stack);
         if (param3 == NULL) param3 = &nilData;

         // zavolam funkci
         if (substr(dest, param, param2, param3 ) == ERR)
            return ERR_INTERNAL;

         // kdyby predal vic parametru tak vyprazdnim zasobnik
         if(stackDeleteDataDelete(&stack) != STACK_EOK)
            return ERR_INTERNAL;

         // uvolnim pamet
         if (param != &nilData) {
            free(param);
         }
         if (param2 != &nilData) {
            free(param2);
         }
         if (param3 != &nilData) {
            free(param3);
         }
      }
      break;

      /*=========================================I_FIND===========================================*/
      case I_FIND: {
         // ziskam parametry funkce
         dest = giveMeData(instr->dest,fce);
         param   = stackPopVarData(&stack);
         if (param == NULL) param = &nilData;
         param2  = stackPopVarData(&stack);
         if (param2 == NULL) param2 = &nilData;

         // zavolam funkci
         if (find(dest, param, param2) == ERR)
            return ERR_INTERNAL;

         // kdyby predal vic parametru tak vyprazdnim zasobnik
         if(stackDeleteDataDelete(&stack) != STACK_EOK)
            return ERR_INTERNAL;

         // uvolni pamet
         if (param != &nilData) {
            free(param);
         }
         if (param2 != &nilData) {
            free(param2);
         }
      }
      break;

      /*=========================================I_SORT===========================================*/
      case I_SORT: {
         // ziskam parametry funkce
         dest = giveMeData(instr->dest,fce);
         param   = stackPopVarData(&stack);
         if (param == NULL) param = &nilData;

         // zavolam funkci
         if (sort(dest, param) == ERR)
            return ERR_INTERNAL;

         // kdyby predal vic parametru tak vyprazdnim zasobnik
         if(stackDeleteDataDelete(&stack) != STACK_EOK)
            return ERR_INTERNAL;

         // uvolnim pamet
         if (param != &nilData) {
            free(param);
         }
      }
      break;

      /*v pripade jineho typu chyba*/
      default:
         return ERR_INTERNAL;
         break;
      } //konec switch

      /*posune aktivitu na dalsi instrukci*/
      if ((listSucc(&fce->instructions)) == LIST_ERR)
         return ERR_INTERNAL;
   } //konec while

   fce->cnt--;
   return INTERPRET_OK;
} //konec executor


//=================================================================================================>
//--------------------------------int stackDeleteDataDelete(TStack *s);---------------------------->
//=================================================================================================>
/* uvolni zasobnik + uvolni data
 * @author Marek Salat - xsalat00
 * @param zasobnik
 * @return chybovy kod
 */
int stackDeleteDataDelete(TStack *s) {
   if (s != NULL) {
      while (!stackEmpty(s) ) {
         TVarData *tmp = (TVarData *)stackTop(s);
         free(tmp);
         stackPop(s);
      }
   }
   /*chybny ukazatel*/
   else return STACK_ERR;
   return STACK_EOK;
}
