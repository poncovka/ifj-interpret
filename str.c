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

   if ((s->str = (char*) malloc(sizeof(char)*STR_LEN_INC)) == NULL)
      return STR_ERROR;

   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;

   return STR_SUCCESS;
}

/*
 * Funkce vytvoøí prázdný string
 * o velikosti dané parametrem.
 * Pro len < 0 nealokuje nic,
 * pro len = 0 vznikne prazdny retezec,
 * jinak alokuje se rezetec dane delky.
 * @author  Vendula Poncová
 * @param   ukazatel na string
 * @param   delka noveho retezce
 * @return  chybový kód
 */
int strInitLen(string *s, int len) {

   // provede alokaci
   if (len >= 0){
     if ((s->str = (char*) malloc(sizeof(char)*(len + 1))) == NULL)
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
      s1->allocSize = s1->length + STR_LEN_INC;

      char *oldPtr = s1->str;
      if ((s1->str = (char*) realloc(s1->str, sizeof(char) * s1->allocSize)) == NULL) {
         free(oldPtr);
         return STR_ERROR;
      }
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
   if ((strNew = (char *) malloc(sizeof(char)*(s->length + 1))) == NULL) {
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
  s.str = strCopyChar(sCopy);

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

/*
 * Funkce naète n znakù a vrátí je ve
 * vytvoøeném øetìzci.
 * @author  Vendula Poncová
 * @param   vstupní soubor
 * @param   poèet naètených znakù
 * @return  nový string
 */
string strReadNChar(FILE *f, int n) {

  // inicializujeme a alokujeme string
  string s = {NULL, 0, 0};

  // naèteme a zkopírujeme znaky:
  if (n >= 0 && strInitLen(&s, n) == STR_SUCCESS){
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
 * Funkce naèítá znaky do konce øádku
 *  a vrátí je ve vytvoøeném øetìzci.
 * @author  Vendula Poncová
 * @param   vstupní soubor
 * @return  nový string
 */
string strReadLine(FILE *f) {
  int err = STR_SUCCESS;
  
  // inicializujeme a alokujeme string
  string s = {NULL, 0, 0};
  err = strInit(&s);

  // naèteme a zkopírujeme znaky:
  if (err == STR_SUCCESS){
    int c;
    while(((c = fgetc(f)) != EOF) && (c !='\n')){
      err = strAddChar(&s, c);
    }
    // pokud do¹lo k chybì, uvolníme data
    if (err == STR_ERROR) {
      strFree(&s);
    }
  }

  return s;
}

/*
 * Funkce naèítá znaky do konce souboru (EOF)
 *  a vrátí je ve vytvoøeném øetìzci.
 * @author  Vendula Poncová
 * @param   vstupní soubor
 * @return  nový string
 */
string strReadAll(FILE *f) {
  int err = STR_SUCCESS;
  
  // inicializujeme a alokujeme string
  string s = {NULL, 0, 0};
  err = strInit(&s);

  // naèteme a zkopírujeme znaky:
  if (err == STR_SUCCESS){
    int c;
    while(((c = fgetc(f)) != EOF)){
      err = strAddChar(&s, c);
    }
    // pokud do¹lo k chybì, uvolníme data
    if (err == STR_ERROR) {
      strFree(&s);
    }
  }

  return s;
}

/* konec souboru str.c */
