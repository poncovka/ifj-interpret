/*
 * @description   Knihovna pro pr�ci s datov�m typem string
 * @author        p�evzato z jednoduchy_interpret/str.c
                  Vendula Poncov� - xponco00
 *                Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#include "str.h"

/*
 * Funkce vytvo�� pr�zdn� string
 * o velikosti STR_LEN_INC
 * @param   ukazatel na string
 * @return  chybov� k�d
 */
int strInit(string *s) {

   if ((s->str = (char*) malloc(sizeof(char)*STR_LEN_INC)) == NULL)
      return STR_ERROR;

   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;

   return STR_SUCCESS;
}

/*
 * Funkce p�ekop�ruje do stringu s1 obsah stringu s2
 * Pokud pam� nesta��, provede realokaci.
 * @param   ukazatel na string, KAM kop�rujeme
 * @param   ukazatel na string, ODKUD kop�rujeme
 * @param   chybov� k�d
 */
int strCopyString(string *s1, string *s2) {

   int newLength = s2->length;
   if (newLength >= s1->allocSize) {
      // pamet nestaci, je potreba provest realokaci
      s1->allocSize = newLength + 1;
      char *oldPtr = s1->str;
      if ((s1->str = (char*) realloc(s1->str, sizeof(char)*s1->allocSize)) == NULL) {
         free(oldPtr);
         return STR_ERROR;
      }
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}


/*
 * Funkce provede konkatenaci dvou string�
 * a vr�t� nov� string.
 * @author  Vendula Poncov�
 * @param   ukazatel na string
 * @param   ukazatel na string
 * @param   v�sledn� string
 */
string strConcatenation (string *s1, string *s2) {

   string s;
   s.length = s1->length + s2->length;
   s.allocSize = s.length + 1;

   if ( (s.str = (char*)malloc(sizeof(char)*s.allocSize)) != NULL ) {

      strcpy(s.str, s1->str);
      strcpy(&s.str[s1->length], s2->str);

   } else s.str = NULL;

   return s;
}

/*
 * Funkce vytvo�� a vr�t� nov� string, jeho�
 * �et�zec bude ukazovat na zadan�.
 * �et�zec nelze m�nit. Nemus� se uvol�ovat.
 * @author  Vendula Poncov�
 * @param   �et�zec, kter� se zkop�ruje
 * @return  nov� string
 */
string strCreateConstString (char *str) {
   string s;
   s.length = strlen(str);
   s.allocSize = s.length + 1;
   s.str = str;

   return s;
}

/*
 * Funkce na�te n znak� a vr�t� je ve
 * vytvo�en�m �et�zci.
 * @author  Vendula Poncov�
 * @param   vstupn� soubor
 * @param   po�et na�ten�ch znak�
 * @return  nov� string
 */
string strReadNChar(FILE *f, int n) {

   // inicializujeme a alokujeme string
   string s = {NULL, 0, 0};

   // na�teme a zkop�rujeme znaky:
   if (n >= 0 && strInitLen(&s, n) == STR_SUCCESS) {
      int i, c;
      for (i = 0; (i < n) && ((c = fgetc(f)) != EOF); i++) {
         s.str[i] = (char)c;
      }
      s.str[i] = '\0';
      s.length = i;
   }

   return s;
}

/*
 * Funkce na��t� znaky do konce ��dku
 *  a vr�t� je ve vytvo�en�m �et�zci.
 * @author  Vendula Poncov�
 * @param   vstupn� soubor
 * @return  nov� string
 */
string strReadLine(FILE *f) {
   int err = STR_SUCCESS;

   // inicializujeme a alokujeme string
   string s = {NULL, 0, 0};
   err = strInit(&s);

   // na�teme a zkop�rujeme znaky:
   if (err == STR_SUCCESS) {
      int c;
      while(((c = fgetc(f)) != EOF) && (c !='\n')) {
         err = strAddChar(&s, c);
      }
      // pokud do�lo k chyb�, uvoln�me data
      if (err == STR_ERROR) {
         strFree(&s);
      }
   }

   return s;
}

/*
 * Funkce na��t� znaky do konce souboru (EOF)
 *  a vr�t� je ve vytvo�en�m �et�zci.
 * @author  Vendula Poncov�
 * @param   vstupn� soubor
 * @return  nov� string
 */
string strReadAll(FILE *f) {
   int err = STR_SUCCESS;

   // inicializujeme a alokujeme string
   string s = {NULL, 0, 0};
   err = strInit(&s);

   // na�teme a zkop�rujeme znaky:
   if (err == STR_SUCCESS) {
      int c;
      while(((c = fgetc(f)) != EOF)) {
         err = strAddChar(&s, c);
      }
      // pokud do�lo k chyb�, uvoln�me data
      if (err == STR_ERROR) {
         strFree(&s);
      }
   }

   return s;
}

/* konec souboru str.c */
