#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

typedef struct ship {
    float x,y;
    int hpMax;
    int hp;
    //coordonnée d'un point de la bounding box relativement au centre du joueur (le 2e s'obtien avec une multiplication par -1)
    float Bx, By;
    //texture à rajouter par le suite
    int fireRate;
    struct ship* next;
} Ship, *ShipList;

typedef struct Lazer {
    float x, y;
    float speed; //>0 : ->, <0 : <-
    unsigned char r, g, b; //couleur
    struct Lazer* next;
} Lazer, *LazerList;

Ship * allocShip(float x, float y, int hpMax, int width, int height, int fireRate);

void addShipToList(Ship * ship, ShipList * list);

void freeShipList(ShipList * list);

int removeShipFromList(ShipList * ship, ShipList * list);

Lazer * allocLazer(float x, float y, float speed, unsigned char r, unsigned char g, unsigned char b);

void addLazerToList(Lazer * lazer, LazerList * list);

int removeLazerFromList(LazerList * lazer, LazerList * list);

void freeLazers(LazerList * list);

#endif