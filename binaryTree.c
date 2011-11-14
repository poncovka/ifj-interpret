/*
 * @description   Prace s binarnim vyheldavacim stromem (AVL strom)
 * @author        Marek Salat - xsalat00
 * @projekt       IFJ11
 * @date
 */

#include "binaryTree.h"

/*
 * @return  vetsi ze dvou cisel
 */
int max(int L, int R){
   return L > R ? L : R;
}

//----------------------------------------------------------------------

void BTreeInit(TBTree *T, EBTreeDataType type){
   if(T == NULL)
      return;
   T->root = NULL;
   T->lastAdded = NULL;
   T->nodeCount = 0;
   T->type = type;
}
//----------------------------------------------------------------------

void deleteNode(TNode n){
   if(n != NULL){
      deleteNode(n->left);
      deleteNode(n->right);
      free(n);
   }
}

void BTreeDelete(TBTree *T){
   if(T == NULL)
      return;
   deleteNode(T->root);
   BTreeInit(T, T->type);
}

//----------------------------------------------------------------------

TNode search(TNode T, char *key){
   if(T == NULL)
      return NULL;

   int cmpResult = strcmp(key, T->key);
   if ( cmpResult < 0)
      return search(T->left, key);
   else if (cmpResult > 0)
      return search(T->right, key);
   else
      return T;

}

TNode BTreeSearch(TBTree *T, char *key){
   if(T == NULL || key == NULL)
      return NULL;
   return search(T->root, key);
}
//----------------------------------------------------------------------

// slouzi jako pomocna promena funkce BTreeInsert,
// abych po vlozeni prvku mohl nastavi T->lastAdded
TNode position = NULL;
// pomocna promena ktera pomuze funkci BTreeInsert urcit navratovy kod funkce Insert
int   returnValue = INS_OK;

int BTreeInsert(TBTree *T, char *key, void *data){
   if(T == NULL)
      return INS_TREE_NULL;
   if(key == NULL)
      return INS_KEY_NULL;

   returnValue = INS_OK;
   T->root = insert(T->root, key, data);

   if(returnValue != INS_OK)  // insert meni returnValue proto jej kontroluju
      return returnValue;

   T->lastAdded = position;
   T->nodeCount++;
   return INS_OK;
}
//----------------------------------------------------------------------

TNode insert(TNode T, char *key, void *data){
   if(T == NULL){
      // nalezl jsem misto kam vlozit uzel
      T = malloc(sizeof(struct TBTreeNode));
      if(T == NULL){
         returnValue = INS_MALLOC;  // globalni promena
         return T;
      }
      // inicializace uzlu
      position = T;                 // globalni promena
      T->left = NULL;
      T->right = NULL;
      T->height = 0;
      T->data = data;
      T->key = key;
      return T;
   }
   else{
      int cmpResult = strcmp(key, T->key);
      if (cmpResult < 0){
         // je-li klic mensi nez jaky je v uzlu, bez do leveho podstromu
         T->left = insert(T->left, key, data);

         // kontrola zda neni nejaka vetev vic nez o jedna
			/*if( height( T->left ) - height( T->right ) == 2 ){
         // jeden podstrom musi byt vetsi a tedy budem rotovat
            // pokud je klic mensi nez klic leveho podstromu
				if( strcmp(key, T->left->key) < 0)
               // jsme vlevo a rotujeme jednou
					T = rotationLeft( T );
				else
               // jsme v evo a rotujeme dvakrat
					T = doubleRotationLeft( T );
			}*/
      }
      else if (cmpResult > 0){
         // je-li klic vetsi nez jaky je v uzlu, bez do praveho podstromu
         T->right = insert(T->right, key, data);

         // kontrola zda neni nejaka vetev vic nez o jedna
         /*if( height( T->right ) - height( T->left ) == 2 ){
         // jeden podstrom musi byt vetsi a tedy budem rotovat
				if( strcmp(key, T->right->key) > 0)
            // pokud je klic mensi nez klic praveho podstromu
               //jsme vpravo a rotujeme jednou
					T = rotationRight( T );
				else
               //jsme vpravo a rotujeme dvakrat
					T = doubleRotationRight( T );
			}*/
      }
      else{
         // oba klice jsou stejne, uloz chybu a ukonci se
         // uzivatel sam rozhodne jak se v teto situaci zachovat
         returnValue = INS_NODE_EXIST;
         return T;
      }
      // aktualizuje vysku stromu
      T->height = max( height( T->left ), height( T->right ) ) + 1;
   }
   return T;
}
//----------------------------------------------------------------------

TNode rotationLeft (TNode C){
   /*
    *      C           B
    *     /           / \
    *    B      =>   A   C
    *   /
    *  A
    *
    */

   TNode B = C->left;   // B z obrazku
   C->left= B->right;   // nesmim zapomenout pripojit vsechno co bylo u B v pravo k C do leva
   B->right = C;

   C->height = max(height(C->left), height(C->right))  + 1;
   B->height = max(height(B->left), C->height) + 1;

   return B;
}

TNode rotationRight (TNode A){

   /*
    *  A
    *   \             B
    *    B     =>    / \
    *     \         A   C
    *      C
    */

   TNode B = A->right;  // B z obrazku
   A->right = B->left;  // nesmim zapomenout pripojit vsechno co bylo u B vlevo k A do prava
   B->left = A;

   A->height = max(height(A->left), height(A->right))   + 1;
   B->height = max(height(B->right), A->height) + 1;

   return B;
}

TNode doubleRotationLeft(TNode C){
   /*
    *
    *     C           C
    *    /           /         B
    *   A      =>   B    =>   / \
    *    \         /         A   C
    *     B       A
    */

   C->left = rotationRight(C->left);
   return rotationLeft(C);
}

TNode doubleRotationRight(TNode A){
   /*
    *
    *    A           A
    *      \          \          B
    *       C   =>     B   =>   / \
    *      /            \      A   C
    *     B              C
    */

   A->right = rotationLeft(A->right);
   return rotationRight(A);
}

int height(TNode T){
   if(T == NULL)
      return -1;
   else
      return T->height;
}

