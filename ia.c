#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include "ia.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


bool move_IA(int x, int y, Player *joueur, Map *map){
    //printf("(%d,%d)\n",game->joueur->_x_dest/32,game->joueur->_y_dest/32);
    //printf("%d\n",tile_map(game->carte, game->joueur->_x_dest/32 + x, (game->joueur->_y_dest)/32 + y).type);
    if(tile_map(map, joueur->_x_dest/32 + x, (joueur->_y_dest)/32 + y).type != water.type){
        if(joueur->_x_dest%32 > 16){
            joueur->_x_dest =  (joueur->_x_dest/32+1)*32;
        } else {
            joueur->_x_dest = (joueur->_x_dest/32)*32;
        }
        if(joueur->_y_dest%32 > 16){
            joueur->_y_dest =  (joueur->_y_dest/32+1)*32 ;
        } else {
            joueur->_y_dest =  (joueur->_y_dest/32)*32 ;
        }
    }
    if(!IS_BLOCK(tile_map(map, joueur->_x_dest/32 + x, (joueur->_y_dest)/32 + y))){
        joueur->_x_dest +=  x*32;
        joueur->_y_dest +=  y*32;
        return true ;
    } else {
        return false ;
    }
}

bool test_death_IA(Player *joueur, Map *map, Liste_Mob *liste){
    if(a_death_mob_collide(liste, joueur)){
        return true;
    }else if(a_mob_collide(liste, joueur)){
        return false;
    }else{
        return IS_DANGER(tile_map(map, joueur->_x_dest/32, joueur->_y_dest/32));
    }
}

Mob *clone_mob_IA(Mob *mob){
    // creer un mob selon le mob rentré
    Mob *new_mob = malloc(sizeof(Mob));
    new_mob->heigth = mob->heigth;
    new_mob->length = mob->length;
    new_mob->is_death = mob->is_death;
    new_mob->move_x = mob->move_x;
    new_mob->_x = mob->_x;
    new_mob->_y = mob->_y;
    new_mob->speed = mob->speed;
    new_mob->delay = mob->speed;
    new_mob->id_mob = mob->id_mob;
    new_mob->offset_x = mob->offset_x;
    //new_mob->offset_y = mob.offset_y;
    new_mob->path = mob->path;
    new_mob->action_delay=mob->action_delay;
    new_mob->action_value=mob->action_value;
    new_mob->memory__x = mob->memory__y;;
    new_mob->memory__y = mob->memory__y;
    return new_mob;
}

Liste_Mob *clone_listemob(Liste_Mob *liste){
    Liste_Mob *clone = NULL;
    while(liste != NULL){
        clone = list_append(clone, clone_mob_IA(liste->mob));
        liste = liste->next;
    }
    return clone;
}

void update_game_IA(Liste_Mob *liste, Player *joueur){
    Liste_Mob* cur = liste;
    while(cur!=NULL){
        if(cur->mob->move_x){
            if(mob_collide(cur->mob, joueur)){
                if(cur->mob->delay==0){
                    joueur->_x += cur->mob->move_x;
                    joueur->_x_dest += cur->mob->move_x;
                }
            }
        }
        cur = cur->next;
    }
    move_all_mob(liste, joueur);
    joueur->_x = (joueur->_x + joueur->_x_dest)/2;
    joueur->_y = (joueur->_y + joueur->_y_dest)/2;
}

int move_up_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(0, 1, p2, map)){
        return 1;
    }else{
        for(int i=0; i<16; i++){
            // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
            if(test_death_IA(p2, map, liste)){
                return 2;
            }else{
                update_game_IA(liste, p2);
            }
        }
    }
    return 0;
}

int move_down_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(0, -1, p2, map)){
        return 1;
    }else{
        for(int i=0; i<16; i++){
            // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
            if(test_death_IA(p2, map, liste)){
                return 2;
            }else{
                update_game_IA(liste, p2);
            }
        }
    }
    return 0;
}

int move_left_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(-1, 0, p2, map)){
        return 1;
    }else{
        for(int i=0; i<16; i++){
            // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
            if(test_death_IA(p2, map, liste)){
                return 2;
            }else{
                update_game_IA(liste, p2);
            }
        }
    }
    return 0;
}

int move_right_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(1, 0, p2, map)){
        return 1;
    }else{
        for(int i=0; i<16; i++){
            // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
            if(test_death_IA(p2, map, liste)){
                return 2;
            }else{
                update_game_IA(liste, p2);
            }
        }
    }
    return 0;
}

int *score_temp(Game *game){
    int *temp = calloc(5, sizeof(int));
    temp[0] = 10;
    temp[2] = 3;
    if(IS_DANGER(tile_map(game->carte, game->joueur->_x_dest/32, game->joueur->_y_dest/32))){
        temp[4] = 6;
    }else{
        temp[4] = 1;
    }
    if(game->joueur->_x_dest/32 >= LENGTH/2){
        temp[1] = 4;
        temp[3] = 5;
    }else{
        temp[1] = 5;
        temp[3] = 4;
    }
    return temp;
}

