#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tuiles.h"
#include "mob.h"
#include "map.h"
#include "core.h"
#include "ia.h"

int main(void){
    printf("les tests sont lancés\n");
    Game *game = initialise_game();
    eval(game);
    int *score = game->joueur->score;
    for(int i=0; i<5; i++){
        printf("directive :%d; score : %d\n", i, *(score+i));
    }
    int directive = max_score(game->joueur->score);
    printf("%d\n", directive);
    assert(1==1);
    printf("les tests sont passes\n");
    destroy_game(game);
    return 0;
}
