/*
 * @description   Knihovna vestav�n�ch funkc�
 * @author        Vendula Poncov� - xponco00
 *                Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#include "library.h"

/*
 * Inicializuje se z�sobn�k. Ukazatel na NULL vyvol� chybu.
 * @param   ukazatel na z�sobn�k
 * @return  k�d chyby
 */

const char *dataTypeStrings[]= {
 [NIL] ="nil", 
 [BOOL]="bool", 
 [STRING]="string",
 [NUMBER]="number",  
};

/////////////////////////////////////////////////////////////////////////////////// vestaven� funkce:

/*
 * Funkce vrac� �et�zec obsahuj�c� datov� typ dat.
 * @param   ukazatel na data, jejich� typ chceme zn�t
 * @return  data STRING obsahuj�c� �et�zec s datov�m typem
 */
TVarData type(TVarData *param) {

  TVarData data;
  data.value.s = strCreateString((char*) dataTypeStrings[param->type]);
  data.type = STRING;
  return data;
}

/*
 * Funkce vrac� pod�et�zec zadan�ho �et�zce.
 * Pokud jsou parametry nespr�vn�ho typu, vrac� NIL.
 * Pokud se nepoda�� alokace �et�zce, vrac� chybov� k�d do err.
 * @param   ukazatel na data STRING
 * @param   ukazatel na data NUMBER
 * @param   ukazatel na data NUMBER
 * @param   ukazatel chybu
 * @return  data STRING
 */
TVarData substr(TVarData *dataS, TVarData *dataFrom, TVarData *dataTo, int *err) {

  TVarData data;
  data.type = NIL;

  // kotrola parametr�:
  if (dataS->type == STRING && dataFrom->type == NUMBER && dataTo->type == NUMBER) {

    int from = dataFrom->value.n;
    int to   = dataTo->value.n;
    char *s  = dataS->value.s.str;
    //int srcLen = dataS->value.s.length;

    // p�epo��t�n� index� - DOKON�IT
    int destLen = to - from + 1;

    // alokace nov�ho �et�zce
    if(strInitLen(&data.value.s, destLen) == EOK) {
      data.type = STRING;

      // kop�rov�n�
      strncpy(data.value.s.str, &s[from-1], destLen);
      data.value.s.str[destLen] = '\0';
    }
    else *err = ERR;

  }
  return data;
}

/*
 * Funkce vyhled� prvn� v�skyt pod�et�zce sPattern v �et�zci s
 * @param   ukazatel na data STRING, v tomto �et�zci vyhled�v�me
 * @param   ukazatel na data STRING, tento �et�zec hled�me
 * @return  data typu NIL, BOOL nebo NUMBER
 */
TVarData find(TVarData *sData, TVarData *sPatternData) {

  TVarData data;
  data.type = NIL;  

  // kontrola parametr�
  if (sData->type == STRING && sPatternData->type == STRING) {

    // vyhled�v�n� pod�et�zce
    int position = kmp (sData->value.s.str, sPatternData->value.s.str);
    if (position < 0) {
      data.type = BOOL;
      data.value.b = FALSE;
    }
    else {
      data.type = NUMBER;
      data.value.n = position;
    }
  }
  return data;
}

/*
 * Se�ad� znaky v dan�m �et�zci podle ord. hodnot.
 * @param   ukazatel na data STRING
 * @param   ukazatel na chybu
 * @return  data STRING se set��zen�m �et�zcem
 */
TVarData sort(TVarData *sData, int *err) {

  TVarData data;
  data.type = NIL;

  // kontrola parametr�:
  if (sData->type == STRING) {

    data.type = STRING;
    strInitLen(&data.value.s, 0);
    *err = strCopyString(&data.value.s, &sData->value.s);
    // set��zen�:
    if (*err == EOK) mergeSort(data.value.s.str);
  }

  return data;
}

/////////////////////////////////////////////////////////////////////////////////// funkce KMP:

/*
 * Funkce pro nalezen� pod�et�zce v �et�zci.
 * Nejsp� se z n� stane funkce sort(string,string).
 * @param   prohled�van� �et�zec
 * @param   hledan� �et�zec
 * @return  pozice prvn�ho v�skytu vzoru v textu
 *          pokud vzor nen� nalezen, vr�t� se -1
 */
int kmp (char *text, char *vzor) {

  // vzor je pr�zdn� �et�zec, vra� nulu
  if (strlen(vzor) == 0) return 0;
  
  int dVzoru = strlen(vzor);
  int vektor[dVzoru];

  // nalezen� fail vektoru pro vzor
  kmpVektor(vzor, vektor);

  // vra� index vzoru v textu
  return kmpHledej(text, vzor, vektor);
}

