#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"

int main(){

  printf("Writing and reading file test :\n\n");
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  int energie=10;

  srand(time(NULL));

  // creating 4 specific preys
  Animal *a1 = creer_animal(0,10, energie); 
  Animal *a2 = creer_animal(15,35, energie);
  Animal *a3 = creer_animal(2,42, energie);
  Animal *a4 = creer_animal(18,13, energie);
  
  a1->suivant=a2;
  a2->suivant=a3;
  a3->suivant=a4;

  liste_proie=a1;

  // creating 4 specific predators
  Animal *a5 = creer_animal(2,5, energie);
  Animal *a6 = creer_animal(15,35, energie);
  Animal *a7 = creer_animal(9,22, energie);
  Animal *a8 = creer_animal(17,3, energie);
  
  a5->suivant=a6;
  a6->suivant=a7;
  a7->suivant=a8;

  liste_predateur=a5;
  
  printf("4 preys and 4 predators :\n");
  ecrire_ecosys("animaux.txt", liste_predateur, liste_proie); //writing all animals in "animaux.txt" (empty file)
  afficher_ecosys(liste_proie, liste_predateur); 
  printf("\n\n");

  Animal *liste_proie2= NULL;
  Animal *liste_predateur2 = NULL;

  printf("Getting the data from the recently written file :\n");
  lire_ecosys("animaux.txt", &liste_predateur2, &liste_proie2); //reading data from "animaux.txt"
  afficher_ecosys(liste_proie2, liste_predateur2); //displaying the data and testing if both lists are the same
  printf("\n\n");

  return 0;
}