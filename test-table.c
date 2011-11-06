/*
 * @description   test tabulkz funkci
 * @author        Marek Salat - xsalat00
 * @projekt       IFJ11
 * @date
 */

#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "str.h"
#include <string.h>

int main()
{
   TTable table;
   tableInit(&table);

   printf("\nTabulka by mela byt prazdna: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   tableInsertFunction(&table, strCreateString("func1"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var1func1"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var2func1"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var3func1"));

   printf("\nJedna funkce: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   tableInsertFunction(&table, strCreateString("func2"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var1func2"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var2func2"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var3func2"));

   printf("\nDve funkce: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   tableInsertFunction(&table, strCreateString("func3"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var1func3"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var2func3"));
      fuctionInsertVar(table.lastAddedFunc, strCreateString("var3func3"));

   printf("\nVsechny: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   // test heldani
   {

      TFunction *fceSearch;

      printf("\nObsahuje tabulka funkci %s? \t", "func1");
      fceSearch = tableSearchFunction(&table, strCreateString("func1"));
      if(fceSearch != NULL){
         printf("ANO\n");
         printf("   Obsahuje funkce promenou %s?\t", "var1func1");
         if(functionSearchVar(fceSearch, strCreateString("var1func1")) != NULL)
            printf("ANO");
         else
            printf("NE");
      }
      else
         printf("NE\n");

      printf("\nObsahuje tabulka funkci %s? \t", "funcX");
      fceSearch = tableSearchFunction(&table, strCreateString("funcX"));
      if(fceSearch != NULL){
         printf("ANO\n");
         printf("   Obsahuje funkce promenou %s?\t", "var1func1");
         if(functionSearchVar(fceSearch, strCreateString("var1func1")) != NULL)
            printf("ANO");
         else
            printf("NE");
      }
      else
         printf("NE\n");

      printf("\n----------------------------\n");
   }

   printf("\nSmazu: \n");
   tableClear(&table);
   tablePrintOrder(table);
   printf("\n----------------------------\n");
}
