#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "structures.h"
#include "draw.h"
#include "level.h"
#include "parameters.h"

/*********************************************************************************  FONCTIONS  */

//detect if collision btw 2 BBs
int collision(int B1x, int B1y, int B2x, int B2y, int A1x, int A1y, int A2x, int A2y) {
    if(A1y>=B1y && B1y>=A2y){
        if(A1x>=B1x && B1x>=A2x){
            return 1;
        }
        if(A1x>=B2x && B2x>=A2x){
            return 1;
        }
    }
    if(A1y>=B2y && B2y>=A2y){
        if(A1x>=B2x && B2x>=A2x){
            return 1;
        }
        if(A1x>=B1x && B1x>=A2x){
            return 1;
        }
    }
    return 0;
}

void help(){
    printf("*** HELP ***\n");
    printf("Move: up and down arrows\n");
    printf("Shoot : spacebar\n");
    printf("Qui : q\n");
    printf("Player infos : i\n");
    printf("Help : h\n");
    return;
}

void tailleVirtuelle() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1., 1., -1., 1.);
}
/*********************************************************************************  MAIN  */
int main() {

    /* Initialisation de la SDL */
    if(-1 == SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Couldn't initialize SDL.\n");
        return EXIT_FAILURE;
    }

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Can't open window.\n");
        return EXIT_FAILURE;
    }
    tailleVirtuelle();
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_WM_SetCaption("FlapImac", NULL);

