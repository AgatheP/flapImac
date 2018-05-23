#ifndef _DRAW_H
#define _DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "structures.h"
#include "parameters.h"

void drawShip(Ship* joueur);

void drawLazer(Lazer* lazer);

#endif