/*
 * Pro dan� vzor najde p�echody mezi stavy KMP automatu 
 * a zap�e je do vektoru FAIL.
 * @param   vzorov� �et�zec
 * @param   pole integer� - vektor
 */
void kmpVektor (char *vzor, int vektor[]) {

  int delka = strlen(vzor);  // d�lka �et�zce vzor
  int stav;                  // aktu�ln� pozice ve vzoru (index)
  int prechod;               // n�vratov� index pro dan� stav (hodnota v poli vektor)

  vektor[0] = -1;            // z nult�ho stavu n�vrat do po��te�n�ho stavu "-1" 
                             // stav "-1" existuje pouze teoreticky

  for (stav = 1; stav < delka; stav++) { // pro ka�dou pozici ve vzoru:

    prechod = vektor[stav-1];            // ulo� p�echod p�edchoz�ho stavu

    while ( (prechod > -1) && (vzor[prechod] != vzor[stav-1]) ) {

      /* P�echod nevede do po��te�n�ho stavu a znak ve stavu, do kter�ho
         se vrac�me, je r�zn� od znaku na p�edchoz� pozici.
         Provedeme n�vrat pomoc� p�echodu na ni��� pozici
         a ulo��me si nov� p�echod.
      */
      prechod = vektor[prechod];
    }
    
    /* P�echod vede do po��te�n�ho stavu (nen� tedy kam se d�l posunovat)
       nebo porovn�van� znaky se rovnaly.
       P�i��t�me 1, nebo� nalezen� "suffix", na kter� se lze p�esunout
       roz�i�ujeme o nov� znak.
    */
    vektor[stav] = prechod + 1;
  }
}

/*
 * Vyhled�v� pod�et�zec vzor v �et�zci text pomoc�
 * KMP algoritmu. Pozice se ��sluje od 1.
 * @param   prohled�van� �et�zec
 * @param   hledan� �et�zec
 * @param   vektor FAIL
 * @return  pozice prvn�ho v�skytu vzoru v textu
 *          pokud vzor nen� nalezen, vr�t� se -1
 */
int kmpHledej (char *text, char *vzor, int vektor[]) {

  int i = 0;                  // index v textu
  int stav = 0;               // stav vzoru
  int maxStav = strlen(vzor); // koncov� stav

  while (text[i] != '\0' && stav < maxStav) {
    // nedo�etli jsme text do konce 
    // a nedo�li do koncov�ho stavu 

    if (stav == -1 || text[i] == vzor[stav]) {
      // p�esun na nov� znak textu 
      // a do n�sleduj�c�ho stavu vzoru
      i++;
      stav++;
    }
    else {
      // z�st�v�me na aktu�ln�m znaku textu
      // n�vrat do p�edchoz�ho stavu dan�ho vektorem
      stav = vektor[stav];
    }

  } // while

  if (stav == maxStav) {
  // vr�t� index, na jak�m byl vzor nalezen
    return (i - maxStav + 1);
  }
  // vzor nebyl nenalezen
  else return -1; 

}

/////////////////////////////////////////////////////////////////////////////////// merge sort:


/*
 * Funkce se�ad� �et�zec (pole znak�) podle ordin�ln�ch hodnot.
 * Pou�it� agoritmus: merge sort
 * @param   �et�zec
 */
