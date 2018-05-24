#include <stdio.h>
#include <stdlib.h>

#include "draw.h"

GLuint loadTexture(const char* path) {
   SDL_Surface* text_name = IMG_Load(path);
  if(text_name == NULL) {
    printf("Error loading image\n");
    exit(0);
  }

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_name->w, text_name->h,
  0, GL_RGBA, GL_UNSIGNED_BYTE, text_name->pixels);

  SDL_FreeSurface(text_name);

  glBindTexture(GL_TEXTURE_2D, 0);

  return textureID;
}

void drawShip(Ship* joueur, GLuint texture){
    glPointSize(8);
    glPushMatrix();
        //player's center
        glScalef(0.015,0.025,0);
        glTranslatef(joueur->x,joueur->y,0);
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

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

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

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

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

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    return;
}
void selectDrawBuff(Buff* buff){
    switch(buff->type){
        case 0:
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

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

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

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

    glPopMatrix();
    return;
}

void drawHeart(GLuint texture, float x, float y) {
  glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glBegin(GL_QUADS);
          glColor3ub(255,255,255);
          glTexCoord2f(1, 0);
          glVertex2f(x+0.03, y+0.03);
          glTexCoord2f(1, 1);
          glVertex2f(x+0.03, y-0.03);
          glTexCoord2f(0, 1);
          glVertex2f(x-0.03, y-0.03);
          glTexCoord2f(0, 0);
          glVertex2f(x-0.03, y+0.03);
      glEnd();

      glDisable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, 0);

  glPopMatrix();
  return;
}
