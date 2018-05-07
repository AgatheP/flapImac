#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************  CONSTANTES  */
/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 1100;
static unsigned int WINDOW_HEIGHT = 500;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

/*********************************************************************************  STRUCTURES  */
typedef struct Point{
    float x,y;
} Point;

typedef struct Player{
    float x,y;
    int pvMax;
    int pv;
    //coordonnée d'un point de la bounding box relativement au centre du joueur (le 2e s'obtien avec une multiplication par -1)
    float Bx,By;
    //texture à rajouter par le suite
} Player;

typedef struct Lazer{
    float x,y;
    float vitesse;
    int dir; //dirrection du tir (1: ->, -1: <-)
    unsigned char r,g,b; //couleur
    struct Lazer* next;
}Lazer, *LazerList;

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


Player* allocPlayer(float x,float y, int pvMax, int width, int height){
    //créer le joueur et le placer au milieu de la hauteur de l'écrant
    if(x>WINDOW_WIDTH){
        printf("Erreur création du joueur\n");
        exit(1);
    }
    Player* p=(Player*) malloc(sizeof(Player));
    if(!p){exit(1);}
    p->pvMax=pvMax;
    p->pv=pvMax;
    p->y=y;
    p->x=x;
    //création de la bounding box
    p->Bx=(float)width/2.0;
    p->By=(float)height/2.0;
    return p;
}


void drawPlayer(Player* joueur){
    //pour l'instant on ne dessine que la bounding box
    glPointSize(8);
    glPushMatrix();
        //se placer au centre du joueur
        glScalef(0.015,0.015,0);
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
        glScalef(0.015,0.015,0);
        glTranslatef(lazer->x,lazer->y,0);
        glBegin(GL_POINTS);
            glColor3ub(lazer->r,lazer->g,lazer->b);
            glVertex2i(0,0);
        glEnd();
    glPopMatrix();
    return;
}

void drawLazers(LazerList list){
    while(list){
        drawLazer(list);
        list=list->next;
    }
}

Lazer* allocLazer(float x,float y, int dir,float vitesse,unsigned char r, unsigned char g, unsigned char b){
    Lazer* lazer=(Lazer*) malloc(sizeof(Lazer));
    if(!lazer){
        return NULL;
    }
    lazer->x = x;
    lazer->y = y;
    lazer->vitesse=vitesse;
    lazer->dir = dir;
    lazer->r = r;
    lazer->g = g;
    lazer->b = b;
    lazer->next=NULL;
    return lazer;
}


void addLazerToList(Lazer* lazer, LazerList* list){
    if(*list == NULL){
        *list = lazer;
    }else{
        addLazerToList(lazer, &(*list)->next);
    }
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
    SDL_WM_SetCaption("Hello l'IMAC!", NULL);
    int mooveUp=0;
    int mooveDown=0;

    //Initialisation du joueur
    Player* joueur=allocPlayer(-60,0,10,7,5);
    LazerList lazers=NULL;
    int vitesseJoueur=1;

    int posYmax=63; //pour l'instant fait à l'oeuil (il faudrait trouver le moyen de le calculer)
    int posYmin=-63;
    //info
    help();
    /* Boucle d'affichage */
    int loop = 1;
    while(loop) {

        /* Récupération du temps au début de la boucle */
        Uint32 startTime = SDL_GetTicks();
        
        /* Placer ici le code de dessin */
        glClear(GL_COLOR_BUFFER_BIT);
        drawPlayer(joueur);
        drawLazers(lazers);
        //opérations à faire sur tous les lazers
        //TO DO: faire une boucle pour appeler les fonction sur chaque lazer


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
                            SDL_Quit();
                            return EXIT_SUCCESS;
                            break;
                        case SDLK_h:
                            help();
                            break;
                        case SDLK_UP:
                            printf("*** UP ***\n");
                            mooveUp=1;
                            //joueur->y+=vitesseJoueur;
                            break;

                        case SDLK_DOWN:
                            printf("*** DOWN ***\n");
                            mooveDown=1;
                            //joueur->y-=vitesseJoueur;
                            break;

                        case SDLK_SPACE:
                            printf("*** SPACE ***\n");
                            addLazerToList(allocLazer(joueur->x,joueur->y,1,1,0,255,0),&lazers);
                            break;
                        case SDLK_i:
                            printf("*** INFO JOUEUR***\n");
                            printf("Position: (%f,%f)\n",joueur->x,joueur->y);
                            printf("PV: %d/%d\n",joueur->pv, joueur->pvMax);
                            printf("Position de la boundingBox:(%2f,%2f) (%2f,%2f)\n", joueur->Bx,joueur->By,-joueur->Bx,-joueur->By);
                            printf("Vitesse du joueur: %d\n",vitesseJoueur);
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
                joueur->y+=vitesseJoueur;
            }
            if(mooveDown && joueur->y>posYmin){
                joueur->y-=vitesseJoueur;
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

    /* Liberation des ressources associées à la SDL */ 
    SDL_Quit();

    return EXIT_SUCCESS;
}
