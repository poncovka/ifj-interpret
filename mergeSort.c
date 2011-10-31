/*
 * @description   Razeni retezce metodou merge-sort
 * @author        Patrik Hronsky - xhrons00
 * @projekt       IFJ11
 * @date
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

// kontrola serazeni seznamu
// vraci true je-li seznam serazen
bool isSorted(int strLen, char *arr) {
  bool sorted=true;
  for(int x=0;x<strLen-1;x++)
    if(arr[x]>arr[x+1])
      sorted=false;
  return sorted;
}

//skopiruje zbytek neklesajici posloupnosti zprava do ciloveho pole
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

//skopiruje zbytek neklesajici posloupnosti zleva do ciloveho pole
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

void mergeSort(char *str) {
  if(str==NULL)
    return;
  int copied=0;  // pocet prvku ktere jiz byly skopirovany do ciloveho pole
  int strLen=strlen(str);  // delka retezce
  char arr[2*strLen];  // pomocne pole o dva krat velikosti retezce bez \0
  copyArray(arr,str,strLen);  // skopiruju retezec bez \0
                              // do leve casti pomocneho pole
  int i,j,k,l,step,extra;
  bool endLeft,endRight,endMiddle,sourceLeft,sorted;
  sourceLeft=true;  // pocatecni smer postupu - leve pole je zdroj, prave cil
  sorted=isSorted(strLen,arr);

  while(!sorted) {  // dokud neni posloupnost serazena
    copied=0;
    if(sourceLeft) {  // jestli smer postupu je z leve casti pole do prave
      i=0;           // zacatek pole
      j=strLen-1;    // konec leve casti pole
      k=strLen;      // zacatek prave casti pole
      l=2*strLen-1;  // konec pole
    }
    else {  // jestli smer postupu je z prave casti pole do leve
      k=0;
      l=strLen-1;
      i=strLen;
      j=2*strLen-1;
    }
    step=1;  // krok - smer postupu cilovym polem - zleva doprava o jedno

    while(i!=j || copied!=strLen) {  // dokud se i a j nesetkaji a dokud nebyly
                                     // skopirovany vsechny prvky zdrojoveho pole
      endLeft=false;  // konec neklesajici posloupnosti zleva
      endRight=false;  // konec neklesajici posloupnosti zprava
      endMiddle=false;  // setkani se i a j

      while(!endLeft&&!endRight&&!endMiddle) {  // dokud neskoncila
                        // neklesajici posloupnost zleva, zprava,
                        // nebo dokud se i a j nesetkaji
        if(arr[i]<arr[j]) {  // jestli prvek v leve casti zdrojoveho pole
                             //je mensi nez prvek v prave casti
          arr[k]=arr[i];  // vloz prvek do ciloveho pole
          copied++;
          if(i==j)  // jestli se i a j setkali
            endMiddle=true;
          else {  // jestli se i a j jeste nesetkali
            i++;  // posun i
            k+=step;  // posun k (index ciloveho pole)
            if(arr[i]<arr[i-1])  // jestli skoncila neklesajici posloupnost zleva
              endLeft=true;
          }
        }
        else {  // jestli prvek v prave casti zdrojoveho pole je mensi nez prvek v leve casti
          arr[k]=arr[j];  // vloz prvek do ciloveho pole
          copied++;
          if(i==j)  // jestli se i a j setkali
            endMiddle=true;
          else {  // jestli se i a j jeste nesetkali
            j--;  // posun j
            k+=step;  // posun k (index ciloveho pole)
            if(arr[j]<arr[j+1])  // jestli skoncila neklesajici posloupnost zprava
              endRight=true;
          }
        }
      }
      if(endLeft) {  // jestli skoncila neklesajici posloupnost zleva
        copyRight(&k,&j,&copied,arr,step);  // skopiruj pravou neklesajici
        endLeft=false;
      }
      if(endRight) {  // jestli skoncila neklesajici posloupnost zprava
        copyLeft(&k,&i,&copied,arr,step);  // skopiruj levou neklesajici
        endRight=false;
      }
      extra=k;  // prehozeni k a l
      k=l;
      l=extra;
      step=-step;  // obraceni polarity postupu cilovym polem
    }
    sourceLeft=!sourceLeft;  // zmena smeru postupu (prehozeni zdrojoveho a ciloveho pole)
    if(!sourceLeft)  // jestli byla leva cast pole zdrojova
      copyToSrc(strLen,arr);  // skopiruju cilove pole do zdrojoveho
    sorted=isSorted(strLen,arr);  // je retezec serazeny?
  }
  copyArray(str,arr,strLen);  // skopiruju jiz serazenou posloupnost znaku
                              // z pomocneho pole na adresu puvodniho retezce
}
