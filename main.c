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
  FILE *f;

  if (argc == 1) {
    fprintf(stderr,"ERROR: neni zadan vstupni soubor\n");
    return -INTR_ERR;
  }
  if ((f = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr,"ERROR: vstupni soubor se nepodarilo otevrit\n");
    return -INTR_ERR;
  }

  TTable table;
  tableInit(&table);
  setSourceFile(f);

  int err = parser(&table);
  if (err == PRS_OK){
   err = interpret(table.lastAddedFunc);
   countOfRows = -1;
  }

  if(err != PRS_OK){
    fprintf(stderr,"\n\n----------------------------------------------------------------------\n\n");
    switch(err){
      case LEX_ERR: fprintf(stderr,"Lexikalni chyba: ");break;
      case SYN_ERR: fprintf(stderr,"Syntakticka chyba: ");break;
      case SEM_ERR: fprintf(stderr,"Semanticka chyba: ");break;
      case RUN_ERR: fprintf(stderr,"Interpretacni chyba");break;
      case INTR_ERR:fprintf(stderr,"Interni chyba: ");break;
    }
    if(countOfRows == -1)
      printf("%s", err == RUN_ERR ? "" : "Pri interpretaci");
   else
      printf("Blizko radku %d", countOfRows);
    fprintf(stderr,"\n\n----------------------------------------------------------------------\n");
  }

  // ---- DEBUG ----
  // /*
    FILE *log = fopen("debug.log", "w");
    tablePrintOrder(table, log);
    fclose(log);
  // */
  // -- END DEBUG --

  fclose(f);
  tableClear(&table);
  return -err;
}
