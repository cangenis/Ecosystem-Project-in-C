#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

Animal* creer_animal(int x, int y, float energie){
    // create and return animal with "x" as poisiton-x, "y" as position-y and "energie" as energie

    Animal* res = (Animal*) malloc(sizeof(Animal));
    assert(res != NULL);

    res->x = x;
    res->y = y;
    res->energie = energie;

    res->dir[0] = rand() % (1 - (-1) + 1) + (-1);
    res->dir[1] = rand() % (1 - (-1) + 1) + (-1);

    res->suivant = NULL;

    return res;
}


Animal* ajouter_en_tete_animal(Animal* liste, Animal* animal){
    // add the animal "animal" in the head of "liste" and return the head

    assert(animal != NULL);

    animal->suivant = liste;

    return animal;
}


void ajouter_animal(int x, int y, float energie, Animal** liste_animal){
    // create an animal in the desired position ("x", "y") and add it to the list "liste_animal"

    assert((x < SIZE_X) && (y < SIZE_Y));

    //float energie = ((float)rand() * (float)(RAND_MAX)) * 10; //random float between 0 and 10 (included) as the energy level of animal
    
    *liste_animal = ajouter_en_tete_animal(*liste_animal, creer_animal(x, y, energie));
}


void enlever_animal(Animal** liste, Animal* animal){
    // release "animal" from "liste"

    Animal* temp = (Animal*)malloc(sizeof(Animal));

    if (*liste == animal){ //the wanted animal is the head of "liste"
       temp = (*liste)->suivant;
       free(*liste);
       *liste = temp;
    }

    else{
        Animal* origin = (Animal*)malloc(sizeof(Animal));
        origin = *liste; //storing the head of the "liste"

        while (*liste){
            if (*liste == animal){
                temp->suivant = (*liste)->suivant;
                free(*liste);
                *liste = temp->suivant;
            }

            else{
                temp = *liste;
                *liste = (*liste)->suivant;
            }
        }

        *liste = origin;
    }
}


Animal* liberer_liste_animaux(Animal* liste){
    // release the memory that is allocated to "liste"

    Animal* temp = (Animal*)malloc(sizeof(Animal));

    while(liste){
        temp = liste->suivant;
        free(liste);
        liste = temp;
    }

    return liste;
}

unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}


void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
    FILE *f=fopen(nom_fichier, "w"); //opening file in writer mode
   
    fprintf(f,"<proies>\n"); //writing the first line to the file

    Animal *tmp=liste_proie; 

    while (tmp) { //writing each prey to the file
        fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",tmp->x, tmp->y, tmp->dir[0], tmp->dir[1], tmp->energie);
        tmp=tmp->suivant;
    }

    fprintf(f,"</proies>\n"); 
    fprintf(f,"<predateurs>\n");
    tmp=liste_predateur;

    while (tmp) { //writing each predator to the file
        fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",tmp->x, tmp->y, tmp->dir[0], tmp->dir[1], tmp->energie);
        tmp=tmp->suivant;
    }

    fprintf(f,"</predateurs>\n");
    fclose(f);
}


void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {

    FILE *f=fopen(nom_fichier, "r");

    if (f==NULL) {
        printf("Erreur lors de l’ouverture de %s\n",nom_fichier);
        //fprintf(stderr, "Erreur lors de l’ouverture de %s\n",nom_fichier)
        return;
    }

    char buffer[256];
    int x_lu, y_lu, dir_lu[2];
    float e_lu;

    fgets(buffer, 256, f); //reading of <proies>
    assert(strncmp(buffer, "<proies>",8)==0);
    fgets(buffer, 256, f); //reading the line containing the first prey
    
    while (strncmp(buffer,"</proies>",9)!=0) {
        sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n", &x_lu, &y_lu, &dir_lu[0], &dir_lu[1], &e_lu);
        Animal *a_lu=creer_animal(x_lu, y_lu, e_lu);
        a_lu->dir[0]=dir_lu[0];
        a_lu->dir[1]=dir_lu[1];
        //inserting at the top of the list
        a_lu->suivant=*liste_proie;
        *liste_proie = a_lu;
        fgets(buffer, 256, f); //reading the line containing the next prey
    }

    fgets(buffer, 256, f); //reading of <predateurs>
    assert(strncmp(buffer, "<predateurs>",12)==0);

    fgets(buffer, 256, f); //reading the line containing the first predator

    while (strncmp(buffer,"</predateurs>",13)!=0) {
        sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n", &x_lu, &y_lu, &dir_lu[0], &dir_lu[1], &e_lu);
        Animal *a_lu=creer_animal(x_lu, y_lu, e_lu);
        a_lu->dir[0]=dir_lu[0];
        a_lu->dir[1]=dir_lu[1];
        //inserting at the top of the list
        a_lu->suivant=*liste_predateur;
        *liste_predateur = a_lu;
        fgets(buffer, 256, f); //reading the line containing the next predator
    }

    fclose(f);
}


