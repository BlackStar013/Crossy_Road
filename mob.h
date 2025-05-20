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
    int offset_x;
    int offset_y;             // pour une raison que j'ignore, il refuse d'avoir un autre champ ???
    void (*path)(struct _Mob*,Player*);
    int action_value;
    int action_delay;
    int max_action_delay;
    int memory__x;
    int memory__y;
} Mob;



enum MOB{
    CAR,
    BUS,
    PLANK,
    CANON_MOB,
    CANON_BALL,
    ////
    FIRE_MOB,
    MAGIC_CAR,
    BUSH,
    MOB_CULTIST,
    HIGHCULTIST,
    PEBBLE,
    BEBERT,
    SPIKE,
    SHOOT,
    BONE,
    EYE,
    EYEWALL,
    SHIFT,
    NB_MOB
};

enum PROJECTILE{
    CURSE,
    BONEARROW,
    BLOOD,
    EYE_BLAST,
    NB_PROJECTILE
};

typedef struct _List_Mob {
    Mob* mob;
    struct _List_Mob* next;
} Liste_Mob;

bool death_mob_collide(Mob*,Player*);
bool mob_collide(Mob*,Player*);

//On gère la vitesse à l'aide du nombre d'actualisation

bool mob_in_map(Mob*);
Mob* clone_mob(Mob,int,int,int,int);

void path_right(Mob*,Player*);
extern const Mob car_right;
extern const Mob plank_right;
extern const Mob bus_right;

void path_left(Mob*,Player*);
extern const Mob car_left;
extern const Mob plank_left;
extern const Mob bus_left;

void canon_idle(Mob*,Player*);

void canon_right_fast(Mob*,Player*);
extern const Mob canon_right;
extern const Mob canon_ball_right;

void canon_left_fast(Mob*,Player*);
extern const Mob canon_left;
extern const Mob canon_ball_left;




//Fonction liés à List_Mob
Liste_Mob* list_create(Mob* mob);
Liste_Mob *list_append(Liste_Mob* list, Mob* mob);
void print_liste(Liste_Mob* list);
void move_all_mob(Liste_Mob* list,Player*);
void switch_mob(Liste_Mob* list);
bool a_death_mob_collide(Liste_Mob* list,Player* player);
bool a_mob_collide(Liste_Mob* list,Player* player);
void destroy_mobs(Liste_Mob *liste);
void destroy_liste_mob(Liste_Mob *liste);
Liste_Mob* destroy_liste_mob_from(int y, Liste_Mob *liste);
Liste_Mob* destroy_liste_mob_collide(Liste_Mob *liste,Player* player);



///////
void clockwise_cycle(Mob *fire,Player* player);
extern const Mob fire_clock;
void counterclock_cycle(Mob *fire,Player* player);
extern const Mob fire_counter;
// void clockwise_cycle_3(Mob *fire,Player* player);
// extern const Mob fire_3_clock;
// void counterclock_cycle_3(Mob *fire,Player* player);
// extern const Mob fire_3_counter;

void ambush(Mob *bush,Player* player);
extern const Mob bush;

void patrol_clock4x3(Mob *fire, Player* player);
void patrol_clock5x3(Mob *fire, Player* player);
void patrol_clock4x4(Mob *fire, Player* player);
void patrol_counter4x3(Mob *fire, Player* player);
void patrol_counter5x3(Mob *fire, Player* player);
void patrol_counter4x4(Mob *fire, Player* player);
void babel1_1(Mob *mob,Player* player);
void babel2_1(Mob *mob,Player* player);
void babel3_1(Mob *mob,Player* player);
void babel4_1(Mob *mob,Player* player);
void babel1_2(Mob *mob,Player* player);
void babel2_2(Mob *mob,Player* player);
void babel3_2(Mob *mob,Player* player);
void babel4_2(Mob *mob,Player* player);

extern const Mob cult_clock1;
extern const Mob cult_clock2;
extern const Mob cult_clock3;
extern const Mob cult_counter1;
extern const Mob cult_counter2;
extern const Mob cult_counter3;

void still(Mob *mage,Player* player);
void follow(Mob *projectile,Player* player);

extern const Mob mage;
extern const Mob curse_arrow;
//extern const Mob curse_arrow_bis_1;
//extern const Mob curse_arrow_bis_2;
extern const Mob pebble_left;
extern const Mob pebble_right;

extern const Mob babel11;
extern const Mob babel12;
extern const Mob babel21;
extern const Mob babel22;
extern const Mob babel31;
extern const Mob babel32;
extern const Mob babel41;
extern const Mob babel42;

//
void path_hard_right(Mob *car, Player*p);
void path_hard_left(Mob *car, Player*p);
extern const Mob magic_car_right;
extern const Mob magic_car_left;

void path_still_right(Mob *car, Player*p);
void path_still_left(Mob *car, Player*p);
extern const Mob big_bus_right;
extern const Mob big_bus_left;

void path_offset_left(Mob* mob, Player*p);
void path_offset_right(Mob* mob, Player*p);
void path_offset_trigger_left(Mob* mob, Player*p);
void path_offset_trigger_right(Mob* mob, Player*p);
void spike_timer(Mob* mob, Player*p);
void spike_trigger(Mob* mob, Player*p);
void shooter_trigger_still_left(Mob* mob, Player*p);
void shooter_trigger_still_right(Mob* mob, Player*p);
void shooter_timer_still(Mob* mob, Player*p);
void still_eye(Mob* mob, Player*p);
void sight_blast(Mob* mob,Player*p);
void still_eyewall(Mob* mob,Player*p);
void dropping_path(Mob* mob,Player*p);

extern const Mob shooter;
extern const Mob shooter_trap_left;
extern const Mob shooter_trap_right;
extern const Mob spike;
extern const Mob spike_trap;
extern const Mob bone_arrow_left;
extern const Mob bone_arrow_right;
extern const Mob bone_arrow_left_trap;
extern const Mob bone_arrow_right_trap;
extern const Mob eye;
extern const Mob eye_wall;
extern const Mob eye_drop;
extern const Mob eye_blast;
const Mob shift_left;
const Mob shift_right;
const Mob bone_left;
const Mob bone_right;
