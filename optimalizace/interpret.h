#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "str.h"
#include "list.h"
#include "table.h"
#include "stack.h"
#include "expression.h"
#include "library.h"

#define ERR_SEM				-3
#define ERR_INTERNAL	-5
#define ERR_INTERPRET -4
#define INTERPRET_OK 	0

#define isGenerallyInstr(i)  (i <= I_SET)
#define isMathInstr(i)       (i <= I_CON)
#define isLogicInstr(i)      (i <= I_CMP_NE)
#define isOtherInstr(i)      (i <= I_CALL)
#define isFuncInstr(i)       (i <= I_SORT)

/*definice funkci*/
int interpret(TFunction *fce);

#endif
