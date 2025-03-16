#include <stdio.h>
#include <stdlib.h>
#include "PrototypeRoutes.h"


line *line_map(int y, map *carte){ 
    int nb_obst = 0;
    int nb_line = 0;
    for(int i = 0; i < y; i++){
        nb_line += 1;
        if (nb_line >= NB_LINES(OBSTACLES(nb_obst,carte))) {
            nb_line = 0;
            nb_obst += 1;
        }
    }
    return LINES(nb_line,OBSTACLES(nb_obst,carte));
} 

tile *tile_map(int x, int y, map *carte) {
    return TILES(x, line_map(y, carte));
}

map *initialize_map(void) {
    map* carte;
    carte = malloc(sizeof(map));
    GEN_LINES(carte)=0;
    (carte->obstacles) = malloc(sizeof(obstacle) * (HEIGTH+FUTURE_HEIGTH));
    for(int i = 0 ; i < HEIGTH+FUTURE_HEIGTH ; i++) {   //UNE FOIS OBSTACLES MULTI LIGNES DEFINIES:boucle genere obstacles tant que HEIGTH n'est pas dépassé, de taille max FUTURE_HEIGTH
        OBSTACLES(i, carte)->lines = malloc(sizeof(tile)*LENGTH); 
        //Pour l'instant on considère que touts les obtacles n'ont qu'une lignes
        NB_LINES(OBSTACLES(i, carte)) = 1;
        GEN_LINES(carte)=GEN_LINES(carte)+1;
        for(int j=0;j<LENGTH;j++) {
            TYPE(tile_map(j, i, carte)) = '_';
            if ((j==0) || (j==LENGTH-1)) {
                IS_BLOCK(tile_map(j, i, carte)) = true;
                TYPE(tile_map(j, i, carte)) = '0';
            } else {
                IS_BLOCK(tile_map(j, i, carte)) = false;
            }
            if (i==0) {
                IS_DANGER(tile_map(j, i, carte)) = true;
            }
            else {
                IS_DANGER(tile_map(j, i, carte)) = false;
            }
        }
    }
    return carte;
}

//Les structures sont sur des zones mutables (malloc) et donc on en profite pour les muter.
void move(player *joueur,map *carte, int x, int y) {
    if(IS_BLOCK(tile_map(X_PLAYER(joueur)+x, Y_PLAYER(joueur)+y, carte)) == false) {
        X_PLAYER(joueur) += x;
        Y_PLAYER(joueur) += y;
    }
    return ;
}

//Les fonctions move locales pour une utilisation plus simples
void move_up(player *joueur, map *carte) {
    move(joueur,carte,0,1);
}
void move_down(player *joueur, map *carte) {
    move(joueur,carte,0,-1);
}
void move_right(player *joueur, map *carte) {
    move(joueur,carte,1,0);
}
void move_left(player *joueur, map *carte) {
    move(joueur,carte,-1,0);
}

void freeline(line* ligne) {
    for (int i=0;i<LENGTH;i++) {
        free(TILES(i,ligne));
    }
    free(ligne);
}

void freeobstacle(obstacle* obs) {
    for (int i=0;i<NB_LINES(obs);i++) {
        freeline(LINES(i,obs));
    }
    free(obs);
}

void freemap(map* carte) {
    for (int i=0;i<LEN(carte);i++) {
        freeline(OBSTACLES(i,carte));
    }
    free(carte);
}

void map_shift(map* carte,player* joueur) {
    if (NB_LINES(OBSTACLES(1,carte))==1) {
        free(OBSTACLES(1,carte));
        LEN(carte)=LEN(carte)-1;
        for (int i=2;i<LEN(carte);i++) {
            *OBSTACLES(i-1,carte)=*OBSTACLES(i,carte);
        }
    }
    else {
        obstacle *exiting_obstacle;
        NB_LINES(exiting_obstacle)=NB_LINES(OBSTACLES(1,carte))-1;
        for (int i=0;i<NB_LINES(exiting_obstacle)-1;i++) {
            *LINES(i,exiting_obstacle)=*LINES(i+1,OBSTACLES(1,carte));
        free(OBSTACLES(1,carte));
        *OBSTACLES(1,carte)=*exiting_obstacle;
        }
    }
    GEN_LINES(carte)=GEN_LINES(carte)-1;
    Y_PLAYER(joueur)=Y_PLAYER(joueur)-1;
    if (GEN_LINES(carte)==HEIGTH) { 
        obstacle* futur;
        futur->lines = malloc(sizeof(tile)*LENGTH); //car une seule ligne pour l'instant
        NB_LINES(futur) = 1;
        for (int i=0;i<LENGTH;i++) {
            TYPE(TILES(i,LINES(0,futur))) = '_';
            if ((i==0) || (i==LENGTH-1)) {
                IS_BLOCK(TILES(i,LINES(0,futur))) = true;
                TYPE(TILES(i,LINES(0,futur))) = '0';
        } else {
            IS_BLOCK(TILES(i,LINES(0,futur))) = false;
        }
        IS_DANGER(TILES(i,LINES(0,futur))) = false;
        }
        *OBSTACLES(LEN(carte),carte)=*futur;
        LEN(carte)++;
        GEN_LINES(carte)=GEN_LINES(carte)+NB_LINES(futur);
    }
} 

bool test_death(player *joueur,map* carte) {
    return IS_DANGER(tile_map(X_PLAYER(joueur),Y_PLAYER(joueur),carte));
}

int main(void) {
    printf("coucou");
    return 0;
}

