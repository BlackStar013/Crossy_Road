#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tuiles.h"
#include "mob.h"
#include "map.h"
#include "core.h"

int main(void){
    printf("les tests sont lancés\n");
    Liste_Mob *l=list_create(clone_mob(car_right,1,1,1,1));
    l=list_append(l,clone_mob(car_left,1,1,1,1));
    Player* p=initialise_player(3,0);
    move_all_mob(l,p);
    move_all_mob(l,p);
    assert(l->mob->_x==35);
    assert(l->next->mob->_x==29);
    assert(l->mob->_y==32);
    assert(l->next->mob->_y==32);
    switch_mob(l);
    assert(l->mob->_x==35);
    assert(l->next->mob->_x==29);
    assert(l->mob->_y==0);
    assert(l->next->mob->_y==0);
    destroy_liste_mob(l);
    Map *m=create_empty_map();
    assert(carte_vide(m));
    destroy_map(m);
    free(p);
    printf("les tests sont passés\n");
    return 0;
}
