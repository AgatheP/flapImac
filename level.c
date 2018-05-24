#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "level.h"
#include "parameters.h"


void loadLevel(ShipList *foes, BlockList *obstacles) {
  int tab[10][100][3];
  char line[3];

	FILE* file = fopen(FILE_NAME,"r");
	if(file == NULL) {
		printf("ERROR : the file couldn't be loaded.\n");
		exit(EXIT_FAILURE);
	}

  //reads PPM file and fills array with its values
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
  //creates et places blocks, ennemies and buffs
  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 100; j++) {
      int posX = j*playableHeight/10;
      int posY = -i*playableHeight/10 + playableHeight/2.2;
      if(tab[i][j][0] == 255 && tab[i][j][1] == 0 && tab[i][j][2] == 0) {
        //red, add block
        addBlockToList(allocBlock(posX, posY), obstacles);
      }
      if(tab[i][j][0] == 0 && tab[i][j][1] == 255 && tab[i][j][2] == 0) {
        //green, add enemy
        addShipToList(allocShip(posX, posY,1,7,7,80), foes);
      }
      if(tab[i][j][0] == 0 && tab[i][j][1] == 0 && tab[i][j][2] == 255) {
        //blue. We know how to load and interact with different types of buffs
        //(end of level, speed boost...) but a bug prevented us from using them.
      }
    }
  }
}
