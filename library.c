/*
 * @description   Knihovna vestavìných funkcí
 * @author        Vendula Poncová - xponco00
 *                Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#include "library.h"

/*
 * Inicializuje se zásobník. Ukazatel na NULL vyvolá chybu.
 * @param   ukazatel na zásobník
 * @return  kód chyby
 */

const char *dataTypeStrings[]= {
 [NIL] ="nil", 
 [BOOL]="bool", 
 [STRING]="string",
 [NUMBER]="number",  
};

/////////////////////////////////////////////////////////////////////////////////// vestavené funkce:

/*
 * Funkce vrací øetìzec obsahující datový typ dat.
 * @param   ukazatel na data, jejich¾ typ chceme znát
 * @return  data STRING obsahující øetìzec s datovým typem
 */
TVarData type(TVarData *param) {

  TVarData data;
  data.value.s = strCreateString((char*) dataTypeStrings[param->type]);
  data.type = STRING;
  return data;
}

/*
 * Funkce vrací podøetìzec zadaného øetìzce.
 * Pokud jsou parametry nesprávného typu, vrací NIL.
 * Pokud se nepodaøí alokace øetìzce, vrací chybový kód do err.
 * @param   ukazatel na data STRING
 * @param   ukazatel na data NUMBER
 * @param   ukazatel na data NUMBER
 * @param   ukazatel chybu
 * @return  data STRING
 */
TVarData substr(TVarData *dataS, TVarData *dataFrom, TVarData *dataTo, int *err) {

  TVarData data;
  data.type = NIL;

  // kotrola parametrù:
  if (dataS->type == STRING && dataFrom->type == NUMBER && dataTo->type == NUMBER) {

    int from = dataFrom->value.n;
    int to   = dataTo->value.n;
    char *s  = dataS->value.s.str;
    //int srcLen = dataS->value.s.length;

    // pøepoèítání indexù - DOKONÈIT
    int destLen = to - from + 1;

    // alokace nového øetìzce
    if(strInitLen(&data.value.s, destLen) == EOK) {
      data.type = STRING;

      // kopírování
      strncpy(data.value.s.str, &s[from-1], destLen);
      data.value.s.str[destLen] = '\0';
    }
    else *err = ERR;

  }
  return data;
}

/*
 * Funkce vyhledá první výskyt podøetìzce sPattern v øetìzci s
 * @param   ukazatel na data STRING, v tomto øetìzci vyhledáváme
 * @param   ukazatel na data STRING, tento øetìzec hledáme
 * @return  data typu NIL, BOOL nebo NUMBER
 */
TVarData find(TVarData *sData, TVarData *sPatternData) {

  TVarData data;
  data.type = NIL;  

  // kontrola parametrù
  if (sData->type == STRING && sPatternData->type == STRING) {

    // vyhledávání podøetìzce
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
 * Seøadí znaky v daném øetìzci podle ord. hodnot.
 * @param   ukazatel na data STRING
 * @param   ukazatel na chybu
 * @return  data STRING se setøízeným øetìzcem
 */
TVarData sort(TVarData *sData, int *err) {

  TVarData data;
  data.type = NIL;

  // kontrola parametrù:
  if (sData->type == STRING) {

    data.type = STRING;
    strInitLen(&data.value.s, 0);
    *err = strCopyString(&data.value.s, &sData->value.s);
    // setøízení:
    if (*err == EOK) mergeSort(data.value.s.str);
  }

  return data;
}

/////////////////////////////////////////////////////////////////////////////////// funkce KMP:

/*
 * Funkce pro nalezení podøetìzce v øetìzci.
 * Nejspí¹ se z ní stane funkce sort(string,string).
 * @param   prohledávaný øetìzec
 * @param   hledaný øetìzec
 * @return  pozice prvního výskytu vzoru v textu
 *          pokud vzor není nalezen, vrátí se -1
 */
int kmp (char *text, char *vzor) {

  // vzor je prázdný øetìzec, vra» nulu
  if (strlen(vzor) == 0) return 0;
  
  int dVzoru = strlen(vzor);
  int vektor[dVzoru];

  // nalezení fail vektoru pro vzor
  kmpVektor(vzor, vektor);

  // vra» index vzoru v textu
  return kmpHledej(text, vzor, vektor);
}

/*
 * Pro daný vzor najde pøechody mezi stavy KMP automatu 
 * a zapí¹e je do vektoru FAIL.
 * @param   vzorový øetìzec
 * @param   pole integerù - vektor
 */
void kmpVektor (char *vzor, int vektor[]) {

  int delka = strlen(vzor);  // délka øetìzce vzor
  int stav;                  // aktuální pozice ve vzoru (index)
  int prechod;               // návratový index pro daný stav (hodnota v poli vektor)

  vektor[0] = -1;            // z nultého stavu návrat do poèáteèního stavu "-1" 
                             // stav "-1" existuje pouze teoreticky

  for (stav = 1; stav < delka; stav++) { // pro ka¾dou pozici ve vzoru:

    prechod = vektor[stav-1];            // ulo¾ pøechod pøedchozího stavu

    while ( (prechod > -1) && (vzor[prechod] != vzor[stav-1]) ) {

      /* Pøechod nevede do poèáteèního stavu a znak ve stavu, do kterého
         se vracíme, je rùzný od znaku na pøedchozí pozici.
         Provedeme návrat pomocí pøechodu na ni¾¹í pozici
         a ulo¾íme si nový pøechod.
      */
      prechod = vektor[prechod];
    }
    
    /* Pøechod vede do poèáteèního stavu (není tedy kam se dál posunovat)
       nebo porovnávané znaky se rovnaly.
       Pøièítáme 1, nebo» nalezený "suffix", na který se lze pøesunout
       roz¹iøujeme o nový znak.
    */
    vektor[stav] = prechod + 1;
  }
}

/*
 * Vyhledává podøetìzec vzor v øetìzci text pomocí
 * KMP algoritmu. Pozice se èísluje od 1.
 * @param   prohledávaný øetìzec
 * @param   hledaný øetìzec
 * @param   vektor FAIL
 * @return  pozice prvního výskytu vzoru v textu
 *          pokud vzor není nalezen, vrátí se -1
 */
int kmpHledej (char *text, char *vzor, int vektor[]) {

  int i = 0;                  // index v textu
  int stav = 0;               // stav vzoru
  int maxStav = strlen(vzor); // koncový stav

  while (text[i] != '\0' && stav < maxStav) {
    // nedoèetli jsme text do konce 
    // a nedo¹li do koncového stavu 

    if (stav == -1 || text[i] == vzor[stav]) {
      // pøesun na nový znak textu 
      // a do následujícího stavu vzoru
      i++;
      stav++;
    }
    else {
      // zùstáváme na aktuálním znaku textu
      // návrat do pøedchozího stavu daného vektorem
      stav = vektor[stav];
    }

  } // while

  if (stav == maxStav) {
  // vrátí index, na jakém byl vzor nalezen
    return (i - maxStav + 1);
  }
  // vzor nebyl nenalezen
  else return -1; 

}

/////////////////////////////////////////////////////////////////////////////////// merge sort:


/*
 * Funkce seøadí øetìzec (pole znakù) podle ordinálních hodnot.
 * Pou¾itý agoritmus: merge sort
 * @param   øetìzec
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

  // tøídìní:

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

  } // while, dokud pole není setøízené

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
