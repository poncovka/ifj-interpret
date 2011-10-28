/*
 * @description   test binarnoho vyheldavaciho stromu
 * @author        Marek Salat - xsalat00
 * @projekt       IFJ11
 * @date
 */

 /* 28.10.2011 TableFunctions je obsah jedne polozky v tabulce funkci, nejdriv sem vytvoril 3 stromy jako tabulku promenych
               potom sem zacal pridavat funkce do tabulky funci a jako data jsem dal jeden ze stromu promenych
               dale muzete videt jak se k takovym datum dostanu pokud vim jaky datovy typ predstavuji( v nasem pripade je to
               TableFunctions )
 */

#include <stdio.h>
#include <stdlib.h>
#include "binaryTreeAVL.h"

void avlPrint(TNode, int);
void avlPrintOrder(TBTree*);

int main()
{
   int cnt = 1;

      TBTree varTabFunc1, varTabFunc2, varTabFunc3;
      BTreeInit(&varTabFunc1,DEFAULT);
      BTreeInit(&varTabFunc2,DEFAULT);
      BTreeInit(&varTabFunc3,DEFAULT);

      printf("\n\nPromene func%d\n\n", cnt++);
      BTreeInsert(&varTabFunc1, "var1_func1", NULL);
      BTreeInsert(&varTabFunc1, "var2_func1", NULL);
      BTreeInsert(&varTabFunc1, "var3_func1", NULL);
      avlPrintOrder(&varTabFunc1);

      printf("\n\nPromene func%d\n\n", cnt++);
      BTreeInsert(&varTabFunc2, "var1_func2", NULL);
      BTreeInsert(&varTabFunc2, "var2_func2", NULL);
      BTreeInsert(&varTabFunc2, "var3_func2", NULL);
      avlPrintOrder(&varTabFunc2);

      printf("\n\nPromene func%d\n\n", cnt++);
      BTreeInsert(&varTabFunc3, "var1_func3", NULL);
      BTreeInsert(&varTabFunc3, "var2_func3", NULL);
      BTreeInsert(&varTabFunc3, "var3_func3", NULL);
      avlPrintOrder(&varTabFunc3);

   TableFunctions func1 = {&varTabFunc1, NULL, "func1", 0}; // {tabulka promenych, tabulka konstant, klic, pocet spusteni}
   TableFunctions func2 = {&varTabFunc2, NULL, "func2", 0};
   TableFunctions func3 = {&varTabFunc3, NULL, "func3", 0};

   TBTree funcTab;                  // tabulka funkci
   BTreeInit(&funcTab, FUNCIONS);   // inicalizujeme ji jako tabulku funcki

   printf("\n\n--------------\n\n");


   BTreeInsert(&funcTab, "func1", &func1);   // do dat uylu pridam funkci
      printf("\n\nnaposledy pridana funkce: %s\n\n      obsahuje tyto promene: \n\n", funcTab.lastAdded->key);
      // data ukazuji na void, ale ja vim ze tyto data jsou typu ukazatel na TableFunctions, prot musim pretypovat
      //           (                func1                    )
      avlPrintOrder(((TableFunctions *)funcTab.lastAdded->data)->variables);

   BTreeInsert(&funcTab, "func2", &func2);
      printf("\n\nnaposledy pridana funkce: %s\n\n      obsahuje tyto promene: \n\n", funcTab.lastAdded->key);
      //           (                func2                    )
      avlPrintOrder(((TableFunctions *)funcTab.lastAdded->data)->variables);

   BTreeInsert(&funcTab, "func3", &func3);
      printf("\n\nnaposledy pridana funkce: %s\n\n      obsahuje tyto promene: \n\n", funcTab.lastAdded->key);
      //           (                func3                    )
      avlPrintOrder(((TableFunctions *)funcTab.lastAdded->data)->variables);


   printf("\n\ntabulka funci obsahuje tyto funkce + promene:\n");
   avlPrintOrder(&funcTab);

      //BTreeDelete(&varTabFunc1);
      //BTreeDelete(&varTabFunc2);
      //BTreeDelete(&varTabFunc3);

   BTreeDelete(&funcTab);  // funkce by mela smazat i vsechny podstromy
   return 0;
}
//             uzel     typ stromu       oddelovac
void printNode(TNode n, ETreeDataType t, char *delim){
   if(n != NULL){
      printNode(n->left, t, delim);

      printf("%s%s\n", delim, n->key);
      switch(t){
         case FUNCIONS:{
            printf("     variables:\n");
            printNode( ((TableFunctions *)n->data)->variables->root, ((TableFunctions *)n->data)->variables->type /*VAR_CONST*/, "      " );
         }break;
         case VAR_CONST:{
            fprintf(stderr, "\nTisk promene nebo konstanty neni implenetovan!");
         }break;
         case DEFAULT:
         default : break;
      }

      printNode(n->right, t, delim);
   }
}

void avlPrintOrder(TBTree *T){
   printNode(T->root, T->type, "   ");
}
