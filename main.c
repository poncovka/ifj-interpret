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
#include "interpret.h"

int main(int argc, char *argv[]){

   FILE *f = fopen(argv[1], "r");
   TTable table;
   tableInit(&table);

   setSourceFile(f);

   int err = parser(&table);

   if(err == PRS_OK)
      err = interpret(table.lastAddedFunc);

   if(err != PRS_OK){
     fprintf(stderr,"\n\n----------------------------------------------------------------------\n\n");
      switch(err){
         case LEX_ERR: fprintf(stderr,"Lexikalni chyba, radek: %d\n", countOfRows);break;
         case SYN_ERR: fprintf(stderr,"Syntakticka chyba, radek: %d\n", countOfRows);break;
         case SEM_ERR: fprintf(stderr,"Semanticka chyba, radek: %d\n", countOfRows);break;
         case RUN_ERR: fprintf(stderr,"Interpretacni chyba,  radek: %d\n", countOfRows);break;
         case INTR_ERR:fprintf(stderr,"Interni chyba, radek: %d\n", countOfRows);break;
      }
      fprintf(stderr,"\n----------------------------------------------------------------------\n");
   }

   // ---- DEBUG ----
      FILE *log = fopen("debug.log", "w");
      tablePrintOrder(table, log);
      fclose(log);
   // -- END DEBUG --

   fclose(f);

   tableClear(&table);
   return -err;
}
