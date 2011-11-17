#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "list.h"
#include "table.h"
#include "stack.h"

#define ERR_SEM				3
#define ERR_INTERNAL	4
#define ERR_INTERPRET 5
#define INTERPRET_OK 	0

/*definice funkci*/
int interpret(TFunction *fce);

#endif
