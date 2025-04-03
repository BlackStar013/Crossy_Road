#pragma once
#include "mob.h"
#include "tuiles.h"
#include "map.h"
#include <stdbool.h>
#include "map.h"



typedef struct _Game {
    Map* carte;
    Player* joueur;
    Liste_Mob* mobs;
    int score;
    int* proba;
    void (*generator)(struct _Game*);
} Game;

enum Obstacle {
    FOREST,
    ROAD,
    RIVER,
    NB_OBSTACLE
};

//Initialises les probas
int* initialise_proba(void);

//Fonctions de genrations d'obastcales
void (generator_forest)(Game*);
void (generator_road)(Game*);
void (generator_river)(Game*);

//Initialise les générateurs
void (*initialise_generator(void))(Game*);


//Initialise une partie
Game *initalise_game(void);

//Ajoute aléatoirement un obstacle dans le buffer
void add_obstacle(Game*);

//Fait déplacer le joueur de x case à droite (positif) et y cases en haut.
void move(int x, int y,Game*);
void move_up(Game*);
void move_left(Game*);
void move_right(Game*);
void move_down(Game*);

//Fait déplacer la giga mab et les mobs dedans et les joueurs dedans.
void move_game(Game*);
void destroy_game(Game *game);
