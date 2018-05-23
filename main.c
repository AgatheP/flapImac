#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "parameters.h"
#include "structures.h"
#include "draw.h"


/*********************************************************************************  FONCTIONS  */

void windowResize(int height, int width){
    WINDOW_WIDTH=width;
    WINDOW_HEIGHT=height;
    printf("Globales: h:%d, w:%d\n",WINDOW_HEIGHT,WINDOW_WIDTH );
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.,1.,-1.,1.);
    SDL_SetVideoMode(WINDOW_WIDTH,WINDOW_HEIGHT,BIT_PER_PIXEL,SDL_OPENGL | SDL_RESIZABLE);
}

//détercte si collision entre 2 bounding box
int collision(int B1x, int B1y, int B2x, int B2y,
   int B3x, int B3y, int B4x, int B4y) {
   if(B3x <= B1x <= B4x && B3y <= B1y <= B4y
     || B3x <= B2x <= B4x && B3y <= B2y <= B4y) {
     printf("Bbox 1 dans bbox2\n");
     return 1;
   }
   if(Bx1 <= Bx3 <= Bx2 && By1 <= By3 <= By2
     || Bx1 <= Bx4 <= Bx2 && By1 <= By4 <= By2) {
     printf("Bbox2 dans bbox1\n");
     return 1;
   }
   return 0
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
/*********************************************************************************  MAIN  */
int main(int argc, char** argv) {

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
    glClearColor(0.1,0.1,0.1,1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Titre de la fenêtre */
    SDL_WM_SetCaption("Flapimac", NULL);
    int mooveUp=0;
    int mooveDown=0;
    float scrollSpeed=0.35; //vitesse de déplacement horizontal du jeu

    //Initialisation du joueur
    Ship* joueur=allocShip(-60,0,10,7,5,50);
    int speedJoueur=1;

    LazerList lazers=NULL;
    ShipList foes=NULL;


    int posYmax=37; //pour l'instant fait à l'oeuil (il faudrait trouver le moyen de le calculer)
    int posYmin=-37;

    /* Initialisation*/
    //Lire le PPM pour connaitre la position de chaque énémi/obstache/bonnus et les metres dans les listes

    /* Boucle d'affichage */
    int loop = 1;

    /*Test*/
    addShipToList(allocShip(0,0,1,5,5,40),&foes);
    addShipToList(allocShip(0,20,1,5,5,40),&foes);
    /*Test*/
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();

        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);
        drawShip(joueur);
        //************************************************************************************opérations à faire sur tous les lazers*/
        LazerList tmpLazer = lazers;
        while(tmpLazer!=NULL){
            tmpLazer->x+=tmpLazer->speed; //déplacer le lazer
            //vérifier si le lazer est tjrs dans l'écran
            if(tmpLazer->x>=WINDOW_WIDTH*0.06 || tmpLazer->x<WINDOW_WIDTH*-0.06){
                // retirer le lazer de la liste
                removeLazerFromList(&tmpLazer, &lazers);
                tmpLazer=tmpLazer->next;
                continue;
            }
            //dessiner le lazer
            drawLazer(tmpLazer);
            //chercher si le lazer est entré en collistion avec quelquechose

            tmpLazer=tmpLazer->next;
        }
        //************************************************************************************opérations à faire sur tous les Enemis*/
        ShipList tmpShip=foes;
        while(tmpShip!=NULL){
            tmpShip->x-=scrollSpeed;

            //vérifier si le vaiseau est tjrs dans l'écran
            if(tmpShip->x>=(WINDOW_WIDTH*0.06) || tmpShip->x<(WINDOW_WIDTH*-0.06)){
                removeShipFromList(&tmpShip,&foes);
                tmpShip=tmpShip->next;
                continue;
            }
            drawShip(tmpShip);
            if(loop%tmpShip->fireRate==0){
                printf("tirer\n");
            }

            tmpShip=tmpShip->next;
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
                    printf("window resize\n");
                    //printf("Nouvelle taille: h:%d, w:%d\n",e.resize.h,e.resize.w);
                    windowResize(e.resize.h,e.resize.w);
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
    printf("Libération de la mémoire:\n");
    printf("- Libération joueur\n");
    free(joueur);
    printf("- Liberation lazers\n");
    if(lazers != NULL){
        freeLazers(&lazers);
    }else{
        printf("(pas de lazers à supprimer)\n");
    }
    printf("- Liberation des énémis\n");
    if(foes != NULL){
        freeShipList(&foes);
    }else{
        printf("(pas d'énemis à libérer.)\n");
    }
    /* Liberation des ressources associées à la SDL */
    SDL_Quit();

    return EXIT_SUCCESS;
}
