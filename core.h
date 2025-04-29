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
    int biome;
    int until_next;
} Game;



enum Biome {
    CROSSY_ROAD,
    FLAMING_MANOR,
    CULTIST_CAVE,
    NB_BIOME
};
//Les séparateurs correspondent aux Biomes.
enum Obstacle {
    FOREST,
    ROAD,
    RIVER,
    CANON,
    /////////
    BUSHES,
    ////////
    ENTRANCE,
    FIRE,
    ////////
    TRANSITION_CAVE,
    CAVE,
    CULTIST,
    MAGE,
    //CAVE_CANON,
    NB_OBSTACLE
};

//Initialises les probas
int* initialise_proba(void);

//Fonctions de genrations d'obastcales
void generator_forest(Game*);
void generator_road(Game*);
void generator_river(Game*);
void generator_canon(Game*);
void generator_bushes(Game*);
void generator_entrance(Game*);
void generator_fire(Game*);
void generator_transition_cave(Game * game);
void generator_cave(Game*);
void generator_cultist(Game*);
void generator_mage(Game*);
void generator_obstacle(Game*);

//Initialise les générateurs
void (*initialise_generator(void))(Game*);


//Initialise une partie
Game *initialise_game(int character_id);
void update_game(Game* game);

void switch_game(Game *game);

//Ajoute aléatoirement un obstacle dans le buffer
void add_obstacle(Game*);

//Fait déplacer le joueur de x case à droite (positif) et y cases en haut.
bool move(int x, int y,Game*);
bool move_up(Game*);
bool move_left(Game*);
bool move_right(Game*);
bool move_down(Game*);

//Fait déplacer la giga mab et les mobs dedans et les joueurs dedans.
bool test_death(Game*);

void destroy_game(Game *game);

