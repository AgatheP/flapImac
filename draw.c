#include <stdio.h>
#include <stdlib.h>

#include "draw.h"

void drawShip(Ship* joueur){
    //pour l'instant on ne dessine que la bounding box
    glPointSize(8);
    glPushMatrix();
        //se placer au centre du joueur
        glScalef(0.015,0.025,0);
        glTranslatef(joueur->x,joueur->y,0);
        //dessiner le joueur
        //centre
        glBegin(GL_POINTS);
            glColor3ub(255,255,255);
            glVertex2i(0,0);
        glEnd();
        //bounding box
        glBegin(GL_LINE_LOOP);
            glColor3ub(0,255,0);
            glVertex2f(joueur->Bx,joueur->By);
            glVertex2f(joueur->Bx,-joueur->By);
            glVertex2f(-joueur->Bx,-joueur->By);
            glVertex2f(-joueur->Bx,joueur->By);
        glEnd();
    glPopMatrix();
    return;
}

void drawLazer(Lazer* lazer){
    glPointSize(7);
    glPushMatrix();
        glScalef(0.015,0.025,0);
        glTranslatef(lazer->x,lazer->y,0);
        glBegin(GL_POINTS);
            glColor3ub(lazer->r,lazer->g,lazer->b);
            glVertex2f(0,0);
        glEnd();
        //bounding box
        glBegin(GL_LINE_LOOP);
            glColor3ub(255,255,255);
            glVertex2f(BBLazer,BBLazer);
            glVertex2f(BBLazer,-BBLazer);
            glVertex2f(-BBLazer,-BBLazer);
            glVertex2f(-BBLazer,BBLazer);
        glEnd();
    glPopMatrix();
    return;
}

void drawBlock(Block* block){
    glPointSize(10);
    glPushMatrix();
        glScalef(0.015,0.025,0);
        glTranslatef(block->x, block->y, 0);
        glBegin(GL_POINTS);
            glColor3ub(255,120,0);
            glVertex2f(0,0);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glColor3ub(255,120,0);
            glVertex2f(+5, +5);
            glVertex2f(+5, -5);
            glVertex2f(-5, -5);
            glVertex2f(-5, +5);
        glEnd();
    glPopMatrix();
    return;
}
