#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tuiles.h"
#include "mob.h"
#include "map.h"

int main(void){
    printf("les tests sont lancés\n");
    assert(1==1);
    Tile *dest = malloc(sizeof(Tile));
    dest->type='_';
    printf("avant : %c\n", dest->type);
    clone(dest, water);
    printf("apres : %d %d %c %c\n", dest->is_block, dest->is_danger, dest->color, dest->type);
    free(dest);
    printf("les tests sont passés\n");
    return 0;
}
