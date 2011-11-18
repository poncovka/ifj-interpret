//=================================================================================================>
//---------------------------------------------INTERPRET------------------------------------------->
//=================================================================================================>
/* @author: Tomas Trkal, xtrkal00@stud.fit.vutbr.cz
 * @date:   14.11.2011
 */

#include "interpret.h"

//enum ETruth {FALSE, TRUE}
enum ESource {DEST, SRC1, SRC2};

//=================================================================================================>
//------------------void saveData(TVarData *data, TInstr *instr, TFunction *fce);------------------>
//=================================================================================================>
/* @description ulozi data do struktury /co, kam, fce/
 * @param nova data
 * @param uloziste
 * @param aktualni funkce
 * @return chybovy kod
 */
int saveData(TVarData *data, void *dest, TFunction *fce) {
        int index = (((TVar *)dest)->varType == VT_VAR) ? fce->cnt : 0;
        TVarData *tempVar = &((TVar *)dest)->varData[index];

        /*pokud prepisovana hodnota je retezec, uvolni*/
        if (tempVar->type == STRING)
                strFree(&tempVar->value.s);

        /*nastavi typ a data promenne*/
        tempVar->type = data->type;
        switch (data->type) {
                case BOOL: tempVar->value.b = data->value.b; break;
                case NUMBER: tempVar->value.n = data->value.n; break;
                case STRING:
                        strInit(&tempVar->value.s);
            if (strCopyString(&tempVar->value.s,&data->value.s) == STR_ERROR)
                                return EXIT_FAILURE;
          break;
                case NIL: break;
        }
        return EXIT_SUCCESS;
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
TVarData *giveMeData(void *data, TFunction *fce) {
  return &(((TVar *)data)->varData[(((TVar *)data)->varType == VT_VAR) ? fce->cnt : 0]);
}

//=================================================================================================>
//------------------------------------int interpret(TFunction *fce);------------------------------->
//=================================================================================================>
/* @description vykona interpretaci funkce
 * @param aktualni funkce
 * @return chybovy kod
 */
int interpret(TFunction *fce) {
        TInstr *instr;
        TVarData *data1;
        TVarData *data2;
        TVarData newData;

        /*vytvoreni a inicializace zasobniku*/
        TStack stack;
        stackInit(&stack);

        /*nastavi ukazatel na prvni instrukci*/
        fce->cnt++;
  if (listFirst(&fce->instructions) == LIST_ERR)
                return ERR_INTERNAL;

  /*cyklus provede vykonani vsech fci ze seznamu*/
        while (listActive(&fce->instructions)) {

                /*precte aktualni instrukci ze seznamu*/
                if ((instr = (TInstr*) listCopy(&fce->instructions)) == NULL)
                        return ERR_INTERNAL;

                /*kontrola semantiky matematickych a porovnavacich operaci*/
                if (checkSemErr(instr, (TVar *) instr->src1)) return ERR_SEM;
                if (checkSemErr(instr, (TVar *) instr->src2)) return ERR_SEM;

                /*rozpozna typ instrukce a vykona ji*/
                switch (instr->type) {
                        case I_LAB: break;
                        case I_RETURN: break;

                /*instrukce pro praci se zasobnikem*/
                        /*===========================================I_POP==========================================*/
                        case I_POP:

                        break;

                        /*==========================================I_PUSH==========================================*/
                        case I_PUSH:
                                data1 = giveMeData(instr->dest,fce);
                                if (stackPush(&stack,data1) != STACK_EOK)
                                        return ERR_INTERNAL;
                        break;

                        /*=========================================I_STACK_E========================================*/
                        case I_STACK_E:
                                if (stackDelete(&stack) != STACK_EOK)
                                        return ERR_INTERNAL;
                        break;

                /*instrukce pro inicializace, presuny*/
                        /*===========================================I_MOV==========================================*/
                        case I_MOV:
                          data1 = giveMeData(instr->src1,fce);
                                if (saveData(data1,instr->dest,fce) == EXIT_FAILURE)
                                        return ERR_INTERNAL;
                        break;

                        /*===========================================I_SET==========================================*/
                        case I_SET:
                                if (varRealloc(instr->dest,fce->cnt) != INS_OK)
                                return ERR_INTERNAL;
                                if (instr->src1 != NULL) {
                                        data1 = giveMeData(instr->src1,fce);
                                        if (saveData(data1,instr->dest,fce) == EXIT_FAILURE)
                                          return ERR_INTERNAL;
                                }
                                else ((TVar *)instr->dest)->varData->type = NIL;
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
                                /*??? jak spojit retezce ???*/
                                if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
                                        return ERR_INTERNAL;
                        break;

                /*instrukce pro porovnani vyrazu*/
                        /*==========================================I_CMP_L=========================================*/
                        case I_CMP_L:
                                newData.type = BOOL;
                                data1 = giveMeData(instr->src1,fce);
                                data2 = giveMeData(instr->src2,fce);

                                if (data1->type == data2->type) {
                                        switch (data1->type) {
                                                case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) < 0) ? TRUE : FALSE;      break;
                                                case NUMBER: newData.value.b = (data1->value.n < data2->value.n) ? TRUE : FALSE; break;
                                                case BOOL: break;
                                                case NIL: break;
                                        }
                                }

                                else newData.value.b = FALSE;
                                if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
                                        return ERR_INTERNAL;
                        break;

                        /*=========================================I_CMP_LE=========================================*/
                        case I_CMP_LE:
                          newData.type = BOOL;
                                data1 = giveMeData(instr->src1,fce);
                                data2 = giveMeData(instr->src2,fce);

        if (data1->type == data2->type) {
          switch (data1->type) {
            case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) <= 0) ? TRUE : FALSE; break;
            case NUMBER: newData.value.b = (data1->value.n <= data2->value.n) ? TRUE : FALSE; break;
                                                case BOOL: break;
                                                case NIL: break;
          }
        }

                                else newData.value.b = FALSE;
        if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
                                        return ERR_INTERNAL;
                        break;

                        /*==========================================I_CMP_G=========================================*/
                        case I_CMP_G:
        newData.type = BOOL;
                                data1 = giveMeData(instr->src1,fce);
                                data2 = giveMeData(instr->src2,fce);

        if (data1->type == data2->type) {
          switch (data1->type) {
            case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) > 0) ? TRUE : FALSE; break;
            case NUMBER: newData.value.b = (data1->value.n > data2->value.n) ? TRUE : FALSE; break;
                                                case BOOL: break;
                                                case NIL: break;
          }
        }

                                else newData.value.b = FALSE;
                                if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
                                        return ERR_INTERNAL;
                        break;

                        /*=========================================I_CMP_GE=========================================*/
                        case I_CMP_GE:
        newData.type = BOOL;
                                data1 = giveMeData(instr->src1,fce);
                                data2 = giveMeData(instr->src2,fce);

                                if (data1->type == data2->type) {
          switch (data1->type) {
            case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) >= 0) ? TRUE : FALSE; break;
            case NUMBER: newData.value.b = (data1->value.n >= data2->value.n) ? TRUE : FALSE; break;
                                                case BOOL: break;
                                                case NIL: break;
          }
                                }

                                else newData.value.b = FALSE;
                                if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE)
                                        return ERR_INTERNAL; break;
                        break;

                        /*=========================================I_CMP_E==========================================*/
                        case I_CMP_E:
        newData.type = BOOL;
                                data1 = giveMeData(instr->src1,fce);
        data2 = giveMeData(instr->src2,fce);

        if (data1->type == data2->type) {
          switch (data1->type) {
            case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) == 0) ? TRUE : FALSE; break;
            case NUMBER: newData.value.b = (data1->value.n == data2->value.n) ? TRUE : FALSE; break;
                                                case BOOL: newData.value.b = (data1->value.b == data2->value.b) ? TRUE : FALSE; break;
                                                case NIL: newData.value.b = TRUE; break;
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
            case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) != 0) ? TRUE : FALSE; break;
            case NUMBER: newData.value.b = (data1->value.n != data2->value.n) ? TRUE : FALSE; break;
            case BOOL: newData.value.b = (data1->value.b != data2->value.b) ? TRUE : FALSE; break;
            case NIL: newData.value.b = FALSE; break;
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
                                  case STRING: printf("%s",data1->value.s.str); break;
                                        case NUMBER: printf("%g",data1->value.n); break;
                                        case BOOL: return ERR_SEM; break;
                                        case NIL: return ERR_SEM; break;
                                }
                        break;

                        /*========================================I_READ============================================*/
                        case I_READ:
                        break;

                        /*========================================I_CALL============================================*/
                        case I_CALL:
                                switch (interpret((TFunction *)instr->dest)) {
                                  case ERR_INTERPRET: return ERR_INTERPRET; break;
                                        case ERR_INTERNAL: return ERR_INTERNAL; break;
                                        case ERR_SEM: return ERR_SEM; break;
                                        case INTERPRET_OK: break;
                                }
                        break;

                /*instrukce pro vestavene funkce*/
                        case I_TYPE: break;
                        case I_SUBSTR: break;
                        case I_FIND: break;
                        case I_SORT: break;

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
} //konec interpret
