#include <stdlib.h>
#include <stdio.h>
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
    (carte->obstacles) = malloc(sizeof(obstacle) * HEIGTH);
    (carte->len) = HEIGTH;
    for(int i = 0 ; i < HEIGTH ; i++) {
        //Pour l'instant on considère que touts les obtacles n'ont qu'une lignes
        NB_LINES(OBSTACLES(i, carte)) = 1;
        OBSTACLES(i, carte)->lines = malloc(sizeof(line) * NB_LINES(OBSTACLES(i, carte)));
        (LINES(0, OBSTACLES(i, carte))->tiles) = malloc(sizeof(tile)*LENGTH);
        for(int j=0;j<LENGTH;j++) {
            TYPE(tile_map(j, i, carte)) = '0';
            if ((j==0) || (j==LENGTH-1) || (i==0) || (i==HEIGTH-1)) {
                IS_BLOCK(tile_map(j, i, carte)) = true;
            } else {
                IS_BLOCK(tile_map(j, i, carte)) = false;
            }
            IS_DANGER(tile_map(j, i, carte)) = false;
            if ((j==X_INI) && (i==Y_INI)) {
                IS_PLAYER(tile_map(j, i, carte)) = true;
            } else {
                IS_PLAYER(tile_map(j, i, carte)) = false;
            }
        }
    }
    return carte;
}

//Les structures sont sur des zones mutables (malloc) et donc on en profite pour les muter.
void move(player *joueur,map *carte, int x, int y) {
    if(IS_BLOCK(tile_map(X_PLAYER(joueur)+x, Y_PLAYER(joueur)+y, carte)) == false) {
        IS_PLAYER(tile_map(X_PLAYER(joueur), Y_PLAYER(joueur), carte)) = false;
        X_PLAYER(joueur) += x;
        Y_PLAYER(joueur) += y;
        IS_PLAYER(tile_map(X_PLAYER(joueur), Y_PLAYER(joueur), carte)) = true;
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
