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
	    if (strCopyString(&data->value.s,&tempVar->value.s) == STR_ERROR)
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
						case STRING: newData.value.b = (strcmp(data1->value.s.str,data2->value.s.str) < 0) ? TRUE : FALSE;	break;
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
					return ERR_INTERNAL;
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
						case NIL: newData.value.b = TRUE;
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
            case NIL: newData.value.b = FALSE;
	        }
				}

        else newData.value.b = TRUE;
				if (saveData(&newData,instr->dest,fce) == EXIT_FAILURE) 
					return ERR_INTERNAL;
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
