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

#define NEXT_TOKEN lex = getNextToken(&token); if(lex < 0) return lex;

string token;
int    lex;
TTable *table;

int parser(TTable*);

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
int prsAssign();
int prsVarParams();
int prsVar();
int prsVarN();


#endif // PARSER_H_INCLUDED
