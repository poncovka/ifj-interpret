/*
 * Soubor:  kmp.c
 * Autor:   Vendula Poncov�
 * Projekt: IFJ - KMP algoritmus
 * Popis:   Vyhled�v�n� pod�et�zce v �et�zci.

            Spou�t�t s testy p��kazem: ./kmp <"kmp-test.txt",
            nebo zad�vat lze zad�vat p�es termin�l ve tvaru: vzor text o�ek�van�Index.

            Tisk pomocn�ch v�pis� lze povolit zm�nou makra TISK.
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

////////////////////////////////////////////////////////////////// fce pro tisk pomocn�ch v�pis�
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

  // tiskni pomocn� v�pisy
  tisk(
     novyRadek();
     //tiskniCisla(0, strlen(vzor)-1, 1);
     printf(" %s\n",vzor);
     for (unsigned int i = 0; i < strlen(vzor); i++) printf("%d",vektor[i]);
     novyRadek();
     novyRadek();
  ) // tisk

  // vra� index vzoru v textu
  return kmpHledej(text, vzor, vektor);

}

/////////////////////////////////////////////////////
/**
 * Hlavn� program
 */
int main (void)
{
 char vzor[MAX_SIZE];
 char text[MAX_SIZE];
 int indexVZOR = -1;  // index z testu
 int indexKMP = -1;   // zji�t�n� index
 int test = 0;
 int err = 0;

 while (scanf("%50s %50s %d",vzor, text, &indexVZOR) != EOF) {
   test++;

   // hledej jehlu v kupce sena
   indexKMP = kmp(text, vzor);

   // tiskni pomocn� v�pisy
   tisk (
     tiskniCisla(1, strlen(text), 1);
     printf(" %s\n",text);
     tiskniMezery(indexKMP-1);
     printf("^\n");
     printf("TEST %d: index = %d\n", test, indexKMP);
   ) //tisk

   // porovnej index
   if (indexKMP != indexVZOR) {
     printf("TEST %d: Indexy se li��! %d != %d\n", test, indexKMP, indexVZOR);
     err++;
   }
  }

  printf("Celkem %d test�, %d chybn�ch.\n",test, err);

  return EXIT_SUCCESS;
}

/* konec souboru kmp.c */