bool eval(Game *game){
    // score[i]=? => 0=haut; 1=droite; 2=bas; 3=gauche; 4=attendre
    Player *p2 = malloc(sizeof(Player));
    p2->_x = game->joueur->_x;
    p2->_y = game->joueur->_y;
    p2->_x_dest = game->joueur->_x_dest;
    p2->_y_dest = game->joueur->_y_dest;
    Liste_Mob *liste = clone_listemob(game->mobs);
    int *temp1 = NULL;
    int *temp2 = NULL;
    int *temp3 = NULL;
    int bouge1;
    int bouge2;
    int bouge3;
    //bouge vaut 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    //choix du premier mouvement haut
    temp1 = score_temp(game);
    bouge1 = move_up_IA(game->carte, p2, liste);
    if(!bouge1){
        //choix du deuxieme mouvement haut
        temp2 = score_temp(game);
        bouge2 = move_up_IA(game->carte, p2, liste);
        if(!bouge2){
            //choix du troisième mouvement haut
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement droite
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            //choix du troisieme mouvement gauche
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement bas
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[0] = 0;
            if(bouge2 == 2){
                move_down_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement droite
        bouge2 = move_right_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }

            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[1] = 0;
            if(bouge2 == 2){
                move_left_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement gauche
        bouge2 = move_left_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[3] = 0;
            if(bouge2 == 2){
                move_right_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
        
        //choix du deuxieme mouvement bas
        bouge2 = move_down_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[2] = 0;
            if(bouge2 == 2){
                move_up_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
    }else{
        free(temp2);
        temp1[0] = 0;
        if(bouge1 == 2){
            move_down_IA(game->carte, p2, liste);
        }
        // desupdategame !!!
    }

    //choix du premier mouvement droite
    bouge1 = move_right_IA(game->carte, p2, liste);
    if(!bouge1){
        temp2 = score_temp(game);
        //choix du deuxieme mouvement haut
        bouge2 = move_up_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            //choix du troisième mouvement haut
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement droite
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            //choix du troisieme mouvement gauche
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement bas
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[0] = 0;
            if(bouge2 == 2){
                move_down_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement droite
        bouge2 = move_right_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }

            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[1] = 0;
            if(bouge2 == 2){
                move_left_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement gauche
        bouge2 = move_left_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[3] = 0;
            if(bouge2 == 2){
                move_right_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
        
        //choix du deuxieme mouvement bas
        bouge2 = move_down_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[2] = 0;
            if(bouge2 == 2){
                move_up_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
    }else{
        free(temp2);
        temp1[1] = 0;
        if(bouge1 == 2){
            move_left_IA(game->carte, p2, liste);
        }
        // desupdategame !!!
    }

    //choix du premier mouvement gauche
    bouge1 = move_left_IA(game->carte, p2, liste);
    if(!bouge1){
        temp2 = score_temp(game);
        //choix du deuxieme mouvement haut
        bouge2 = move_up_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            //choix du troisième mouvement haut
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement droite
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            //choix du troisieme mouvement gauche
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement bas
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[0] = 0;
            if(bouge2 == 2){
                move_down_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement droite
        bouge2 = move_right_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }

            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[1] = 0;
            if(bouge2 == 2){
                move_left_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement gauche
        bouge2 = move_left_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[3] = 0;
            if(bouge2 == 2){
                move_right_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
        
        //choix du deuxieme mouvement bas
        bouge2 = move_down_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[2] = 0;
            if(bouge2 == 2){
                move_up_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
    }else{
        free(temp2);
        temp1[3] = 0;
        if(bouge1 == 2){
            move_right_IA(game->carte, p2, liste);
        }
        // desupdategame !!!
    }

    //choix du premier mouvement bas
    bouge1 = move_down_IA(game->carte, p2, liste);
    if(!bouge1){
        temp2 = score_temp(game);
        //choix du deuxieme mouvement haut
        bouge2 = move_up_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            //choix du troisième mouvement haut
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement droite
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            //choix du troisieme mouvement gauche
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            //choix du troisieme mouvement bas
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[0] = 0;
            if(bouge2 == 2){
                move_down_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement droite
        bouge2 = move_right_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }

            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!

            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }

        }else{
            free(temp3);
            temp2[1] = 0;
            if(bouge2 == 2){
                move_left_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }

        //choix du deuxieme mouvement gauche
        bouge2 = move_left_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[3] = 0;
            if(bouge2 == 2){
                move_right_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
        
        //choix du deuxieme mouvement bas
        bouge2 = move_down_IA(game->carte, p2, liste);
        if(!bouge2){
            temp3 = score_temp(game);
            bouge3 = move_up_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[0] = 0;
                if(bouge3 == 2){
                    move_down_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
        
            bouge3 = move_right_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[1] = 0;
                if(bouge3 == 2){
                    move_left_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            
            }
            bouge3 = move_left_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[3] = 0;
                if(bouge3 == 2){
                    move_right_IA(game->carte, p2, liste);
                }
                // desupdategame !!!
            }
            bouge3 = move_down_IA(game->carte, p2, liste);
            if(!bouge3){
                game->joueur->score[0] = max_score(temp1);
                game->joueur->score[1] = max_score(temp2);
                game->joueur->score[2] = max_score(temp3);
                free(temp1);
                free(temp2);
                free(temp3);
                free(p2);
                destroy_liste_mob(liste);
                return true;
            }else{
                temp3[2] = 0;
                if(bouge3 == 2){
                    move_up_IA(game->carte, p2, liste);
                }
                //desupdategame !!!
            }
        
        }else{
            free(temp3);
            temp2[2] = 0;
            if(bouge2 == 2){
                move_up_IA(game->carte, p2, liste);
            }
            // desupdategame !!!
        }
    }else{
        free(temp2);
        temp1[2] = 0;
        if(bouge1 == 2){
            move_up_IA(game->carte, p2, liste);
        }
        // desupdategame !!!
    }
    game->joueur->score[0] = max_score(temp1);
    game->joueur->score[1] = max_score(temp2);
    game->joueur->score[2] = max_score(temp3);
    free(temp1);
    // free(temp2);
    // free(temp3);
    free(p2);
    destroy_liste_mob(liste);
    return true;
}

    //test pour avancer 1 fois:
    // bouge1 = move_IA(0, 1, p2, game->carte);
    // if(!bouge1){
    //     game->joueur->score[0] = 0;
    // }else{
    //     liste = clone_listemob(game->mobs);
    //     for(int i=0; i<16; i++){
    //         // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
    //         if(test_death_IA(p2, game->carte, liste)){
    //             game->joueur->score[0] = 0;
    //         }else{
    //             update_game_IA(liste, p2);
    //         }
    //     }
    //     destroy_liste_mob(liste);
    // }
    // if(bouge1){
    //     move_IA(0, -1, p2, game->carte);
    // }
    // //test de reculer :
    // bouge1 = move_IA(0, -1, p2, game->carte);
    // if(!bouge1){
    //     game->joueur->score[2] = 0;
    // }else{
    //     liste = clone_listemob(game->mobs);
    //     for(int i=0; i<16; i++){
    //         // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
    //         if(test_death_IA(p2, game->carte, liste)){
    //             game->joueur->score[2] = 0;
    //         }else{
    //             update_game_IA(liste, p2);
    //         }
    //     }   
    //     destroy_liste_mob(liste);
    // }
    // if(bouge1){
    //     move_IA(0, 1, p2, game->carte);
    // }
    // //test d'aller a gauche :
    // bouge1 = move_IA(-1, 0, p2, game->carte);
    // if(!bouge1){
    //     game->joueur->score[3] = 0;
    // }else{
    //     liste = clone_listemob(game->mobs);
    //     for(int i=0; i<16; i++){
    //         // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
    //         if(test_death_IA(p2, game->carte, liste)){
    //             game->joueur->score[3] = 0;
    //         }else{
    //             update_game_IA(liste, p2);
    //         }
    //     }
    //     destroy_liste_mob(liste);
    // }
    // if(bouge1){
    //     move_IA(1, 0, p2, game->carte);
    // }
    // //test d'aller a droite :
    // bouge1 = move_IA(1, 0, p2, game->carte);
    // if(!bouge1){
    //     game->joueur->score[1] = 0;
    // }else{
    //     liste = clone_listemob(game->mobs);
    //     for(int i=0; i<16; i++){
    //         // printf("i :%d  mort? %d\n", i, test_death_IA(p2, game->carte, liste));
    //         if(test_death_IA(p2, game->carte, liste)){
    //             game->joueur->score[1] = 0;
    //         }else{
    //             update_game_IA(liste, p2);
    //         }
    //     }
    //     destroy_liste_mob(liste);
    // }
    // if(bouge1){
    //     move_IA(-1, 0, p2, game->carte);
    // }

    // for(int i=0; i<5; i++){
    //     printf("directive :%d; score : %d\n", i, *(game->joueur->score+i));
    // }
    // printf("\n");
    // free(p2);
// }

int max_score(int *score){
    if(score == NULL){
        return 4;
    }
    printf("%d %d %d %d %d\n", score[0], score[1], score[2], score[3], score[4]);
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