void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  // initializing the array 
  for (i = 0; i < SIZE_X; i++) {
    for (j = 0; j < SIZE_Y; j++) {
      ecosys[i][j]=' ';
    }
  }

  // adding the prey 
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  // adding the predator 
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { 
        ecosys[pa->x][pa->y] = '@';
      }
      else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  // printing the array 
  printf("+");

  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  

  printf("+\n");

  for (i = 0; i < SIZE_X; ++i) {
    printf("|");

    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }

    printf("|\n");
  }

  printf("+");

  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }

  printf("+\n");

  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Prey count : %5d\tPredator count : %5d\n", nbproie, nbpred);
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 for deleting the screen */
}


void bouger_animaux(Animal *la) {
  Animal *ap = la;
  while (ap) {
      if (rand() / (float)RAND_MAX < 0.01) {  //changing direction with p_ch_dir probability
          ap->dir[0] = rand() % 3 - 1;
          ap->dir[1] = rand() % 3 - 1;
      }
      // moving the animal according to its direction and verifying limits
      ap->x = (ap->x + ap->dir[0] + SIZE_X) % SIZE_X; 
      ap->y = (ap->y + ap->dir[1] + SIZE_Y) % SIZE_Y;
      ap = ap->suivant;
  }

}


void reproduce(Animal **liste_animal, float p_reproduce) {
  Animal *ap = liste_animal!= NULL ? *liste_animal : NULL;
  while (ap) {
      if (rand() / (float)RAND_MAX < p_reproduce) {  //reproduction of an animal with probability p_reproduce
          ajouter_animal(ap->x, ap->y, ap->energie/2, liste_animal); //creating a new animal with half the energy of its parent
          ap->energie=ap->energie/2; //dividing parent's energy by 2
      }
      ap = ap->suivant;
  }

}


void rafraichir_proies(Animal **liste_proie) {
  assert(liste_proie);

  bouger_animaux(*liste_proie); //moving preys

  Animal* iter = *liste_proie;
  assert(iter);
  
  while(iter){
    (iter->energie)--; //decreasing the energy level by 1

    if (iter->energie <= 0){ 
      enlever_animal(liste_proie, iter); //deleting the prey if its energy is less than 0
    }

    iter = iter -> suivant;
  }
  
  reproduce(liste_proie, 0.4); //p_rep_proie = 0.4
}


Animal *animal_en_XY(Animal *l, int x, int y){
  //assert(l);

  Animal *iter = l;
  //assert(iter);

  while(iter){
    if (iter->x == x && iter->y == y){ //finding the prey
      return iter; 
    }
    iter = iter->suivant;
  }

  return NULL; 
}


void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie){ 
  assert(liste_predateur);
  assert(liste_proie);

  bouger_animaux(*liste_predateur);
  Animal *proie_temp = (Animal*)malloc(sizeof(Animal));
  Animal* iter = *liste_predateur;
  assert(iter);

  while(iter){
    proie_temp = animal_en_XY(*liste_proie, iter->x, iter->y);
    (iter -> energie)--; //decreasing the energy level by 1
    if (proie_temp != NULL){ //checking if there is a prey in the same case
      iter->energie += proie_temp->energie; //increasing the energy level of the predator by the energy level of the prey
      enlever_animal(liste_proie, proie_temp); //deleting the prey
    }
     
    if (iter -> energie <= 0){ 
      enlever_animal(liste_predateur, iter); //deleting the predator if its enery is equal to 0
    }
    iter = iter -> suivant;
  }
  reproduce(liste_predateur, 0.5); //p_rep_predateur = 0.5 
}