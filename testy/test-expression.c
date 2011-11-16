/*
 * @description   Test analýzy výrazù
 * @author        Vendula Poncová - xponco00
 * @projekt       IFJ11
 * @date
 */

#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "str.h"
#include <string.h>
#include "stack.h"
#include "list.h"
#include "parser.h"
#include "expression.h"

int main()
{
  TTable mujTable;
  table = &mujTable;
  tableInit(table);

  tableInsertFunction(table, strCreateString("fce"));
  functionInsertVar(table->lastAddedFunc, strCreateString("x"));
  functionInsertVar(table->lastAddedFunc, strCreateString("y"));
  functionInsertVar(table->lastAddedFunc, strCreateString("z"));

   printf("\nJedna funkce: \n");
   tablePrintOrder(*table);
   printf("\n----------------------------\n");


 //tiskniPrecTab();
 FILE *f = fopen("testy/test-expr2.txt","r");
 setSourceFile(f);
 strInit(&attr);
 listInit(&table->lastAddedFunc->tmpVar);

 int err = EOK;
 TVar *x;
 int test = 1;

 while((token = getNextToken(&attr)) != END_OF_FILE)
 {
  err = parseExpression(table, &x);
  printf("Test %d skoncil s chybou: %d a vysledkem: %d \n", test,err, (int)x);
  test++;
  err = EOK;
 }
 tiskniList(&table->lastAddedFunc->instructions);

 listDataDelete(&table->lastAddedFunc->tmpVar);
 listDispose(&table->lastAddedFunc->tmpVar);

 fclose(f);
 tableClear(table);
 strFree(&attr);
 return EXIT_SUCCESS;
}
