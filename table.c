/*
void deleteNode(TNode n, EBTreeDataType type){
   if(n != NULL){
      deleteNode(n->left,  type);
      deleteNode(n->right, type);

      // tady uz muzu smazat polozku
      switch(type){
         // predpis jak smazat data u funkce pokud jsou typu TFunction*
         case FUNCIONS:{
            TBTree *temp = ((TFunction *)n->data)->variables;
            deleteNode( temp->root, temp->type); // type by mel byt VAR_CONST
            //free(temp);  // data u funkce sem asi taky alokoval proto je mazu
            // smazat konstanty
            // smazat seznam instrukci
         }break;
         // predpis jak smazat data poku jsou typu xxx
         case VAR_CONST:{
            fprintf(stderr, "\nMazani konstanty neni implementovano!");
         }break;
         // nic nedelam
         case DEFAULT:
         default: break;
      }
      free(n);
   }
}
*/
