/*
 * Soubor:  kmp.c
 * Autor:   Vendula Poncová
 * Projekt: IFJ - KMP algoritmus
 * Popis:   Vyhledávání podøetìzce v øetìzci.

            Spou¹tìt s testy pøíkazem: ./kmp <"kmp-test.txt",
            nebo zadávat lze zadávat pøes terminál ve tvaru: vzor text oèekávanýIndex.

            Tisk pomocných výpisù lze povolit zmìnou makra TISK.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define TISK 0

#if TISK
  #define tisk(prikazy) prikazy
#else
  #define tisk(prikazy)
#endif

////////////////////////////////////////////////////////////////// fce pro tisk pomocných výpisù
void novyRadek() {
  printf("\n");
}

void tiskniMezery(int m) {
  while (m > 0) {
    printf(" ");
    m--;
  }
}

void tiskniCisla(int min, int max, int mezery){

  if (max > 9 ){
    tiskniMezery(mezery);
    for (int i = min; i <= max; i++) {
      if (i%10 == 0) printf("%d",i/10);
      else printf(" ");
    }
    novyRadek();
  }

  tiskniMezery(mezery);
  for (int i = min; i <= max; i++) {
    printf("%d",i%10);
  }

  novyRadek();
}

////////////////////////////////////////////////////////////////// fce KMP

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

  // tiskni pomocné výpisy
  tisk(
     novyRadek();
     //tiskniCisla(0, strlen(vzor)-1, 1);
     printf(" %s\n",vzor);
     for (unsigned int i = 0; i < strlen(vzor); i++) printf("%d",vektor[i]);
     novyRadek();
     novyRadek();
  ) // tisk

  // vra» index vzoru v textu
  return kmpHledej(text, vzor, vektor);

}

/////////////////////////////////////////////////////
/**
 * Hlavní program
 */
int main (void)
{
 char vzor[MAX_SIZE];
 char text[MAX_SIZE];
 int indexVZOR = -1;  // index z testu
 int indexKMP = -1;   // zji¹tìný index
 int test = 0;
 int err = 0;

 while (scanf("%50s %50s %d",vzor, text, &indexVZOR) != EOF) {
   test++;

   // hledej jehlu v kupce sena
   indexKMP = kmp(text, vzor);

   // tiskni pomocné výpisy
   tisk (
     tiskniCisla(1, strlen(text), 1);
     printf(" %s\n",text);
     tiskniMezery(indexKMP-1);
     printf("^\n");
     printf("TEST %d: index = %d\n", test, indexKMP);
   ) //tisk

   // porovnej index
   if (indexKMP != indexVZOR) {
     printf("TEST %d: Indexy se li¹í! %d != %d\n", test, indexKMP, indexVZOR);
     err++;
   }
  }

  printf("Celkem %d testù, %d chybných.\n",test, err);

  return EXIT_SUCCESS;
}

/* konec souboru kmp.c */
