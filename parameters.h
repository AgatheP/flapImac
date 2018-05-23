#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#include <stdlib.h>
#include <stdio.h>

/* Dimensions de la fenêtre */
static const unsigned int WINDOW_WIDTH = 1100;
static const unsigned int WINDOW_HEIGHT = 500;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60; 
static const float BBLazer = 2; //La bounding box des lazers est carré et on ne donne que la moitiée du côté

/* Rapport de taille avec les pixels du PPM */
static const unsigned int heightPPM=10; /*Nombe de pixel par collone de l'image*/
/*static const float blockSide=(int)WINDOW_HEIGHT/(float)heightPPM; */ //ne fonctionne pas
static const float blockSide=500/10.0;

#endif