void mergeSort(char *str) {
  if(str == NULL)  return;

  // inicializace:

  int copied = 0;            // pocet prvku ktere jiz byly skopirovany do ciloveho pole
  int strLen = strlen(str);  // delka retezce
  char arr[2*strLen];        // pomocne pole o dva krat velikosti retezce bez \0
  copyArray(arr,str,strLen); // skopiruju retezec bez \0
                             // do leve casti pomocneho pole
  int i,j,k,l,step,extra;
  bool endLeft,endRight,endMiddle,sourceLeft;
  sourceLeft=true;           // pocatecni smer postupu - leve pole je zdroj, prave cil

  // t��d�n�:

  while(!isSorted(strLen,arr)) {
  // dokud neni posloupnost serazena:

    copied=0;
    if(sourceLeft) {         // jestli smer postupu je z leve casti pole do prave
      i=0;                   // zacatek pole
      j=strLen-1;            // konec leve casti pole
      k=strLen;              // zacatek prave casti pole
      l=2*strLen-1;          // konec pole
    }
    else {                   // jestli smer postupu je z prave casti pole do leve
      k=0;
      l=strLen-1;
      i=strLen;
      j=2*strLen-1;
    }
    step=1;  // krok - smer postupu cilovym polem - zleva doprava o jedno

    while(i!=j || copied!=strLen) {  
      // dokud se i a j nesetkaji a dokud nebyly
      // skopirovany vsechny prvky zdrojoveho pole

      endLeft=false;         // konec neklesajici posloupnosti zleva
      endRight=false;        // konec neklesajici posloupnosti zprava
      endMiddle=false;       // setkani se i a j

      while(!endLeft && !endRight && !endMiddle) {  
        // dokud neskoncila
        // neklesajici posloupnost zleva, zprava,
        // nebo dokud se i a j nesetkaji

        if(arr[i]<arr[j]) {  // jestli prvek v leve casti zdrojoveho pole
                             // je mensi nez prvek v prave casti
          arr[k]=arr[i];     // vloz prvek do ciloveho pole
          copied++;
          if(i==j) {         // jestli se i a j setkali
            endMiddle=true;
          }
          else {             // jestli se i a j jeste nesetkali
            i++;             // posun i
            k+=step;         // posun k (index ciloveho pole)
            if(arr[i]<arr[i-1]) {
              endLeft=true;  // jestli skoncila neklesajici posloupnost zleva
            }
          }
        }

        else {  
          // jestli prvek v prave casti zdrojoveho pole je mensi nez prvek v leve casti

          arr[k]=arr[j];     // vloz prvek do ciloveho pole
          copied++;
          if(i==j) {         // jestli se i a j setkali
            endMiddle=true;
          }
          else {             // jestli se i a j jeste nesetkali
            j--;             // posun j
            k+=step;         // posun k (index ciloveho pole)
            if(arr[j]<arr[j+1]) {
              // jestli skoncila neklesajici posloupnost zprava
              endRight=true;
            }
          }
        }
      }

      if(endLeft) {          // jestli skoncila neklesajici posloupnost zleva
        copyRight(&k,&j,&copied,arr,step);  // skopiruj pravou neklesajici
        endLeft=false;
      }
      if(endRight) {         // jestli skoncila neklesajici posloupnost zprava
        copyLeft(&k,&i,&copied,arr,step);  // skopiruj levou neklesajici
        endRight=false;
      }
      extra=k;               // prehozeni k a l
      k=l;
      l=extra;
      step=-step;            // obraceni polarity postupu cilovym polem
    }

    sourceLeft=!sourceLeft;  // zmena smeru postupu (prehozeni zdrojoveho a ciloveho pole)

    if(!sourceLeft) {        // jestli byla leva cast pole zdrojova
      copyToSrc(strLen,arr); // skopiruju cilove pole do zdrojoveho
    }

  } // while, dokud pole nen� set��zen�

  copyArray(str,arr,strLen); // skopiruju jiz serazenou posloupnost znaku
                             // z pomocneho pole na adresu puvodniho retezce
}

// skopiruju retezec do pomocneho pole nebo naopak
void copyArray(char *dest, char *src, int strLen) {
  for(int x=0;x<strLen;x++)
    dest[x]=src[x];
}

// skopiruju cilove pole do zdrojoveho
void copyToSrc(int strLen, char *arr) {
  for(int x=0;x<strLen;x++)
    arr[x]=arr[x+strLen];
}


/*
 * Kontrola serazeni seznamu
 * vraci true je-li seznam serazen
 * @return  bool
 */
bool isSorted(int strLen, char *arr) {

  bool sorted=true;
  for(int x=0;x<strLen-1;x++) {

    if(arr[x]>arr[x+1]) sorted=false;
  }
  return sorted;
}

/*
 * skopiruje zbytek neklesajici posloupnosti zprava do ciloveho pole
 */
void copyRight(int *k, int *j, int *copied, char *arr, int step) {
  bool endLeft=true;
  while(endLeft) {
    arr[(*k)]=arr[(*j)];
    (*copied)++;
    (*j)--;
    (*k)+=step;
    endLeft=(arr[(*j)]>=arr[(*j)+1]);  // jestli skoncila neklesajici posloupnost
  }                                    // zprava, endLeft bude false a ukonci cyklus
}

/*
 * skopiruje zbytek neklesajici posloupnosti zleva do ciloveho pole
 */
void copyLeft(int *k, int *i, int *copied, char *arr, int step) {
  bool endRight=true;
  while(endRight) {
    arr[(*k)]=arr[(*i)];
    (*copied)++;
    (*i)++;
    (*k)+=step;
    endRight=(arr[(*i)]>=arr[(*i)-1]);  // jestli skoncila neklesajici posloupnost
  }                                     // zleva, endRight bude false a ukonci cyklus
}

/* konec library.c */
