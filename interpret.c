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
/* @description ulozi data do struktury
 * @param nova data
 * @param vykonavana instrukce
 * @param aktualni funkce
 * @return chybovy kod
 */
int saveData(TVarData *data, TInstr *instr, TFunction *fce) {
  TVar *tempVar = (TVar *) instr->dest; 

	/*pokud prepisovana hodnota je retezec, uvolni*/
	if (tempVar->varData->type == STRING)
		strFree(&tempVar->varData[fce->cnt].value.s);

	/*nastavi typ a data promenne*/
	tempVar->varData[fce->cnt].type = data->type;
	switch (data->type) {
		case BOOL: tempVar->varData[fce->cnt].value.b = data->value.b; break;
		case NUMBER: tempVar->varData[fce->cnt].value.n = data->value.n; break;
		case STRING: 
	    if (strCopyString(&data->value.s,&tempVar->varData[fce->cnt].value.s) == STR_ERROR)
				return EXIT_FAILURE;				
	  break;
		case NIL: break;
	} 
	return EXIT_SUCCESS;
}

//=================================================================================================>
//-------------------------TVarData *giveMeData(TInstr *instr, TFunction *fce);-------------------->
//=================================================================================================>
/* @description vytahne data ze struktur
 * @param dest/src1/src2
 * @param vykonavana instrukce
 * @param aktualni funkce
 * @return ukazatel na data
 */
TVarData *giveMeData(int what, TInstr *instr, TFunction *fce) {
	TVar *tempVar;

	switch (what) {
		case DEST: tempVar = (TVar *) instr->dest; break;
		case SRC1: tempVar = (TVar *) instr->src1; break;
	  case SRC2: tempVar = (TVar *) instr->src2; break;
		default: return NULL; break;
	}

  if (tempVar->varType == VT_VAR)
   	return &tempVar->varData[fce->cnt];
  else return tempVar->varData;
}

//=================================================================================================>
//------------------------------------int interpret(TFunction *fce);------------------------------->
//=================================================================================================>
/* @description vykona interpretaci funkce
 * @param aktualni funkce
 * @return chybovy kod 
 */
