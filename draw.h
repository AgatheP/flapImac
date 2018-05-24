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

//loads a texture image
GLuint loadTexture(const char* path);

//draws a textured spaceship
void drawShip(Ship* joueur, GLuint texture);

//draws a textured lazer
void drawLazer(Lazer* lazer, GLuint texture);

//draws a textured obstacle block
void drawBlock(Block* block, GLuint texture);

//chooses what texture to apply to the buff
void selectDrawBuff(Buff* buff);

//draws a textured buff
void drawBuff(Buff* buff);

//draws the background
void drawBackground(GLuint texture);

//draws the game over screen or the victory screen
void drawEndScreen(GLuint texture);

//draws a heart
void drawHeart(GLuint texture, float x, float y);

#endif
