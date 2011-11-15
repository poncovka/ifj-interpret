//=================================================================================================>
//---------------------------------------------INTERPRET------------------------------------------->
//=================================================================================================>
/* @author: Tomas Trkal, xtrkal00@stud.fit.vutbr.cz
 * @date:   14.11.2011
 */						

#include "interpret.h"

/*vykona interpretaci funkce*/
int interpret(TFunction *fce) {

	fce->cnt++;
	TInstr *instr; 
  if (listFirst(&fce->instructions) == LIST_ERR) 
		return ERR_INTERNAL;

  /*cyklus provede vykonani vsech fci ze seznamu*/
	while (listActive(&fce->instructions)) {
		if ((instr = (TInstr*) listCopy(&fce->instructions)) == NULL)
			return ERR_INTERNAL;

		/*rozpozna instrukci*/
		switch (instr->type) {

			case I_LAB: break;
			case I_RETURN: break;

			case I_POP: break;
			case I_PUSH: break;
			case I_STACK_E: break;

			case I_MOV: break;
			case I_SET: break;

			case I_ADD: break;
			case I_SUB: break;
			case I_MUL: break;
			case I_DIV: break;
			case I_POW: break;
			case I_CON: break;

			case I_CMP_L: break;
			case I_CMP_LE: break;
			case I_CMP_G: break;
			case I_CMP_GE: break;
			case I_CMP_E: break;
			case I_CMP_NE: break;

			case I_JMP: break;
			case I_JMP_Z: break;
			case I_JMP_NZ: break;

			case I_WRITE: break;
			case I_READ: break;
			case I_CALL: break;

			case I_TYPE: break;
			case I_SUBSTR: break;
			case I_FIND: break;
			case I_SORT: break;
	
		} /*konec switch*/
	} /*konec while*/
} /*konec interpret*/