int interpret(TFunction *fce) {

	fce->cnt++;
	TInstr *instr; 
	TVarData *data1;
	TVarData *data2;
	TVarData newData;
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

		/*rozpozna typ instrukce*/
		switch (instr->type) {
			case I_LAB: break;
			case I_RETURN: break;

		/*instrukce pro praci se zasobnikem*/								 
			case I_POP: break;
			case I_PUSH: break;
			case I_STACK_E: break;

		/*instrukce pro inicializace, presuny*/
			case I_MOV: break;
			case I_SET: break;

		/*instrukce pro aritmeticke operace*/
			/*======================================================================*/
			case I_ADD: break;
			/*======================================================================*/
			case I_SUB: break;
			/*======================================================================*/
			case I_MUL: break;
			/*======================================================================*/
			case I_DIV: break;
			/*======================================================================*/
			case I_POW: break;
			/*======================================================================*/
			case I_CON: break;

		/*instrukce pro porovnani vyrazu*/
			/*===============================I_CMP_L================================*/
			case I_CMP_L:	
				newData.type = BOOL;
				data1 = giveMeData(SRC1,instr,fce);
				data2 = giveMeData(SRC2,instr,fce);

				if ((data1->type == STRING) && (data2->type == STRING)) {
					if (strcmp(data1->value.s.str,data2->value.s.str) < 0) 
					  newData.value.b = TRUE;
					else newData.value.b = FALSE;
				}

				else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
					if (data1->value.n < data2->value.n) 
						newData.value.b = TRUE;
					else newData.value.b = FALSE;
				} 

				else newData.value.b = FALSE;
				if (saveData(&newData,instr,fce) == EXIT_FAILURE) return ERR_INTERNAL;
			break; 

			/*==============================I_CMP_LE================================*/
			case I_CMP_LE: 
			  newData.type = BOOL;
				data1 = giveMeData(SRC1,instr,fce);
				data2 = giveMeData(SRC2,instr,fce);		

			  if ((data1->type == STRING) && (data2->type == STRING)) {
				  if (strcmp(data1->value.s.str,data2->value.s.str) <= 0)
				    newData.value.b = TRUE;
			    else newData.value.b = FALSE;
				}

		    else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
		      if (data1->value.n <= data2->value.n)
	          newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }
        
				else newData.value.b = FALSE;
        if (saveData(&newData,instr,fce) == EXIT_FAILURE) return ERR_INTERNAL;
			break;

			/*==============================I_CMP_G=================================*/
			case I_CMP_G: 
        newData.type = BOOL;
				data1 = giveMeData(SRC1,instr,fce);
				data2 = giveMeData(SRC2,instr,fce);

      	if ((data1->type == STRING) && (data2->type == STRING)) {
				  if (strcmp(data1->value.s.str,data2->value.s.str) > 0)
				    newData.value.b = TRUE;
				  else newData.value.b = FALSE;
				}

       else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
         if (data1->value.n > data2->value.n)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }
 
        else newData.value.b = FALSE;
				if (saveData(&newData,instr,fce) == EXIT_FAILURE) return ERR_INTERNAL;
			break;

			/*==============================I_CMP_GE================================*/
			case I_CMP_GE: 
        newData.type = BOOL;
				data1 = giveMeData(SRC1,instr,fce);
				data2 = giveMeData(SRC2,instr,fce);

        if ((data1->type == STRING) && (data2->type == STRING)) {
          if (strcmp(data1->value.s.str,data2->value.s.str) >= 0)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
          if (data1->value.n >= data2->value.n)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }
 
				else newData.value.b = FALSE;
				if (saveData(&newData,instr,fce) == EXIT_FAILURE) return ERR_INTERNAL;
			break;

			/*==============================I_CMP_E=================================*/
			case I_CMP_E: 
        newData.type = BOOL;
				data1 = giveMeData(SRC1,instr,fce);
        data2 = giveMeData(SRC2,instr,fce);

        if ((data1->type == STRING) && (data2->type == STRING)) {
          if (strcmp(data1->value.s.str,data2->value.s.str) == 0)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
          if (data1->value.n == data2->value.n)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == BOOL) && (data2->type == BOOL)) {
          if (data1->value.b == data2->value.b)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == NIL) && (data2->type == NIL))
          newData.value.b = TRUE;
        else newData.value.b = FALSE;
        if (saveData(&newData,instr,fce) == EXIT_FAILURE) return ERR_INTERNAL;
			break;

			/*=============================I_CMP_NE=================================*/
			case I_CMP_NE: 
        newData.type = BOOL;
				data1 = giveMeData(SRC1,instr,fce);
        data2 = giveMeData(SRC2,instr,fce);

        if ((data1->type == STRING) && (data2->type == STRING)) {
          if (strcmp(data1->value.s.str,data2->value.s.str) != 0)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == NUMBER) && (data2->type == NUMBER)) {
          if (data1->value.n != data2->value.n)
             newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == BOOL) && (data2->type == BOOL)) {
          if (data1->value.b != data2->value.b)
            newData.value.b = TRUE;
          else newData.value.b = FALSE;
        }

        else if ((data1->type == NIL) && (data2->type == NIL))
           newData.value.b = FALSE;
        else newData.value.b = TRUE;
				if (saveData(&newData,instr,fce) == EXIT_FAILURE) return ERR_INTERNAL;
			break;

		/*instrukce pro skoky*/
			case I_JMP: break;
			case I_JMP_Z: break;
			case I_JMP_NZ: break;

		/*instrukce write, read, call*/
			case I_WRITE: break;
			case I_READ: break;
			case I_CALL: break;

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
