#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000

/* Global parameters of the ecosystem (externe in ecosys.h) 
float p_ch_dir = 0.01;
float p_reproduce_proie = 0.4;
float p_reproduce_predateur = 0.5; */

int main(void){

  // creating an animal
  srand(time(NULL));
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;
  Animal *a1 = creer_animal(0,10, 30); 

  // changing the direction
  a1->dir[0]=-1; 
  a1->dir[1]=0; 

  liste_proie = a1;

  // testing if toricity is OK
  printf("Toricity test :\n");
  afficher_ecosys(liste_proie, liste_predateur);
  printf("\n\n");

  for(int i=0;i<3;i++){
    bouger_animaux(liste_proie); 
  }
  afficher_ecosys(liste_proie, liste_predateur);
  printf("\n\n"); //it's OK :)

  // testing if animals can reproduce
  printf("Reproduction test :\n");
  reproduce(&liste_proie, 1.0);
  afficher_ecosys(liste_proie, liste_predateur); //prey number should increase 
  printf("\n\n");

  // creation of 20 preys with random positions and energy level
  srand(time(NULL));
  int x = rand() % ((SIZE_X - 1) - (0) + 1) + (0); //random int between 0 and "SIZE_X - 1" (included)
  int y = rand() % ((SIZE_Y - 1) - (0) + 1) + (0); //random int between 0 and "SIZE_Y - 1" (included)
  float energie = ((float)rand() / (float)(RAND_MAX)) * 10; //random float between 0 and 10 (included)

  Animal* liste_proies = malloc(sizeof(Animal));
  liste_proies = creer_animal(x, y, energie);
    
  for (int i = 0; i < 19; i++){
    x = rand() % (SIZE_X);
    y = rand() % (SIZE_Y);
    energie = ((float)rand() / (float)(RAND_MAX)) * 10;

    liste_proies = ajouter_en_tete_animal(liste_proies, creer_animal(x, y, energie));        
  }

  // creation of 20 predators with random positions
  x = rand() % (SIZE_X); //random int between 0 and "SIZE_X - 1" (included)
  y = rand() % (SIZE_Y); //random int between 0 and "SIZE_Y - 1" (included)
  energie = ((float)rand() / (float)(RAND_MAX)) * 10; //random float between 0 and 10 (included)

  Animal* liste_predateurs = malloc(sizeof(Animal));
  liste_predateurs = creer_animal(x, y, energie);
    
  for (int i = 0; i < 19; i++){
    x = rand() % (SIZE_X);
    y = rand() % (SIZE_Y);
    energie = ((float)rand() / (float)(RAND_MAX)) * 10;

    liste_predateurs = ajouter_en_tete_animal(liste_predateurs, creer_animal(x, y, energie));        
  }

  printf("Adding 20 random predators and 20 random preys :\n");
  afficher_ecosys(liste_proies,liste_predateurs);
  printf("\n\n");

  // creation of specific animals to test "enlever_animal"
  Animal* pro1 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
  Animal* pro2 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
  Animal* pro3 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
  liste_proies = ajouter_en_tete_animal(liste_proies, pro1); //adding of "pro1" to the head of the list
  liste_proies = ajouter_en_tete_animal(liste_proies, pro2); //adding of "pro2" to the head of the list
  liste_proies = ajouter_en_tete_animal(liste_proies, pro3); //adding of "pro2" to the head of the list

  Animal* pred1 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
  Animal* pred2 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
  Animal* pred3 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
  liste_predateurs = ajouter_en_tete_animal(liste_predateurs, pred1); //adding of "pred1" to the head of the list
  liste_predateurs = ajouter_en_tete_animal(liste_predateurs, pred2); //adding of "pred2" to the head of the list
  liste_predateurs = ajouter_en_tete_animal(liste_predateurs, pred3); //adding of "pred2" to the head of the list

  printf("Adding 3 specific predators and 3 specific preys :\n");
  afficher_ecosys(liste_proies,liste_predateurs); //testing the differences
  printf("\n\n");

  // releasing specified animals
  printf("Releasing recently added animals :\n");
  enlever_animal(&liste_proies, pro1); //releasing the third element of a list
  enlever_animal(&liste_predateurs, pred3); //releasing the head of a list
  afficher_ecosys(liste_proies,liste_predateurs); //testing the differences
  printf("\n\n");

  printf("Releasing a non-existent animal for testing :\n");
  enlever_animal(&liste_proies, pro1); //releasing an animal that does not exist
  afficher_ecosys(liste_proies,liste_predateurs); //testing the differences
  printf("\n\n");

  // refreshing preys until there are no preys left or 200 attempts
  printf("Refreshing preys until there are no preys left :\n");
  int i=0; 
  while((i<200) && (compte_animal_it(liste_proies) != 0)){
    rafraichir_proies(&liste_proies);
    i++;
  }
  afficher_ecosys(liste_proies,liste_predateurs); //testing the differences
  printf("\n\n");

  // refreshing predators until there are no predators left (limiting to 200 attempts)
  printf("Refreshing predators until there are no predators left :\n");
  
  i=0; 
  while((i<200) && (compte_animal_it(liste_predateurs) != 0)){
    rafraichir_predateurs(&liste_predateurs, &liste_proies);
    i++;
  }
  afficher_ecosys(liste_proies,liste_predateurs); //testing the differences
  printf("\n\n");

  // releasing the allocated memory
  printf("Releasing the allocated memory :\n");
  liste_proies = liberer_liste_animaux(liste_proies);
  liste_predateurs = liberer_liste_animaux(liste_predateurs);
  afficher_ecosys(liste_proies,liste_predateurs); //testing the differences
  printf("\n\n");

  return 0;
}

