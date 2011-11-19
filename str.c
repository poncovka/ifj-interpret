/*
 * @description   Knihovna pro práci s datovým typem string
 * @author        pøevzato z jednoduchy_interpret/str.c
                  Vendula Poncová - xponco00
 *                Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#include "str.h"

/*
 * Funkce vytvoøí prázdný string
 * o velikosti STR_LEN_INC
 * @param   ukazatel na string
 * @return  chybový kód
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
 * Funkce vytvoøí prázdný string
 * o velikosti dané parametrem.
 * @author  Vendula Poncová
 * @param   ukazatel na string
 * @param   delka noveho retezce
 * @return  chybový kód
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
 * Funkce uvolní øetìzec z pamìti
 * a jeho hodnoty vynuluje.
 * @param   ukazatel na string
 */
void strFree(string *s) {
  free(s->str);
  s->length = 0;
  s->allocSize = 0;
}

/*
 * Funkce "vyma¾e" obsah øetìzce.
 * Velikost alokovaného prostoru se nezmìní.
 * @param   ukazatel na string
 */
void strClear(string *s) {
   s->str[0] = '\0';
   s->length = 0;
}

/*
 * Funkce vrací true, pokud øetìzec ukazuje na NULL
 * @param   true/false
 */
int strIsNull (string *s) {
   return s->str == NULL;
}

/*
 * Funkce pøidá na konec øetìzce jeden znak.
 * Pokud pamì» nestaèí, provede realokaci.
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
 * Funkce vytvoøí nový øetìzec a zkopíruje
 * do nìj øetìzec ze stringu.
 * @author  Patrik Hronský
 * @param   ukazatel na string
 * @return  øetìzec
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
 * Funkce pøekopíruje do stringu s1 obsah stringu s2
 * Pokud pamì» nestaèí, provede realokaci.
 * @param   ukazatel na string, KAM kopírujeme
 * @param   ukazatel na string, ODKUD kopírujeme
 * @param   chybový kód
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
 * Funkce porovná dva stringy a vrátí výsledek.
 * @param   ukazatel na string
 * @param   ukazatel na string
 * @param   integer, výsledek porovnání
 */
int strCmpString(string *s1, string *s2) {
   return strcmp(s1->str, s2->str);
}

/*
 * Funkce provede konkatenaci dvou stringù 
 * a vrátí nový string.
 * @author  Vendula Poncová
 * @param   ukazatel na string
 * @param   ukazatel na string
 * @param   výsledný string
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
 * Funkce vytvoøí a vrátí nový string, jeho¾
 * øetìzcem bude kopie zadaného.
 * Je tøeba uvolnit voláním strFree() !
 * @author  Vendula Poncová
 * @param   øetìzec, který se zkopíruje
 * @return  nový string
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
 * Funkce vytvoøí a vrátí nový string, jeho¾
 * øetìzec bude ukazovat na zadaný.
 * Øetìzec nelze mìnit. Nemusí se uvolòovat.
 * @author  Vendula Poncová
 * @param   øetìzec, který se zkopíruje
 * @return  nový string
 */
string strCreateConstString (char *str) {
  string s;
  s.length = strlen(str);
  s.allocSize = s.length + 1;
  s.str = str;

  return s;
}

/* BUDE SMAZÁNO

int strCopyString(string *s1, string *s2) {

      // uvolní pùvodní øetìzec
   strFree(s1);

   // inicializuje a alokuje na novou délku
   if (strInitLen(s1, s2->length) == STR_SUCCESS){

     // zkopíruje obsah druhého stringu
     strcpy(s1->str, s2->str);
     s1->length = s2->length;

     return STR_SUCCESS;
   }
   else return STR_ERROR;

}
*/

/* BUDE SMAZÁNO

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
