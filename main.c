/*
 * @description   Hlavni program - intepretace jazyka IFJ11
 * @author        Marek Salat - xsalat00
 * @projekt       IFJ11
 * @date
 */

#include <stdio.h>
#include <stdlib.h>

#include "table.h"
#include "str.h"
#include "scanner.h"
#include "parser.h"

int main(){
   FILE *f = fopen("../ifj-projekt/testy/test.ifj11", "r");
   TTable table;
   tableInit(&table);

   setSourceFile(f);

   int err = parser(&table);

   switch(err){
      case PRS_OK: break;
      case LEX_ERR: printf("Lexikalni chyba, radek: %d\n", countOfRows);break;
      case SYN_ERR: printf("Syntakticka chyba, radek: %d\n", countOfRows);break;
      case SEM_ERR: printf("Semanticka chyba, radek: %d\n", countOfRows);break;
      case RUN_ERR: printf("Interpretacni chyba,  radek: %d\n", countOfRows);break;
      case INTR_ERR:printf("Interni chyba, radek: %d\n", countOfRows);break;
   }
   FILE *log = fopen("log", "w");
   tablePrintOrder(table, log);
   fclose(log);

   tableClear(&table);
   fclose(f);

   return -err;
}
