#pragma once
#include "mob.h"
#include "tuiles.h"
#include "map.h"
#include <stdbool.h>
#include "map.h"



typedef struct _Game {
    Map* carte;
    Player* joueur;
    Liste_Mob* exp; //Est display *avant* mobs
    Liste_Mob* mobs;
    Liste_Mob* projectile; //Est display *après* mobs
    int score;
    int nb_exp;
    int* proba;
    void (*generator)(struct _Game*);
    int biome;
    int until_next;
} Game;



enum Biome {
    CROSSY_ROAD,
    HARD_CROSSY_ROAD,
    FLAMING_MANOR,
    CULTIST_CAVE,
    WITHIN_THE_BEAST,
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
    FAST_RIVER,
    BLOCKED_CANON,
    MAGIC_ROAD,
    ////////
    ENTRANCE,
    FIRELINE,
    FIRE_PATROL,
    FIRE,
    ////////
    TRANSITION_CAVE,
    //CAVE,
    CULTIST,
    MAGE,
    BABEL,
    TRAP,
    //STONE,
    //CAVE_CANON,
    //////
    TRANSITION_FINAL,
    ACID_RIVER,
    FLESH_MAW,
    FLESH_SPIKES,
    BONESHOOTER,
    BARRAGE,
    SENTINEL_EYE,
    LABYRINTH,
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
void generator_fast_river(Game*);
void generator_blocked_canon(Game*);
void generator_magic_road(Game*);
void generator_entrance(Game*);
void generator_fire(Game*);
void generator_fireline(Game *game);
void generator_fire_patrol(Game *game);
void generator_fire_patrol_1(Game *game);
void generator_fire_patrol_2(Game *game);
void generator_transition_cave(Game * game);
void generator_cave(Game*);
void generator_cultist(Game*);
void generator_mage(Game*);
void generator_trap(Game*);
void generator_babel(Game*);
void generator_transition_heart(Game*);
void generator_f_maw(Game*);
void generator_f_spike(Game*);
void generator_boneshooter(Game*);
void generator_acid_river(Game*);
void generator_barrage(Game*);
void generator_sentinel_eye(Game*);
void generator_labyrinth(Game*);
void generator_obstacle(Game*);

//Initialise les générateurs
void (*initialise_generator(void))(Game*);


//Initialise une partie
Game *initialise_game(int character_id, int tot_exp);
void update_game(Game* game);

void switch_game(Game *game);

//Ajoute aléatoirement un obstacle dans le buffer
void add_obstacle(Game*);

//Fait déplacer le joueur de x case à droite (positif) et y cases en haut.
bool move_player(int x, int y,Game*);
bool move_up(Game*);
bool move_left(Game*);
bool move_right(Game*);
bool move_down(Game*);
bool use_power(Game*);

//Fait déplacer la giga mab et les mobs dedans et les joueurs dedans.
bool test_death(Game*);

void destroy_game(Game *game);

