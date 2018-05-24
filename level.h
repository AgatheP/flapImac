#ifndef _LEVEL_H
#define _LEVEL_H

#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "parameters.h"

//reads the PPM file and creates the corresponding objects at the right coordonates
void loadLevel(ShipList *foes, BlockList *obstacles);


#endif