//array of textures used in program
    GLuint textures[9];
    textures[0] = loadTexture(spaceshipTex);
    textures[1] = loadTexture(foeTex);
    textures[2] = loadTexture(bulletTex);
    textures[3] = loadTexture(bulletFoeTex);
    textures[4] = loadTexture(blockTex);
    textures[5] = loadTexture(background);
    textures[6] = loadTexture(heart);
    textures[7] = loadTexture(gameOver);
    textures[8] = loadTexture(victory);

    int mooveUp=0;
    int mooveDown=0;

    //Initialize player
    int shipHeight = 8;
    Ship* joueur=allocShip(-60,0,10,7,shipHeight,50);
    int speedJoueur=2;

    LazerList lazers=NULL;
    ShipList foes=NULL;
    BlockList obstacles=NULL;
    BuffList buffs=NULL;


    int posYmax= (playableHeight-shipHeight)/2;
    int posYmin= -posYmax;

    int partieStatus=0; /* 0 = normal, 1 = victory, -1 = game over*/

    loadLevel(&foes, &obstacles);

    int loop = 1;

    while(loop) {
      if(partieStatus!=0){ //checking if game must continue
          break;
      }
        loop+=1;

        Uint32 startTime = SDL_GetTicks();

        //drawing code
        glClear(GL_COLOR_BUFFER_BIT);
        drawBackground(textures[5]);
        drawShip(joueur, textures[0]);

        /************************************************************************************To do on every lazer*/
        LazerList tmpLazer = lazers;
        while(tmpLazer!=NULL){
            tmpLazer->x+=tmpLazer->speed; //moves lazer
            //checks if lazer is still onscreen
            if(tmpLazer->x>=WINDOW_WIDTH*0.04 || tmpLazer->x<=WINDOW_WIDTH*-0.04){
                //deletes lazer from list
                removeLazerFromList(&tmpLazer, &lazers);
                tmpLazer=tmpLazer->next;
                continue;
            }
            //draws lazer
            if(tmpLazer->speed > 0)
              drawLazer(tmpLazer, textures[2]);
            else drawLazer(tmpLazer, textures[3]);
            //checks if lazer collided with smth

            tmpLazer=tmpLazer->next;
        }
        /************************************************************************************To do on all enemies*/
        ShipList tmpShip=foes;
        while(tmpShip!=NULL){
            tmpShip->x-=scrollSpeed;
            //checks if ship is still onscreen
            if(tmpShip->x<(WINDOW_WIDTH*-0.04)){
                printf("removeShip.\n");
                removeShipFromList(&tmpShip,&foes);
                tmpShip=tmpShip->next;
                continue;
            }
            //draws ship if onscreen
            if(tmpShip->x<=(WINDOW_WIDTH*0.04)){
                drawShip(tmpShip, textures[1]);
                if(loop % (tmpShip->fireRate)==0){
                    addLazerToList(allocLazer(tmpShip->x, tmpShip->y, -0.7 -scrollSpeed, 255, 0, 0), &lazers);
                }
            }
            tmpShip=tmpShip->next;
        }
        if(foes == NULL) {
          //no more enemies, the player won
          partieStatus = 1;
        }
        /************************************************************************************To do on all obstacles*/
        BlockList tmpBlock=obstacles;
        while(tmpBlock != NULL){
            tmpBlock->x -= scrollSpeed;

            if(tmpBlock->x<(WINDOW_WIDTH*-0.04)){
                removeBlockFromList(&tmpBlock, &obstacles);
                tmpBlock=tmpBlock->next;
                continue;
            }
            //if block onscreen
            if(tmpBlock->x<=(WINDOW_WIDTH*0.04)){
                //draws block
                drawBlock(tmpBlock, textures[4]);
                float helpPlayer = 1.2; //prevents player from colliding with blocks too soon
                //detects collision with player with blocks on second part of screen (less calculations)
                if(tmpBlock->x < -50){
                    if(collision(joueur->x+joueur->Bx - helpPlayer, joueur->y+joueur->By - helpPlayer,joueur->x-joueur->Bx - helpPlayer,
                      joueur->y-joueur->By - helpPlayer,
                        tmpBlock->x+BBBlock, tmpBlock->y+BBBlock, tmpBlock->x-BBBlock, tmpBlock->y-BBBlock)){
                        printf("Ouch! You smashed on a wall !\n");
                        partieStatus=-1;
                    }
                }
                //detects lazer collisions
                tmpLazer=lazers;
                while(tmpLazer!=NULL){
                    if(collision(tmpLazer->x+BBLazer, tmpLazer->y+BBLazer,tmpLazer->x-BBLazer, tmpLazer->y-BBLazer,
                        tmpBlock->x+BBBlock, tmpBlock->y+BBBlock, tmpBlock->x-BBBlock, tmpBlock->y-BBBlock)){
                        removeLazerFromList(&tmpLazer, &lazers);
                    }
                    tmpLazer=tmpLazer->next;
                }
            }
            tmpBlock=tmpBlock->next;
        }
        /************************************************************************To do on all buffs*/
        BuffList tmpBuff=buffs;
        while(tmpBuff!=NULL){
            tmpBuff->x -= scrollSpeed;
            //le buff sort de l'écrant
            if(tmpBuff->x < WINDOW_WIDTH*-0.04){
                printf("removeBuff\n");
                removeBuffFromList(&tmpBuff, &buffs);
                tmpBuff=tmpBuff->next;
                continue;
            }
            //if onscreen
            if(tmpBuff->x <= WINDOW_WIDTH*0.04){
                selectDrawBuff(tmpBuff);
                //TO DO: gestion collision avec le joueur
            }
            tmpBuff=tmpBuff->next;
        }

        //displays number of hp left
        if(joueur->hp > 0) {
          for(int i = 0; i < joueur->hp; i++) {
            drawHeart(textures[6], -0.9+(float)i/15, 0.9);
          }
        }


        /************************************************************************Collision manager*/
        //Detects lazer/player et lazer/enemy collisions
        tmpLazer=lazers;
        while(tmpLazer!=NULL){
            //lazer/joueur
            if(tmpLazer->x<-50 && tmpLazer->speed<0){
                //tests only lazers that goes to the player
                if(collision(tmpLazer->x+BBLazer, tmpLazer->y+BBLazer,tmpLazer->x-BBLazer, tmpLazer->y-BBLazer,
                    joueur->x+joueur->Bx, joueur->y+joueur->By,joueur->x-joueur->Bx, joueur->y-joueur->By)){
                    removeLazerFromList(&tmpLazer, &lazers);
                    joueur->hp -= 1;
                    printf("You've been hit! HP: %d/%d\n",joueur->hp, joueur->hpMax);
                    if(joueur->hp <= 0){
                        printf("Ran out of lives!\n");
                        partieStatus = -1;
                    }
                }
            }
            // lazer/énemi
            if(tmpLazer->speed>0){
                tmpShip=foes;
                while(tmpShip!=NULL){
                    if(collision(tmpLazer->x+BBLazer, tmpLazer->y+BBLazer,tmpLazer->x-BBLazer, tmpLazer->y-BBLazer,
                        tmpShip->x+tmpShip->Bx, tmpShip->y+tmpShip->By, tmpShip->x-tmpShip->Bx, tmpShip->y-tmpShip->By)){
                        removeShipFromList(&tmpShip, &foes);
                        removeLazerFromList(&tmpLazer, &lazers);
                        break;
                    }
                    tmpShip=tmpShip->next;
                }
            }
            tmpLazer=tmpLazer->next;
        }
        if(partieStatus == 1){
            printf("Victory!\n");
            drawEndScreen(textures[8]);
            SDL_Delay(3000);
        }
        if(partieStatus == -1){
            printf("You lost!\n");
            drawEndScreen(textures[7]);
            SDL_Delay(3000);
          }

        /* Events loop */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }
            switch(e.type) {
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;

                case SDL_MOUSEMOTION:
                    break;

                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_q:
                            printf("Quit program.\n");
                            loop=0;
                            break;
                        case SDLK_h:
                            help();
                            break;
                        case SDLK_UP:
                            mooveUp=1;
                            //joueur->y+=speedJoueur;
                            break;

                        case SDLK_DOWN:
                            mooveDown=1;
                            //joueur->y-=speedJoueur;
                            break;

                        case SDLK_SPACE:
                            addLazerToList(allocLazer(joueur->x,joueur->y,1 + scrollSpeed,0,255,0),&lazers);
                            break;
                        case SDLK_i:
                            printf("*** INFOS PLAYER***\n");
                            printf("Position: (%f,%f)\n",joueur->x,joueur->y);
                            printf("HP: %d/%d\n",joueur->hp, joueur->hpMax);
                            printf("BoundingBox position:(%2f,%2f) (%2f,%2f)\n", joueur->Bx,joueur->By,-joueur->Bx,-joueur->By);
                            printf("Player speed: %d\n",speedJoueur);
                            break;
                        default: break;
                    }
                    break;
                // touche relachée
                case SDL_KEYUP:
                    switch(e.key.keysym.sym){
                        case SDLK_UP:
                            mooveUp=0;
                            break;
                        case SDLK_DOWN:
                            mooveDown=0;
                            break;
                        default: break;
                    }
                    break;

                case SDL_VIDEORESIZE:
                    printf("You can't change the window size.\n");
                    break;

                default:
                    break;
            }
        }
        //moves player unless two arrows pressed at once
        if(mooveUp==0 ||mooveDown==0){
            if(mooveUp && joueur->y<posYmax){
                joueur->y+=speedJoueur;
            }
            if(mooveDown && joueur->y>posYmin){
                joueur->y-=speedJoueur;
            }
        }

        SDL_GL_SwapBuffers();

        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

    }
    /*END of game*/
    printf("Cleaning memory:\n");
    printf("- free player\n");
    free(joueur);
    printf("- free lazers \n");
    if(lazers != NULL){
        freeLazers(&lazers);
    }else{
        printf("(no lazers to free)\n");
    }
    printf("- free enemies\n");
    if(foes != NULL){
        freeShipList(&foes);
    }else{
        printf("(no enemies to free)\n");
    }
    printf("- free blocks\n");
    if(obstacles !=NULL){
        freeAllBlock(&obstacles);
    }else{
        printf("(no blocks to free)\n");
    }

    if(partieStatus == 1){
        drawEndScreen(textures[8]);
        SDL_Delay(3000);
    }else{
        drawEndScreen(textures[7]);
        SDL_Delay(3000);
    }

    printf("- free textures\n");

    glDeleteTextures(9, textures);

    SDL_Quit();

    return EXIT_SUCCESS;
}
