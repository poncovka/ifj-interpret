//jednoducha knihovna pro praci s nekonecne dlouhymi retezci

#include "str.h"

char *strCopyChar(string *s)
// funkcia vytvori novy retazec
// a skopiruje don retazec zo str
{
   char *strNew;
   if((strNew = (char *) malloc(sizeof(char)*(s->length + 1))) == NULL)
      return NULL;
   strcpy(strNew,s->str);
   return strNew;
}

int strInit(string *s)
// funkce vytvori novy retezec
{
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return STR_SUCCESS;
}

void strFree(string *s)
// funkce uvolni retezec z pameti
{
   free(s->str);
}

void strClear(string *s)
// funkce vymaze obsah retezce
{
   s->str[0] = '\0';
   s->length = 0;
}

int strAddChar(string *s1, char c)
// prida na konec retezce jeden znak
{
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

int strCopyString(string *s1, string *s2)
// prekopiruje retezec s2 do s1
{
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

int strCmpString(string *s1, string *s2)
// porovna oba retezce a vrati vysledek
{
   return strcmp(s1->str, s2->str);
}

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

string strConcatenation (string *s1, string *s2) {
// vrací konkatenaci dvou stringů

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

string strCreateString (char *str)
// vrati naplnenou strukturu string
{

  string s;
  s.length = strlen(str);
  s.allocSize = s.length + 1;
  s.str = str;

  return s;
}

/* konec souboru str.c */
