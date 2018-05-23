#include "structures.h"

Ship* allocShip(float x,float y, int hpMax, int width, int height, int fireRate){
    //créer le joueur et le placer au milieu de la hauteur de l'écrant
    Ship* p=(Ship*) malloc(sizeof(Ship));
    if(!p){exit(1);}
    p->hpMax=hpMax;
    p->hp=hpMax;
    p->y=y;
    p->x=x;
    //création de la bounding box
    p->Bx=(float)width/2.0;
    p->By=(float)height/2.0;
    p->fireRate=fireRate;
    p->next=NULL;
    return p;
}

void addShipToList(Ship* ship, ShipList* list){
    if(*list==NULL){
        *list = ship;
    }else{
        addShipToList(ship,&(*list)->next);
    }
}

void freeShipList(ShipList* l){
    if((*l)->next!=NULL){
        freeShipList(&(*l)->next);
        l=NULL;
        //free(l);
    }else{
        l=NULL;
        //free(l);
    }
}

int removeShipFromList(ShipList* ship, ShipList* list){
    if(*list==NULL || *ship==NULL) return 0;
    //s'il s'agit du premier de la liste
    if(*ship == *list){
        *list=(*list)->next;
        ship=NULL;
        //free(*ship);
        return 1;
    }
    if(*ship == (*list)->next){
        (*list)->next=(*ship)->next;
        ship=NULL;
        //free(*ship);
        return 1;
    }
    return removeShipFromList(ship,&(*list)->next);
}

Lazer* allocLazer(float x,float y, float speed,unsigned char r, unsigned char g, unsigned char b){
    Lazer* lazer=(Lazer*) malloc(sizeof(Lazer));
    if(!lazer){
        return NULL;
    }
    lazer->x = x;
    lazer->y = y;
    lazer->speed=speed;
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

int removeLazerFromList(LazerList* lazer, LazerList* list){
    if(*list == NULL || *lazer == NULL) return 0;
    //il faut retirer le premier de la liste
    if(*lazer == *list){
        *list=(*list)->next;
        //free(*lazer);
        return 1;
    }
    //il faut supprimer celui qui n'est pas le premier
    if(*lazer==(*list)->next){
        (*list)->next=(*lazer)->next;
        //free(*lazer);
        return 1;
    }
    return removeLazerFromList(lazer, &(*list)->next);
}

void freeLazers(LazerList* l){
    if((*l)->next!=NULL){
        LazerList tmpList=(*l)->next;
        l=NULL;
        freeLazers(&tmpList);
    }else{
        l=NULL;
        free(*l);
    }
}

Block* allocBlock(float x, float y){
    Block* b=(Block*) malloc(sizeof(Block));
    if(!b){exit(1);}
    b->x=x;
    b->y=y;
    b->next=NULL;
    return b;
}

void addBlockToList(Block* block, BlockList* list){
    if(*list == NULL){
        *list = block;
    }else{
        addBlockToList(block, &(*list)->next);
    }
}

int removeBlockFromList(BlockList* block, BlockList* list){
    if(*list == NULL || *block == NULL) return 0;
    if(*block == *list){
        *list=(*list)->next;
        free(*block);
        return 1;
    }
    if(*block == (*list)->next){
        (*list)->next=(*block)->next;
        free(*block);
        return 1;
    }
    return removeBlockFromList(block, &(*list)->next);
}

void freeAllBlock(BlockList* list){
    if( (*list)->next != NULL){
        BlockList tmpList=(*list)->next;
        list=NULL;
        freeAllBlock(&tmpList);
    }else{
        list=NULL;
        //free(*list);
    }
}