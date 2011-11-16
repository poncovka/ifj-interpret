#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

#include "table.h"
#include "scanner.h"
#include "str.h"
#include "list.h"

#define PRS_OK    0
#define LEX_ERR  -1
#define SYN_ERR  -2
#define SEM_ERR  -3
#define RUN_ERR  -4
#define INTR_ERR -5

int parser(TTable*);

#endif // PARSER_H_INCLUDED
