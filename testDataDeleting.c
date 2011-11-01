/*
 * @description   test binarnoho vyheldavaciho stromu
 * @author        Marek Salat - xsalat00
 * @projekt       IFJ11
 * @date
 */

 /* 28.10.2011 TFunction je obsah jedne polozky v tabulce funkci, nejdriv sem vytvoril 3 stromy jako tabulku promenych
               potom sem zacal pridavat funkce do tabulky funci a jako data jsem dal jeden ze stromu promenych
               dale muzete videt jak se k takovym datum dostanu pokud vim jaky datovy typ predstavuji( v nasem pripade je to
               TFunction )
 */

#include <stdio.h>
#include <stdlib.h>
#include "binaryTree.h"
#include "table.h"
#include "str.h"

void printTreeNode(TNode, int);
void printTreeNodeOrder(TBTree*);
void tablePrintOrder(TTable);

int main()
{
   char *f1 = "func1";
   char *f2 = "func2";
   char *f3 = "func3";
   string func1; func1.str = f1;
   string func2; func2.str = f2;
   string func3; func3.str = f3;

   char *v1f1 = "var1func1";
   char *v2f1 = "var2func1";
   char *v3f1 = "var3func1";
   string var1func1; var1func1.str = v1f1;
   string var2func1; var2func1.str = v2f1;
   string var3func1; var3func1.str = v3f1;

   char *v1f2 = "var1func2";
   char *v2f2 = "var2func2";
   char *v3f2 = "var3func2";
   string var1func2; var1func2.str = v1f2;
   string var2func2; var2func2.str = v2f2;
   string var3func2; var3func2.str = v3f2;

   char *v1f3 = "var1func3";
   char *v2f3 = "var2func3";
   char *v3f3 = "var3func3";
   string var1func3; var1func3.str = v1f3;
   string var2func3; var2func3.str = v2f3;
   string var3func3; var3func3.str = v3f3;

   TTable table;
   tableInit(&table);

   tableInsertFunction(&table, &func1);
      fuctionInsertVar(table.lastAddedFunc, &var1func1);
      fuctionInsertVar(table.lastAddedFunc, &var2func1);
      fuctionInsertVar(table.lastAddedFunc, &var3func1);

   tableInsertFunction(&table, &func2);
      fuctionInsertVar(table.lastAddedFunc, &var1func2);
      fuctionInsertVar(table.lastAddedFunc, &var2func2);
      fuctionInsertVar(table.lastAddedFunc, &var3func2);

   tableInsertFunction(&table, &func3);
      fuctionInsertVar(table.lastAddedFunc, &var1func3);
      fuctionInsertVar(table.lastAddedFunc, &var2func3);
      fuctionInsertVar(table.lastAddedFunc, &var3func3);

   tablePrintOrder(table);
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


//             uzel     typ stromu       oddelovac
void printNode(TNode n, EBTreeDataType t, char *delim){
   if(n != NULL){
      printNode(n->left, t, delim);

      printf("%s%s\n", delim, n->key);
      switch(t){
         case FUNCIONS:{
               TBTree *temp   = &(((TFunction *)n->data)->variables);
               printf("     variables:\n");
               printNode( temp->root, temp->type /*VAR*/, "      " );
         }break;
         case VAR:{
            //fprintf(stderr, "\nTisk promene nebo konstanty neni implenetovan!");
         }break;
         case DEFAULT:
         default : break;
      }

      printNode(n->right, t, delim);
   }
}

void printTreeNodeOrder(TBTree *T){
   printNode(T->root, T->type, "   ");
}

void tablePrintOrder(TTable table){
   printTreeNodeOrder(&(table.functions)) ;
}
