#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "level.h"
#include "parameters.h"


void loadLevel(ShipList *foes, BlockList *obstacles) {
  int tab[10][100][3];
  char line[3];

  /*vérifie et ouvre le fichier FILE_NAME*/
	FILE* file = fopen(FILE_NAME,"r");
	if(file == NULL) {
		printf("ERREUR: le fichier n'a pas pu être ouvert, vérifiez le chemin ou les droits d'accès.\n");
		exit(EXIT_FAILURE);
	}
  //lis le fichier et rempli tableau avec valeurs rgb pixels
  for(int i=0; i < 3; i++) {
    fgets(line, 10, file);
  }
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 100; j++) {
      for(int k = 0; k < 3; k++) {
        fscanf(file, "%3d", &tab[i][j][k]);
      }
    }
  }
  //crée et place block, ennemis et bonus aux coordonnées correspondantes
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 100; j++) {
      int posX = j*playableHeight/10;
      int posY = -i*playableHeight/10 + playableHeight/2.2;
      if(tab[i][j][0] == 255 && tab[i][j][1] == 0 && tab[i][j][2] == 0) {
        //rouge, ajouter un block
        addBlockToList(allocBlock(posX, posY), obstacles);
        printf("Block chargé\n");
      }
      if(tab[i][j][0] == 0 && tab[i][j][1] == 255 && tab[i][j][2] == 0) {
        //vert, ajouter un ennemi
        addShipToList(allocShip(posX, posY,1,5,5,80), foes);
        printf("Ennemi chargé\n");
      }
      if(tab[i][j][0] == 0 && tab[i][j][1] == 0 && tab[i][j][2] == 255) {
        //bleu, ajouter un bonus
        printf("Bonus chargé\n");
      }
    }
  }
}
