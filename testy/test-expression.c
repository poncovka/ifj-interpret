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
#include "expression.h"


int main()
{
  tableInit(&table);

  tableInsertFunction(&table, strCreateString("fce"));
  functionInsertVar(table.lastAddedFunc, strCreateString("x"));
  functionInsertVar(table.lastAddedFunc, strCreateString("y"));
  functionInsertVar(table.lastAddedFunc, strCreateString("z"));

   printf("\nJedna funkce: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");


 //tiskniPrecTab();
 FILE *f = fopen("testy/test-expr.txt","r");
 setSourceFile(f);
 strInit(&attr);
 listInit(&table.lastAddedFunc->tmpVar);

 int err = EOK;

 while((token = getNextToken(&attr)) != END_OF_FILE)
 {
  err = parseExpression(&table);
  printf("Test skonèil s chybou: %d\n\n", err);
 }
 listDataDelete(&table.lastAddedFunc->tmpVar);
 listDispose(&table.lastAddedFunc->tmpVar);

 fclose(f);
 tableClear(&table);
 strFree(&attr);
 return EXIT_SUCCESS;
}
