/*
 * @description   Razeni retezce metodou merge-sort
 * @author        Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* skopiruju retezec do pomocneho pole nebo naopak */
void copyArray(char *dest, char *src, int strLen)
{
    for(int x=0;x<strLen;x++)
    {
      dest[x]=src[x];
    }
}

void mergeSort(char *str)
{
    int copied=0;   /* pomocna promenna, uchovava pocet prvku ktere jiz byli skopirovany do ciloveho pole */
    int strLen=strlen(str);    /* zjistim si delku retezce */
    char arr[2*strLen];      /* pomocne pole o dva krat velikosti retezce bez \0 */
    copyArray(arr,str,strLen);  /* skopiruju retezec bez \0 do leve casti pomocneho pole */

    int i,j,k,l,step,extra;
    bool endLeft,endRight,endMiddle,sourceLeft,sorted;
    sourceLeft=true;   /* pocatecni smer postupu - leve pole je zdroj, prave cil */
    do      /* dokud neni posloupnost serazena */
    {
      sorted=false;
      copied=0;
      if(sourceLeft)   /* jestli smer postupu je z leve casti pole do prave */
      {
        i=0;           /* zacatek pole */
        j=strLen-1;    /* konec leve casti pole */
        k=strLen;      /* zacatek prave casti pole */
        l=2*strLen-1;  /* konec pole */
      }
      else      /* jestli smer postupu je z prave casti pole do leve */
      {
        k=0;
        l=strLen-1;
        i=strLen;
        j=2*strLen-1;
      }
      step=1;      /* krok - smer postupu cilovym polem - zleva doprava o jedno */
      do
      {     /* dokud se i a j (i-index zdrojove casti pole od zacatku, j-index zdrojove casti pole od konce) nesetkaji a dokud nebyly skopirovany vsechny prvky zdrojoveho pole */
        endLeft=false;   /* konec neklesajici posloupnosti zleva */
        endRight=false;   /* konec neklesajici posloupnosti zprava */
        endMiddle=false;   /* setkani se i a j */
        do
        {   /* dokud neskoncila neklesajici posloupnost zleva, zprava, nebo dokud se i a j nesetkaji */
          if(arr[i]<arr[j])   /* jestli prvek v leve casti zdrojoveho pole je mensi nez prvek v prave casti */
          {
            arr[k]=arr[i];     /* vloz prvek do ciloveho pole */
            copied++;
            if(i==j)        /* jestli se i a j setkali */
            {
              endMiddle=true;
            }
            else            /* jestli se i a j jeste nesetkali */
            {
              i++;      /* posun i */
              k+=step;     /* posun k (index ciloveho pole) */
              if(arr[i]<arr[i-1])     /* jestli skoncila neklesajici posloupnost zleva */
              {
                endLeft=true;
              }
            }
          }
          else      /* jestli prvek v prave casti zdrojoveho pole je mensi nez prvek v leve casti */
          {
            arr[k]=arr[j];    /* vloz prvek do ciloveho pole */
            copied++;
            if(i==j)        /* jestli se i a j setkali */
            {
              endMiddle=true;
            }
            else            /* jestli se i a j jeste nesetkali */
            {
              j--;      /* posun j */
              k+=step;     /* posun k (index ciloveho pole) */
              if(arr[j]<arr[j+1])     /* jestli skoncila neklesajici posloupnost zprava */
              {
                endRight=true;
              }
            }
          }
        } while(!endLeft && !endRight && !endMiddle); /* dokud neskoncila neklesajici posloupnost zleva, zprava, nebo dokud se i a j nesetkaji */
        while(endLeft)   /* jestli nastal konec neklesajici posloupnosti zleva */
        {               /* postupne vkladej zbytek prave neklesajici posloupnosti do ciloveho pole */
          arr[k]=arr[j];
          copied++;
          j--;
          k+=step;
          endLeft=(arr[j]>=arr[j+1]);  /* jestli skoncila neklesajici posloupnost zprava, endLeft bude false a ukonci cyklus */
        }
        while(endRight)   /* jestli nastal konec neklesajici posloupnosti zprava */
        {               /* postupne vkladej zbytek leve neklesajici posloupnosti do ciloveho pole */
          arr[k]=arr[i];
          copied++;
          i++;
          k+=step;
          endRight=(arr[i]>=arr[i-1]);  /* jestli skoncila neklesajici posloupnost zleva, endRight bude false a ukonci cyklus */
        }
        extra=k;      /* prehozeni k a l (k-index ciloveho pole zleva, l-index cil. pole zprava) */
        k=l;
        l=extra;
        step=-step;       /* obraceni polarity postupu cilovym polem */
      } while(i!=j || copied!=strLen);    /* dokud se i a j (i-index pole od zacatku, j-index pole od konce) nesetkaji a dokud nebyly skopirovany vsechny prvky zdrojoveho pole */
      sourceLeft=!sourceLeft;     /* zmena smeru postupu (prehozeni zdrojoveho a ciloveho pole) */
        if(!sourceLeft)    /* jestli bude v dalsim kole zdrojove pole prave a cilove leve */
        {
          for(int x=0;x<strLen;x++)     /* skopiruju cilove pole do zdrojoveho (kvuli nasledujici kontrole serazeni) */
          {
            arr[x]=arr[x+strLen];
          }
        }
      sorted=true;
      for(int x=0;x<strLen-1;x++)        /* kontrola serazeni */
      {
        if(arr[x]>arr[x+1])
          sorted=false;
      }
    } while(sorted==false);     /* dokud neni posloupnost v levem poli serazena */

    copyArray(str,arr,strLen);  /* skopiruju jiz serazenou posloupnost znaku z pomocneho pole na adresu puvodniho retezce */
}
