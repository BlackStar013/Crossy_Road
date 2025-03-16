#pragma once

#include <stdbool.h>

typedef struct _tile {
    bool is_block;
    bool is_danger;
    char type; //si on voudra en graphique des cases différentes
                   //Pas que en graphique 
} tile;

#define IS_BLOCK(tile) (tile->is_block)
#define IS_DANGER(tile) (tile->is_danger)
#define TYPE(tile) (tile->type)


typedef struct _line {
    //La tailles de tiles est LENGTH 
    tile* tiles;
} line;

#define TILES(i,line) ((line->tiles)+i) //Donne la ième case de la ligne, ( i < LENGHT)


typedef struct _obstacle {
    int nb_lines;
    line* lines;
} obstacle;

#define LINES(i,obstacle) ((obstacle->lines)+i) //Donne la ième ligne de l'obstacles ( i < NB_LINES )
#define NB_LINES(obstacle) ((obstacle->nb_lines))

typedef struct _map {
    //La somme de "nb_lines" de content fait HEIGTH
    int len;
    int nb_generated;
    obstacle* obstacles;
} map;

#define OBSTACLES(i,carte) ((carte->obstacles)+i) //Donne le ième obstacle de la carte ( i < LEN )
#define LEN(carte) ((carte->len))
#define GEN_LINES(carte) ((carte->nb_generated))

//On considère x horizontale, y verticale pour le reste du programme

typedef struct _player{
    int x;
    int y;
    int character_id;
} player;

#define X_PLAYER(joueur) (joueur->x)
#define Y_PLAYER(joueur) (joueur->y)
#define ID_PLAYER(joueur) (joueur->character_id)

//Hauteur et largeur de la carte
#define HEIGTH 16
#define LENGTH 16

//Position initales du joueur
#define X_INI 10
#define Y_INI 10

#define FUTURE_HEIGTH 1

//Déclaration des fonctions
line* line_map(int y, map *carte); //Renvoie la ligne n°y de la carte
tile* tile_map(int x, int y, map *carte);//Renvoie la case (x,y) de la carte
map* initialize_map(void); //Initialise la carte à partir des informations définies pour le jeu
void movep(player *joueur,map *carte, int x, int y); //Bouge le joueur de x case horizontalement et y cases verticalement ( si possible)
void move_up(player *joueur, map *carte); //Bouge le joueur vers le haut
void move_down(player *joueur, map *carte); //Bouge le joueur vers la gauche
void move_right(player *joueur, map *carte); //Bouge le joueur vers la droite
void move_left(player *joueur, map *carte); //Bouge le joueur vers la gauche
void freeline(line* ligne); //libère la ligne
void freeobstacle(obstacle* obs); //libère l'obstacle
void freemap(map* carte); //libère la carte
void map_shift(map* carte,player* joueur);//fait défiler la carte vers l'avant 
bool test_death(player *joueur,map* carte); //true->le joueur est mort.

