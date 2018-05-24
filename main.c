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

//détercte si collision entre 2 bounding box
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
    printf("Bouger: fleches haut et bas\n");
    printf("Tirer: barre espace\n");
    printf("Quiter: q\n");
    printf("Info sur le joueur: i\n");
    printf("Aide: h\n");
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
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

    /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
    if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
        fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
        return EXIT_FAILURE;
    }
    tailleVirtuelle();
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Flapimac", NULL);

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


    //Initialisation du joueur
    int shipHeight = 8;
    Ship* joueur=allocShip(-60,0,10,7,shipHeight,50);
    int speedJoueur=2;

    LazerList lazers=NULL;
    ShipList foes=NULL;
    BlockList obstacles=NULL;
    BuffList buffs=NULL;


    int posYmax= (playableHeight-shipHeight)/2; //pour l'instant fait à l'oeuil (il faudrait trouver le moyen de le calculer)
    int posYmin= -posYmax;

    /* Initialisation*/
    int partieStatus=0; /* 0= normal, 1=victoire, -1= game over*/
    //Lire le PPM pour connaitre la position de chaque énémi/obstache/bonnus et les metres dans les listes
    loadLevel(&foes, &obstacles);
    /* Boucle d'affichage */
    int loop = 1;

    while(loop) {
      if(partieStatus!=0){ //on vérifit si on doit continuer la partie
          break;
      }
        loop+=1;
        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);
        drawBackground(textures[5]);
        drawShip(joueur, textures[0]);
        /************************************************************************************opérations à faire sur tous les lazers*/
        LazerList tmpLazer = lazers;
        while(tmpLazer!=NULL){
            tmpLazer->x+=tmpLazer->speed; //déplacer le lazer
            //vérifier si le lazer est tjrs dans l'écran
            if(tmpLazer->x>=WINDOW_WIDTH*0.04 || tmpLazer->x<=WINDOW_WIDTH*-0.04){
                // retirer le lazer de la liste
                removeLazerFromList(&tmpLazer, &lazers);
                tmpLazer=tmpLazer->next;
                continue;
            }
            //dessiner le lazer
            if(tmpLazer->speed > 0)
              drawLazer(tmpLazer, textures[2]);
            else drawLazer(tmpLazer, textures[3]);
            //chercher si le lazer est entré en collistion avec quelquechose

            tmpLazer=tmpLazer->next;
        }
        /************************************************************************************opérations à faire sur tous les Enemis*/
        ShipList tmpShip=foes;
        while(tmpShip!=NULL){
            tmpShip->x-=scrollSpeed;
            //vérifier que le vaiseau n'a pas traversé l'écran
            if(tmpShip->x<(WINDOW_WIDTH*-0.04)){
                printf("removeShip.\n");
                removeShipFromList(&tmpShip,&foes);
                tmpShip=tmpShip->next;
                continue;
            }
            //déssiner le vaiseau s'il est dans l'écran
            if(tmpShip->x<=(WINDOW_WIDTH*0.04)){
                drawShip(tmpShip, textures[1]);
                if(loop % (tmpShip->fireRate)==0){
                    addLazerToList(allocLazer(tmpShip->x, tmpShip->y, -0.7, 255, 0, 0), &lazers);
                }
            }
            tmpShip=tmpShip->next;
        }
        /************************************************************************************opérations à faire sur tous les Obstacles*/
        BlockList tmpBlock=obstacles;
        while(tmpBlock != NULL){
            tmpBlock->x -= scrollSpeed;

            if(tmpBlock->x<(WINDOW_WIDTH*-0.04)){
                printf("removeBlock.\n");
                removeBlockFromList(&tmpBlock, &obstacles);
                tmpBlock=tmpBlock->next;
                continue;
            }
            //si le block est dans l'écran
            if(tmpBlock->x<=(WINDOW_WIDTH*0.04)){
                //dessin du block
                drawBlock(tmpBlock, textures[4]);
                float helpPlayer = 1.2; //prevents player from colliding with blocks too soon
                //détecter la collision avec le joueur avec les block dans la seconde patie de l'écran pour faire mois de calculs
                if(tmpBlock->x < -50){
                    if(collision(joueur->x+joueur->Bx - helpPlayer, joueur->y+joueur->By - helpPlayer,joueur->x-joueur->Bx - helpPlayer,
                      joueur->y-joueur->By - helpPlayer,
                        tmpBlock->x+BBBlock, tmpBlock->y+BBBlock, tmpBlock->x-BBBlock, tmpBlock->y-BBBlock)){
                        printf("Ouch! tu t'es pris un mur!\n");
                        partieStatus=-1;
                    }
                }
                //Détection des collistions de lazers
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
        /************************************************************************opérations sur les buffs*/
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
            //traitement si dans l'écran
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


        /************************************************************************Gestion de collisions*/
        //Détection collision lazer/joueur et lazer/énemi
        tmpLazer=lazers;
        while(tmpLazer!=NULL){
            //lazer/joueur
            if(tmpLazer->x<-50 && tmpLazer->speed<0){
                //on ne teste que les lazers dans la bonne dirrection et dans la zonne du joueur
                if(collision(tmpLazer->x+BBLazer, tmpLazer->y+BBLazer,tmpLazer->x-BBLazer, tmpLazer->y-BBLazer,
                    joueur->x+joueur->Bx, joueur->y+joueur->By,joueur->x-joueur->Bx, joueur->y-joueur->By)){
                    removeLazerFromList(&tmpLazer, &lazers);
                    joueur->hp -= 1;
                    printf("vous avez été touché! HP: %d/%d\n",joueur->hp, joueur->hpMax);
                    if(joueur->hp <= 0){
                        printf("Plus de vie!\n");
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
            printf("victoire\n");
            drawEndScreen(textures[8]);
            SDL_Delay(3000);
        }
        if(partieStatus == -1){
            printf("perdu\n");
            drawEndScreen(textures[7]);
            SDL_Delay(3000);
          }

        /* Boucle traitant les evenements */
        SDL_Event e;
        while(SDL_PollEvent(&e)) {

            /* L'utilisateur ferme la fenêtre : */
            if(e.type == SDL_QUIT) {
                loop = 0;
                break;
            }

            /* Quelques exemples de traitement d'evenements : */
            switch(e.type) {

                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;

                /* Mouvement souris */
                case SDL_MOUSEMOTION:
                    break;

                /* Touche clavier */
                //pressée
                case SDL_KEYDOWN:
                    //printf("touche pressée (code = %d)\n", e.key.keysym.sym);
                    switch(e.key.keysym.sym){
                        case SDLK_q:
                            printf("Quiter le programme.\n");
                            loop=0;
                            //SDL_Quit();
                            //return EXIT_SUCCESS;
                            break;
                        case SDLK_h:
                            help();
                            break;
                        case SDLK_UP:
                            printf("*** UP ***\n");
                            mooveUp=1;
                            //joueur->y+=speedJoueur;
                            break;

                        case SDLK_DOWN:
                            printf("*** DOWN ***\n");
                            mooveDown=1;
                            //joueur->y-=speedJoueur;
                            break;

                        case SDLK_SPACE:
                            printf("*** SPACE ***\n");
                            addLazerToList(allocLazer(joueur->x,joueur->y,1,0,255,0),&lazers);
                            break;
                        case SDLK_i:
                            printf("*** INFO JOUEUR***\n");
                            printf("Position: (%f,%f)\n",joueur->x,joueur->y);
                            printf("PV: %d/%d\n",joueur->hp, joueur->hpMax);
                            printf("Position de la boundingBox:(%2f,%2f) (%2f,%2f)\n", joueur->Bx,joueur->By,-joueur->Bx,-joueur->By);
                            printf("Vitesse du joueur: %d\n",speedJoueur);
                            break;
                        default: break;
                    }
                    break;
                // touche relachée
                case SDL_KEYUP:
                    switch(e.key.keysym.sym){
                        case SDLK_UP:
                            printf("*** UP relaché ***\n");
                            mooveUp=0;
                            break;
                        case SDLK_DOWN:
                            printf("*** DOWN relaché ***\n");
                            mooveDown=0;
                            break;
                        default: break;
                    }
                    break;

                /* Redimentionnement de la fenetre */
                case SDL_VIDEORESIZE:
                    printf("Vous ne pouver pas changer la taille de la fenetre\n");
                    break;

                default:
                    break;
            }
        }
        // Deplacer le joueur sauf s'il apuis sur les 2 flèches en même temps
        if(mooveUp==0 ||mooveDown==0){
            if(mooveUp && joueur->y<posYmax){
                joueur->y+=speedJoueur;
            }
            if(mooveDown && joueur->y>posYmin){
                joueur->y-=speedJoueur;
            }
        }

        /* Echange du front et du back buffer : mise à jour de la fenêtre */
        SDL_GL_SwapBuffers();

        /* Calcul du temps écoulé */
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        /* Si trop peu de temps s'est écoulé, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

    }
    /*FIN du jeu*/


    printf("Libération de la mémoire:\n");
    printf("- Libération joueur\n");
    free(joueur);
    printf("- Liberation lazers\n");
    if(lazers != NULL){
        freeLazers(&lazers);
    }else{
        printf("(pas de lazers à supprimer)\n");
    }
    printf("- Liberation des ennemis\n");
    if(foes != NULL){
        freeShipList(&foes);
    }else{
        printf("(pas d'énemis à libérer.)\n");
    }
    printf("- Liberation des block\n");
    if(obstacles !=NULL){
        freeAllBlock(&obstacles);
    }else{
        printf("(pas de block à libérer)\n");
    }


    if(partieStatus == 1){
        printf("victoire\n");
        drawEndScreen(textures[8]);
        SDL_Delay(3000);
    }else{
        printf("perdu\n");
        drawEndScreen(textures[7]);
        SDL_Delay(3000);
    }

    printf("- Liberation des textures\n");


    glDeleteTextures(9, textures);

    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
