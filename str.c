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

   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
      return STR_ERROR;

   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;

   return STR_SUCCESS;
}

/*
 * Funkce vytvo�� pr�zdn� string
 * o velikosti dan� parametrem.
 * @author  Vendula Poncov�
 * @param   ukazatel na string
 * @param   delka noveho retezce
 * @return  chybov� k�d
 */
int strInitLen(string *s, int len) {

   // provede alokaci
   if (len > 0){
     if ((s->str = (char*) malloc(len + 1)) == NULL)
        return STR_ERROR;

     s->allocSize = len + 1;
     s->str[0] = '\0';
   }
   // bez alokace
   else {
    s->allocSize = 0;
    s->str = NULL;
   }

   s->length = 0;   
   return STR_SUCCESS;
}

/*
 * Funkce uvoln� �et�zec z pam�ti
 * a jeho hodnoty vynuluje.
 * @param   ukazatel na string
 */
void strFree(string *s) {
  free(s->str);
  s->length = 0;
  s->allocSize = 0;
}

/*
 * Funkce "vyma�e" obsah �et�zce.
 * Velikost alokovan�ho prostoru se nezm�n�.
 * @param   ukazatel na string
 */
void strClear(string *s) {
   s->str[0] = '\0';
   s->length = 0;
}

/*
 * Funkce vrac� true, pokud �et�zec ukazuje na NULL
 * @param   true/false
 */
int strIsNull (string *s) {
   return s->str == NULL;
}

/*
 * Funkce p�id� na konec �et�zce jeden znak.
 * Pokud pam� nesta��, provede realokaci.
 * @param   ukazatel na string
 * @param   znak
 */
int strAddChar(string *s1, char c) {

   if (s1->length + 1 >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
         return STR_ERROR;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

/*
 * Funkce vytvo�� nov� �et�zec a zkop�ruje
 * do n�j �et�zec ze stringu.
 * @author  Patrik Hronsk�
 * @param   ukazatel na string
 * @return  �et�zec
 */
char *strCopyChar(string *s) {

   char *strNew = NULL;
   if((strNew = (char *) malloc(sizeof(char)*(s->length + 1))) == NULL) {
      return NULL;
   }
   strcpy(strNew,s->str);
   return strNew;
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
   if (newLength >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}

/*
 * Funkce porovn� dva stringy a vr�t� v�sledek.
 * @param   ukazatel na string
 * @param   ukazatel na string
 * @param   integer, v�sledek porovn�n�
 */
int strCmpString(string *s1, string *s2) {
   return strcmp(s1->str, s2->str);
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

  }
  else s.str = NULL;

  return s;
}

/*
 * Funkce vytvo�� a vr�t� nov� string, jeho�
 * �et�zcem bude kopie zadan�ho.
 * Je t�eba uvolnit vol�n�m strFree() !
 * @author  Vendula Poncov�
 * @param   �et�zec, kter� se zkop�ruje
 * @return  nov� string
 */
string strCreateString (string *sCopy) {

  string s;
  s.length = sCopy->length;
  s.allocSize = s.length + 1;
  s.str = sCopy->str;
  s.str = strCopyChar(&s);

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

/* BUDE SMAZ�NO

int strCopyString(string *s1, string *s2) {

      // uvoln� p�vodn� �et�zec
   strFree(s1);

   // inicializuje a alokuje na novou d�lku
   if (strInitLen(s1, s2->length) == STR_SUCCESS){

     // zkop�ruje obsah druh�ho stringu
     strcpy(s1->str, s2->str);
     s1->length = s2->length;

     return STR_SUCCESS;
   }
   else return STR_ERROR;

}
*/

/* BUDE SMAZ�NO

int strCmpConstStr(string *s1, char* s2)
// porovna nas retezec s konstantnim retezcem
{
   return strcmp(s1->str, s2);
}

char *strGetStr(string *s)
// vrati textovou cast retezce
{
   return s->str;
}

int strGetLength(string *s)
// vrati delku daneho retezce
{
   return s->length;
}
*/

/* konec souboru str.c */
