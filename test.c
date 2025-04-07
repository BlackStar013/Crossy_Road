#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tuiles.h"
#include "mob.h"
#include "map.h"
#include "core.h"

int main(void){
    printf("les tests sont lancés\n");
    assert(1==1);
    Liste_Mob *liste = list_create(clone_mob(car_right, 0, 3, 10));
    list_append(liste, clone_mob(car_right, 0, 0, 10));
    list_append(liste, clone_mob(car_right, 5, 5, 10));
    print_liste(liste);
    destroy_liste_mob(liste);
    printf("les tests sont passés\n");
    return 0;
}
