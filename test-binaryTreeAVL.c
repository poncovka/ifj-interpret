/*
 * @description   test binarnoho vyheldavaciho stromu
 * @author        Marek Salat - xsalat00
 * @projekt       IFJ11
 * @date
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
   TBTree strom;
   BTreeInit(&strom, DEFAULT);

   BTreeInsert(&strom, "a", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "b", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "c", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "d", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "e", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "f", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "g", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "h", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "i", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);
   BTreeInsert(&strom, "j", NULL); printf("naposled pridano: %s\n", strom.lastAdded->key);

   if(BTreeInsert(&strom, "a", NULL) == INS_NODE_EXIST)
      printf("\nPrvek s timto klicem \"a\"jiz existuje!\n\n");
   printf("naposled pridano: %s\n", strom.lastAdded->key);

   avlPrint(strom.root, 0);
   printf("\n\nroot: \t%s\npocet uzlu: \t%d \nnaposled pridany: \t%s\nnejdelsi vetev: \t%d\n", strom.root->key, strom.nodeCount, strom.lastAdded->key, strom.root->height);
   printf("velikost v B: %d\n", strom.nodeCount*sizeof(struct TBTreeNode));

   TNode node = BTreeSearch(&strom, "x"); printf("\nhledam \"x\" -> \"%s\"\n", node == NULL ? "nenalezeno" : node->key);
   node =       BTreeSearch(&strom, "3"); printf("\nhledam \"3\" -> \"%s\"\n", node == NULL ? "nenalezeno" : node->key);
   node =       BTreeSearch(&strom, "a"); printf("\nhledam \"a\" -> \"%s\"\n", node == NULL ? "nenalezeno" : node->key);

   //avlPrintOrder(strom.root);

   BTreeDelete(&strom); printf("zrusil sem strom \npocet uzlu %d\n\n", strom.nodeCount);
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
      printf(" %d|%s \n", T->height, T->key);

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
