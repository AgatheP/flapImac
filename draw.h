#ifndef _DRAW_H
#define _DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "structures.h"
#include "parameters.h"

GLuint loadTexture(const char* path);

void drawShip(Ship* joueur, GLuint texture);

void drawLazer(Lazer* lazer, GLuint texture);

void drawBlock(Block* block, GLuint texture);

#endif
