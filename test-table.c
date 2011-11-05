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

string makeString(string *s, char *t){
   s->length = strlen(t);
   s->str = t;
   return *s;
}

int main()
{
   // priprava testovacich dat

   char *f1 = "func1";
   char *f2 = "func2";
   char *f3 = "func3";
   string func1 = makeString(&func1, f1);
   string func2 = makeString(&func2, f2);
   string func3 = makeString(&func3, f3);

   char *v1f1 = "var1func1";
   char *v2f1 = "var2func1";
   char *v3f1 = "var3func1";
   string var1func1 = makeString(&var1func1, v1f1);
   string var2func1 = makeString(&var2func1, v2f1);
   string var3func1 = makeString(&var3func1, v3f1);

   char *v1f2 = "var1func2";
   char *v2f2 = "var2func2";
   char *v3f2 = "var3func2";
   string var1func2 = makeString(&var1func2, v1f2);
   string var2func2 = makeString(&var2func2, v2f2);
   string var3func2 = makeString(&var3func2, v3f2);

   char *v1f3 = "var1func3";
   char *v2f3 = "var2func3";
   char *v3f3 = "var3func3";
   string var1func3 = makeString(&var1func3, v1f3);
   string var2func3 = makeString(&var2func3, v2f3);
   string var3func3 = makeString(&var3func3, v3f3);

   //-----------
   // tady to zacina :)

   TTable table;
   tableInit(&table);

   printf("\nTabulka by mela byt prazdna: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   tableInsertFunction(&table, &func1);
      fuctionInsertVar(table.lastAddedFunc, &var1func1);
      fuctionInsertVar(table.lastAddedFunc, &var2func1);
      fuctionInsertVar(table.lastAddedFunc, &var3func1);

   printf("\nJedna funkce: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   tableInsertFunction(&table, &func2);
      fuctionInsertVar(table.lastAddedFunc, &var1func2);
      fuctionInsertVar(table.lastAddedFunc, &var2func2);
      fuctionInsertVar(table.lastAddedFunc, &var3func2);

   printf("\nDve funkce: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   tableInsertFunction(&table, &func3);
      fuctionInsertVar(table.lastAddedFunc, &var1func3);
      fuctionInsertVar(table.lastAddedFunc, &var2func3);
      fuctionInsertVar(table.lastAddedFunc, &var3func3);

   printf("\nVsechny: \n");
   tablePrintOrder(table);
   printf("\n----------------------------\n");

   printf("\nSmazu: \n");
   tableClear(&table);
   tablePrintOrder(table);
   printf("\n----------------------------\n");
}
