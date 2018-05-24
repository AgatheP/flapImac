#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 1800;
static unsigned int WINDOW_HEIGHT = 950;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
//Bounding box fixes
static float BBLazer = 2;
static int BBBlock= 4;
static float BBBuff=3.5;

static float scrollSpeed = 0.40; //vitesse de déplacement horizontal du jeu
static int playableHeight = 80;
static int PPMHeight = 10;
static int PPMWidth = 100;
static char * FILE_NAME = "Levels/niveau.ppm";
static char* spaceshipTex = "Sprites/Spaceship.png";
static char* foeTex = "Sprites/Foe.png";
static char* bulletTex = "Sprites/Bullet.png";
static char* bulletFoeTex = "Sprites/BulletFoe.png";
static char* blockTex = "Sprites/Block.png";
static char* background = "Sprites/back2.jpg";
static char* heart = "Sprites/heart.png";

#endif
