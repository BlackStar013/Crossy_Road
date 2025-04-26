#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include "ia.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void eval(Game *game){
    //premier test avec juste min/max à 1 case de distance : score[i]=? => 0=haut; 1=droite; 2=bas; 3=gauche; 4=attendre
    // if(!game->joueur->delai){
    //     game->joueur->delai--;
    //}else{
        free(game->joueur->score);
        game->joueur->score = calloc(5, sizeof(int));
        if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1))){
            *(game->joueur->score) -= 999;
        }
        if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32 + 1, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+1) -= 999;
        }
        if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+2) -= 999;
        }
        if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+3) -= 999;
        }

        if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1))){
            *(game->joueur->score) += 5;
        }
        if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 + 1, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+1) += 3;
        }
        if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+2) += 1;
        }
        if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+3) += 3;
        }
        if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32))){
            *(game->joueur->score+4) += 2;
        }
        Liste_Mob *liste = game->mobs;
        int coef;
        while(liste != NULL){
            if(liste->mob->is_death){
                coef = 1;
            }else{
                coef = -500;
            }
            if((liste->mob->_y == game->joueur->_y) && (liste->mob->_x <= game->joueur->_x+1  || liste->mob->_x >= game->joueur->_x-1)){
                *(game->joueur->score+4) -= coef*10;
            }else if((liste->mob->_y/32 == game->joueur->_y+2) && (liste->mob->_x <= game->joueur->_x+1 && liste->mob->_x >= game->joueur->_x-1)){
                *(game->joueur->score+0) -= coef*3;
            }else if((liste->mob->_y/32 == game->joueur->_y) && (liste->mob->_x <= game->joueur->_x+1 && liste->mob->_x >= game->joueur->_x-1)){
                *(game->joueur->score+2) -= coef*5;
            }
            liste = liste->next;
        }
    //}
}

int max_score(int *score){
    // renvoie la directive donc le score était maximale
    int max = -999;
    int directive = -1;
    for(int i=0; i<5; i++){
        if(*(score+i) > max){
            max = *(score+i);
            directive = i;
        }
    }
    return directive;
}
