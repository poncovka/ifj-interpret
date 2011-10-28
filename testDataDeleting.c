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

typedef struct{
   TBTree *variables;
   TBTree *constants;
   char   *name;
   int    lounchCnt;
}TableFunctions;

void avlPrint(TNode, int);
void avlPrintOrder(TNode);

int main()
{
   int cnt = 1;

      TBTree varTabFunc1, varTabFunc2, varTabFunc3;
      BTreeInit(&varTabFunc1); varTabFunc1.type = VARIABLES;
      BTreeInit(&varTabFunc2); varTabFunc2.type = VARIABLES;
      BTreeInit(&varTabFunc3); varTabFunc3.type = VARIABLES;

      printf("\n\nPromene func%d\n\n", cnt++);
      BTreeInsert(&varTabFunc1, "var1_func1");
      BTreeInsert(&varTabFunc1, "var2_func1");
      BTreeInsert(&varTabFunc1, "var3_func1");
      avlPrint(varTabFunc1.root, 0);

      printf("\n\nPromene func%d\n\n", cnt++);
      BTreeInsert(&varTabFunc2, "var1_func2");
      BTreeInsert(&varTabFunc2, "var2_func2");
      BTreeInsert(&varTabFunc2, "var3_func2");
      avlPrint(varTabFunc2.root, 0);

      printf("\n\nPromene func%d\n\n", cnt++);
      BTreeInsert(&varTabFunc3, "var1_func3");
      BTreeInsert(&varTabFunc3, "var2_func3");
      BTreeInsert(&varTabFunc3, "var3_func3");
      avlPrint(varTabFunc3.root, 0);

   TableFunctions func1 = {&varTabFunc1, NULL, "func1", 0};
   TableFunctions func2 = {&varTabFunc2, NULL, "func2", 0};
   TableFunctions func3 = {&varTabFunc3, NULL, "func3", 0};

   TBTree funcTab;
   BTreeInit(&funcTab); funcTab.type = FUNCIONS;

   printf("\n\n--------------\n\n");


   BTreeInsert(&funcTab, "func1"); funcTab.lastAdded->data = &func1;
      printf("\n\nnaposledy pridana funkce: %s\n\n\tobsahuje tyto promene: \n\n", funcTab.lastAdded->key);
      // data ukazuji na void, ale ja vim ze tyto data jsou typu TableFunctions, prot musim pretypovat
      avlPrint(((TableFunctions *)funcTab.lastAdded->data)->variables->root,5);

   BTreeInsert(&funcTab, "func2"); funcTab.lastAdded->data = &func2;
      printf("\n\nnaposledy pridana funkce: %s\n\n\tobsahuje tyto promene: \n\n", funcTab.lastAdded->key);
      avlPrint(((TableFunctions *)funcTab.lastAdded->data)->variables->root,5);

   BTreeInsert(&funcTab, "func3"); funcTab.lastAdded->data = &func3;
      printf("\n\nnaposledy pridana funkce: %s\n\n\tobsahuje tyto promene: \n\n", funcTab.lastAdded->key);
      avlPrint(((TableFunctions *)funcTab.lastAdded->data)->variables->root,5);


   printf("\n\ntabulka funci obsahuje tyto funkce:\n");
   avlPrint(funcTab.root,0);

      BTreeDeleteWithData(&varTabFunc1);
      BTreeDeleteWithData(&varTabFunc2);
      BTreeDeleteWithData(&varTabFunc3);

   BTreeDeleteWithData(&funcTab);
   return 0;
}

void printSPace(int countOfSpace){
   for(int i = 0; i < countOfSpace-1; i++)
         printf("   ");
}
void avlPrint(TNode T, int countOfSpace){
   if(T != NULL){
      avlPrint(T->right, ++countOfSpace);

      printSPace(countOfSpace);
      printf("%s \n", T->key);

      avlPrint(T->left, countOfSpace);
   }
}


void avlPrintOrder(TNode T){
   if(T != NULL){
      avlPrintOrder(T->left);
      printf("%s\n", T->key);
      avlPrintOrder(T->right);
   }
}
