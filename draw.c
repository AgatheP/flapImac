#include <stdio.h>
#include <stdlib.h>

#include "draw.h"

GLuint loadTexture(const char* path) {
  // Chargement et traitement de la texture
   SDL_Surface* text_name = IMG_Load(path);
  if(text_name == NULL) {
    printf("Erreur lors du chargement de l'image.\n");
    exit(0);
  }

  GLuint textureID;
  glGenTextures(1, &textureID); //initie la texture
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, textureID); //attache la texture à un point de bind

  //change la valeur du paramètre MIN_FILTER de la texure à GL_LINEAR
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  //envoie les données à la carte graphique w:width h:height
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_name->w, text_name->h,
  0, GL_RGBA, GL_UNSIGNED_BYTE, text_name->pixels);

  // Libération des données CPU
  SDL_FreeSurface(text_name);

  glBindTexture(GL_TEXTURE_2D, 0); //bind la texture

  return textureID;
}

void drawShip(Ship* joueur, GLuint texture){
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

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glRotatef(90, 1.0, 1.0, 0.0);
            glTexCoord2f(1, 0);
            glVertex2f(joueur->Bx,joueur->By);
            glTexCoord2f(1, 1);
            glVertex2f(joueur->Bx,-joueur->By);
            glTexCoord2f(0, 1);
            glVertex2f(-joueur->Bx,-joueur->By);
            glTexCoord2f(0, 0);
            glVertex2f(-joueur->Bx,joueur->By);
        glEnd();

        glDisable(GL_TEXTURE_2D); //désactive le texturing
        glBindTexture(GL_TEXTURE_2D, 0); //debind la texture

    glPopMatrix();
    return;
}

void drawLazer(Lazer* lazer, GLuint texture){
    glPointSize(7);
    glPushMatrix();
        glScalef(0.015,0.025,0);
        glTranslatef(lazer->x,lazer->y,0);
        glBegin(GL_POINTS);
            glColor3ub(lazer->r,lazer->g,lazer->b);
            glVertex2f(0,0);
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        //bounding box
        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glTexCoord2f(1, 0);
            glVertex2f(BBLazer,BBLazer);
            glTexCoord2f(1, 1);
            glVertex2f(BBLazer,-BBLazer);
            glTexCoord2f(0, 1);
            glVertex2f(-BBLazer,-BBLazer);
            glTexCoord2f(0, 0);
            glVertex2f(-BBLazer,BBLazer);
        glEnd();

        glDisable(GL_TEXTURE_2D); //désactive le texturing
        glBindTexture(GL_TEXTURE_2D, 0); //debind la texture

    glPopMatrix();
    return;
}

void drawBlock(Block* block, GLuint texture){
    glPointSize(10);
    glPushMatrix();
        glScalef(0.015,0.025,0);
        glTranslatef(block->x, block->y, 0);
        glBegin(GL_POINTS);
            glColor3ub(255,120,0);
            glVertex2f(0,0);
        glEnd();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glTexCoord2f(0, 0);
            glVertex2f(BBBlock, BBBlock);
            glTexCoord2f(1, 0);
            glVertex2f(BBBlock, -BBBlock);
            glTexCoord2f(1, 1);
            glVertex2f(-BBBlock, -BBBlock);
            glTexCoord2f(0, 1);
            glVertex2f(-BBBlock, BBBlock);
        glEnd();

        glDisable(GL_TEXTURE_2D); //désactive le texturing
        glBindTexture(GL_TEXTURE_2D, 0); //debind la texture

    glPopMatrix();
    return;
}
void selectDrawBuff(Buff* buff){
    switch(buff->type){
        case 0:
            //rien à dessiner
            return;
        default:
            drawBuff(buff);
    }
}
void drawBuff(Buff* buff){
    glPointSize(10);
    glPushMatrix();
        glScalef(0.015,0.025,0);
        glTranslatef(buff->x, buff->y, 0);
        glEnd();

        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glVertex2f(BBBuff, BBBuff);
            glVertex2f(BBBuff, -BBBuff);
            glVertex2f(-BBBuff, -BBBuff);
            glVertex2f(-BBBuff, BBBuff);
        glEnd();
    glPopMatrix();
}

void drawBackground(GLuint texture){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glTexCoord2f(1, 0);
            glVertex2f(2, 2);
            glTexCoord2f(1, 1);
            glVertex2f(2, -2);
            glTexCoord2f(0, 1);
            glVertex2f(-2, -2);
            glTexCoord2f(0, 0);
            glVertex2f(-2, 2);
        glEnd();

        glDisable(GL_TEXTURE_2D); //désactive le texturing
        glBindTexture(GL_TEXTURE_2D, 0); //debind la texture

    glPopMatrix();
    return;
}

void drawEndScreen(GLuint texture){
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
            glColor3ub(255,255,255);
            glTexCoord2f(1, 0);
            glVertex2f(1, 1);
            glTexCoord2f(1, 1);
            glVertex2f(1, -1);
            glTexCoord2f(0, 1);
            glVertex2f(-1, -1);
            glTexCoord2f(0, 0);
            glVertex2f(-1, 1);
        glEnd();

        glDisable(GL_TEXTURE_2D); //désactive le texturing
        glBindTexture(GL_TEXTURE_2D, 0); //debind la texture

    glPopMatrix();
    return;
}
