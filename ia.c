#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include "ia.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void eval(Game *game){
    // score[i]=? => 0=haut; 1=droite; 2=bas; 3=gauche; 4=attendre
    //free(game->joueur->score);
    //game->joueur->score = calloc(5, sizeof(int));
    game->joueur->score[0] = 10;
    game->joueur->score[2] = 3;
    game->joueur->score[4] = 1;
    if(game->joueur->_x_dest/32 >= LENGTH/2){
        game->joueur->score[1] = 4;
        game->joueur->score[3] = 5;
    }else{
        game->joueur->score[1] = 5;
        game->joueur->score[3] = 4;
    }

    Player *p2 = malloc(sizeof(Player));
    p2->_x = game->joueur->_x;
    p2->_y = game->joueur->_y;
    p2->_x_dest = game->joueur->_x_dest;
    p2->_y_dest = game->joueur->_y_dest;
    Liste_Mob *liste = game->mobs;
    int  gentil = 0;
    //test si avancer tue :
    p2->_y_dest += 32;
    if((IS_DANGER(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)) || IS_BLOCK(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)))){
        game->joueur->score[0] -= 10;
    }
    liste = game->mobs;
    while(!gentil && liste != NULL){
        if(((mob_collide(liste->mob, p2)) && !liste->mob->is_death)){
            gentil = 1;
        }else if((mob_collide(liste->mob, p2)) && liste->mob->is_death){
            gentil = -1;
        }
        liste = liste->next;
    }
    game->joueur->score[0] += gentil * 10;
    gentil = 0;
    p2->_y_dest -= 32;
    //test si reculer tue :
    p2->_y_dest -= 32;
    if((IS_DANGER(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)) || IS_BLOCK(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)))){
        game->joueur->score[2] -= 3;
    }
    liste = game->mobs;
    while(!gentil && liste != NULL){
        if(((mob_collide(liste->mob, p2)) && !liste->mob->is_death)){
            gentil = true;
        }else if((mob_collide(liste->mob, p2)) && liste->mob->is_death){
            gentil = -1;
        }
        liste = liste->next;
    }
    game->joueur->score[2] += gentil * 3;
    gentil = 0;
    p2->_y_dest += 32;
    //test si aller a gauche tue :
    p2->_x_dest -= 32;
    if((IS_DANGER(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)) || IS_BLOCK(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)))){
        game->joueur->score[3] -= 5;
    }
    liste = game->mobs;
    while(!gentil && liste != NULL){
        if(((mob_collide(liste->mob, p2)) && !liste->mob->is_death)){
            gentil = true;
        }else if((mob_collide(liste->mob, p2)) && liste->mob->is_death){
            gentil = -1;
        }
        liste = liste->next;
    }
    game->joueur->score[3] += gentil * 5;
    gentil = 0;
    p2->_x_dest += 32;
    //test si aller a droite tue :
    p2->_x_dest += 32;
    if((IS_DANGER(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)) || IS_BLOCK(tile_map(game->carte, p2->_x_dest/32, p2->_y_dest/32)))){
        game->joueur->score[1] -= 5;
    }
    liste = game->mobs;
    while(!gentil && liste != NULL){
        if(((mob_collide(liste->mob, p2)) && !liste->mob->is_death)){
            gentil = true;
        }else if((mob_collide(liste->mob, p2)) && liste->mob->is_death){
            gentil = -1;
        }
        liste = liste->next;
    }
    game->joueur->score[1] += gentil * 5;
    gentil = 0;
    p2->_x_dest -= 32;

    for(int i=0; i<5; i++){
        printf("directive :%d; score : %d\n", i, *(game->joueur->score+i));
    }
    printf("\n");
    free(p2); 
}
    // //premier test avec juste min/max à 1 case de distance : score[i]=? => 0=haut; 1=droite; 2=bas; 3=gauche; 4=attendre
    // // if(!game->joueur->delai){
    // //     game->joueur->delai--;
    // //}else{
    //     free(game->joueur->score);
    //     game->joueur->score = calloc(5, sizeof(int));
    //     if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1))){
    //         *(game->joueur->score) -= 999;
    //     }
    //     if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32 + 1, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+1) -= 999;
    //     }
    //     if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+2) -= 999;
    //     }
    //     if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+3) -= 999;
    //     }

    //     if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1))){
    //         *(game->joueur->score) += 5;
    //     }
    //     if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 + 1, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+1) += 3;
    //     }
    //     if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+2) += 1;
    //     }
    //     if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32 - 1, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+3) += 3;
    //     }
    //     if(!IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32))){
    //         *(game->joueur->score+4) += 2;
    //     }
    //     Liste_Mob *liste = game->mobs;
    //     int coef;
    //     //printf("%d %d\n", game->joueur->_x_dest/32, game->joueur->_y_dest/32);
    //     while(liste != NULL){
    //         //printf("xmob : %d ymob : %d\n", liste->mob->_x/32, liste->mob->_y/32);
    //         if(liste->mob->is_death){
    //             coef = 1;
    //         }else{
    //             coef = -500;
    //         }
    //         if((liste->mob->_y/32 == game->joueur->_y_dest/32) && (liste->mob->_x/32 <= game->joueur->_x_dest/32+1)  && (liste->mob->_x/32 >= game->joueur->_x_dest/32-1) 
    //         && (!(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1)) || (liste->mob->_y/32 == game->joueur->_y_dest/32+1) || !IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1))))){
    //             printf("cas 1\n");
    //             *(game->joueur->score+4) -= coef*10;
    //         }else if((liste->mob->_y/32 == game->joueur->_y_dest/32+1) && (liste->mob->_x/32 <= game->joueur->_x_dest/32+1) && (liste->mob->_x/32 >= game->joueur->_x_dest/32-1)){
    //             printf("cas 2\n");
    //             *(game->joueur->score+0) -= coef*3;
    //         }else if((liste->mob->_y/32 == game->joueur->_y_dest/32-1) && (liste->mob->_x/32 <= game->joueur->_x_dest/32+1) && (liste->mob->_x/32 >= game->joueur->_x_dest/32-1)
    //         && (!(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1)) || (liste->mob->_y/32 == game->joueur->_y_dest/32+1) || !IS_BLOCK(tile_map(game->carte, game->joueur->_x_dest/32, (game->joueur->_y_dest)/32 + 1))))){
    //             printf("cas 3\n");
    //             *(game->joueur->score+2) -= coef*5;
    //         }
    //         liste = liste->next;
    //     }
    // //}

int max_score(int *score){
    // renvoie la directive donc le score était maximale
    int max = -999999;
    int directive = -1;
    for(int i=0; i<5; i++){
        if(*(score+i) > max){
            max = *(score+i);
            directive = i;
        }
    }
    return directive;
}
