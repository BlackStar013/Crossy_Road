#pragma once
#include "tuiles.h"
#include "map.h"
#include <stdbool.h>

typedef struct _Mob {
    int heigth;
    int length;
    bool is_death;
    int move_x;
    int _x; //new x = ans x/32;
    int _y; //mismo por y;
    int speed; // ==Delay_max 
    int delay;
    int id_mob;
    void (*path)(struct _Mob*,Player*);
    int action_value;
    int action_delay;
    int memory__x;
    int memory__y;
} Mob;

enum MOB{
    CAR_RIGHT,
    CAR_LEFT,
    BUS_RIGHT,
    BUS_LEFT,
    PLANK_RIGHT,
    PLANK_LEFT,
    FIRE2CLOCK,
    FIRE2COUNTER,
    FIRE3CLOCK,
    FIRE3COUNTER,
    BUSH,
    CULTIST1CLOCK,
    CULTIST2CLOCK,
    CULTIST3CLOCK,
    CULTIST1COUNTER,
    CULTIST2COUNTER,
    CULTIST3COUNTER,
    HIGHCULTIST,
    CURSE,
    NB_MOB
};

typedef struct _List_Mob {
    Mob* mob;
    struct _List_Mob* next;
} Liste_Mob;

bool death_mob_collide(Mob*,Player*);
bool mob_collide(Mob*,Player*);

//On gère la vitesse à l'aide du nombre d'actualisation

bool mob_in_map(Mob*);
Mob* clone_mob(Mob,int,int,int);

void path_right(Mob*,Player*);
extern const Mob car_right;
extern const Mob plank_right;
extern const Mob bus_right;

void path_left(Mob*,Player*);
extern const Mob car_left;
extern const Mob plank_left;
extern const Mob bus_left;

//Fonction liés à List_Mob
Liste_Mob* list_create(Mob* mob);
void list_destroy(Liste_Mob* list);
bool list_is_empty(Liste_Mob* list);
Liste_Mob *list_append(Liste_Mob* list, Mob* mob);
void print_liste(Liste_Mob* list);
void move_all_mob(Liste_Mob* list,Player* Joueur);
void switch_mob(Liste_Mob* list);
bool a_death_mob_collide(Liste_Mob* list,Player* player);
bool a_mob_collide(Liste_Mob* list,Player* player);
void destroy_mobs(Liste_Mob *liste);
void destroy_liste_mob(Liste_Mob *liste);

void clockwise_cycle_2(Mob *fire,Player* player);
extern const Mob fire_2_clock;
void counterclock_cycle_2(Mob *fire,Player* player);
extern const Mob fire_2_counter;
void clockwise_cycle_3(Mob *fire,Player* player);
extern const Mob fire_3_clock;
void counterclock_cycle_3(Mob *fire,Player* player);
extern const Mob fire_3_counter;

void ambush(Mob *bush,Player* player);
extern const Mob bush;

void patrol_clock4x2(Mob *fire, Player* player);
void patrol_clock5x2(Mob *fire, Player* player);
void patrol_clock4x3(Mob *fire, Player* player);
void patrol_counter4x2(Mob *fire, Player* player);
void patrol_counter5x2(Mob *fire, Player* player);
void patrol_counter4x3(Mob *fire, Player* player);

extern const Mob cult_clock1;
extern const Mob cult_clock2;
extern const Mob cult_clock3;
extern const Mob cult_counter1;
extern const Mob cult_counter2;
extern const Mob cult_counter3;

void still(Mob *mage,Player* player);
void curse(Mob *projectile,Player* player);

extern const Mob mage;
extern const Mob curse_arrow;
