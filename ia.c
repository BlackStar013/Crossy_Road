#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include "ia.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


int val_absolue(int x){
    if(x<0){
        return -x;
    }
    return x;
}

File *create_file(Point *sommet){
    File *new_file = malloc(sizeof(File));
    new_file->next = NULL;
    new_file->sommet = sommet;
    return new_file;
}

void append_file(File *file, Point *sommet){
    if(file == NULL){
        file = create_file(sommet);
    }else{
        if(!(file->sommet->i == sommet->i && file->sommet->j == sommet->j)){
            if(file->next == NULL){
                file->next = create_file(sommet);
            }else{
                append_file(file->next, sommet);
            }
        }
    }
}

File *defile(File *file){
    if(file == NULL){
        return NULL;
    }
    File *temp = file->next;
    free(file);
    return temp;
}

void destroy_file(File *file){
    while(file!=NULL){
        File *mem = file->next;
        free(file);
        file = mem;
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

// void desmove_all_mob_IA(Liste_Mob *liste, Player *p2){
//     if(liste!=NULL){
//         for(int i=0; i<16; i++){
//             if((liste->mob->id_mob == CAR_LEFT) || (liste->mob->id_mob == PLANK_LEFT) || (liste->mob->id_mob == BUS_LEFT)){
//                 (path_right)(liste->mob, p2);
//             }else if((liste->mob->id_mob == CAR_RIGHT) || (liste->mob->id_mob == PLANK_RIGHT) || (liste->mob->id_mob == BUS_RIGHT)){
//                 (path_left)(liste->mob, p2);
//             }
//         }
//         desmove_all_mob_IA(liste->next, p2);
//     }
// }

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

int move_up_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(0, 1, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste)){
        return 2;   
    }
    return 0;
}
int move_down_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(0, -1, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste)){
        return 2;   
    }
    return 0;
}

int move_left_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(-1, 0, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste)){
        return 2;   
    }
    return 0;
}

int move_right_IA(Map *map, Player *p2, Liste_Mob *liste){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(1, 0, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste)){
        return 2;   
    }
    return 0;
}

Point *create_point(int i, int j, int val){
    Point *new_point = malloc(sizeof(Point));
    new_point->vu = false;
    new_point->val = val;
    new_point->i = i;
    new_point->j = j;
    new_point->chemin = calloc(PROFONDEUR, sizeof(int));
    new_point->chemin[0] = 4;new_point->chemin[1] = 4;
    return new_point;
}

Point ***matrice_evaluation(void){
    Point ***evaluation = malloc(PROFONDEUR*sizeof(Point));
    for(int i=0; i<PROFONDEUR; i++){
        evaluation[i] = malloc(PROFONDEUR*sizeof(Point));
        for(int j=0; j<PROFONDEUR; j++){
            evaluation[i][j] = create_point(-PROFONDEUR/2+j, PROFONDEUR/2-i, PROFONDEUR/2-i);
        }
    }
    evaluation[PROFONDEUR/2][PROFONDEUR/2]->val = -1;
    return evaluation;
}

void print_matrice(Point ***evaluation){
    for(int i=0; i<PROFONDEUR; i++){
        for(int j=0; j<PROFONDEUR; j++){
            // printf("(%d,%d) ", evaluation[i][j]->i, evaluation[i][j]->j);
            // printf("(%d,%d) ", evaluation[i][j]->chemin[0], evaluation[i][j]->chemin[1]);
            printf("%d ", evaluation[i][j]->val);
        }
        printf("\n");
    }
    printf("\n");
}

void destroy_matrice(Point ***matrice_eval){
    for(int i=0; i<PROFONDEUR; i++){
        for(int j=0; j<PROFONDEUR; j++){
            free(matrice_eval[i][j]->chemin);
            free(matrice_eval[i][j]);
        }
        free(matrice_eval[i]);
    }
    free(matrice_eval);
}

int eval(Game *game){
    // score[i]=? => 0=haut; 1=droite; 2=bas; 3=gauche; 4=attendre
    Liste_Mob *liste = clone_listemob(game->mobs);
    Point ***matrice_eval = matrice_evaluation();
    File *file = create_file(matrice_eval[PROFONDEUR/2][PROFONDEUR/2]);
    int n = 0;
    int o = 0;
    // int compteur = 0;
    while(file != NULL){
        // printf("%d %d\n", file->sommet->i, file->sommet->j);
        // compteur++;
        // printf("%d %d\n", file->sommet->i, file->sommet->j);
        Player *p2 = malloc(sizeof(Player));
        // printf("x :%d, y : %d\n", game->joueur->_x_dest/32, game->joueur->_y_dest/32);
        p2->_x = (game->joueur->_x)+32*file->sommet->i;
        p2->_y = (game->joueur->_y)+32*file->sommet->j;
        p2->_x_dest = (game->joueur->_x_dest)+32*file->sommet->i;
        p2->_y_dest = (game->joueur->_y_dest)+32*file->sommet->j;
        // printf("x : %d, y : %d\n", p2->_x_dest/32, p2->_y_dest/32);
        // printf("%d %d\n", file->sommet->i, file->sommet->j);
        matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+PROFONDEUR/2]->vu = true;
        if(val_absolue(file->sommet->i)+val_absolue(file->sommet->j) > n){
            n = val_absolue(file->sommet->i)+val_absolue(file->sommet->j);
            // printf("UPDATE\n");
            for(int k=0; k<10; k++){
                update_game_IA(liste, p2);
            }
        }
        o = val_absolue(file->sommet->i)+val_absolue(file->sommet->j);
        // printf("o : %d\n", o);
        int bouge;
        if((val_absolue(file->sommet->i)+val_absolue(file->sommet->j)) < PROFONDEUR/2){
            //essaye d'aller vers le bas
            if((file->sommet->j)+PROFONDEUR/2 > 0){
                bouge = move_down_IA(game->carte, p2, liste);
                if(!bouge){
                    if(!matrice_eval[PROFONDEUR/2-(file->sommet->j)+1][(file->sommet->i)+PROFONDEUR/2]->vu){
                        for(int d=0; d<o; d++){
                            matrice_eval[PROFONDEUR/2-(file->sommet->j)+1][(file->sommet->i)+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }
                        matrice_eval[PROFONDEUR/2-(file->sommet->j)+1][(file->sommet->i)+PROFONDEUR/2]->chemin[o] = 2;
                        append_file(file, matrice_eval[PROFONDEUR/2-(file->sommet->j)+1][(file->sommet->i)+PROFONDEUR/2]);
                    }
                    
                }else{ //if(bouge){
                    matrice_eval[PROFONDEUR/2-(file->sommet->j)+1][(file->sommet->i)+PROFONDEUR/2]->val = -99;
                }
                if(bouge != 1){
                    move_up_IA(game->carte, p2, liste);
                }
            }
            // essaye d'aller a gauche
            if((file->sommet->i)+PROFONDEUR/2 > 0){ 
                bouge = move_left_IA(game->carte, p2, liste);
                if(!bouge){
                    if(!matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)-1+PROFONDEUR/2]->vu){
                        for(int d = 0; d<o; d++){
                            matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)-1+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }  
                        matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)-1+PROFONDEUR/2]->chemin[o] = 3;
                        append_file(file, matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)-1+PROFONDEUR/2]);
                    }
                }else{ //if(bouge){
                    matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)-1+PROFONDEUR/2]->val = -99;
                }
                if(bouge != 1){
                    move_right_IA(game->carte, p2, liste);
                }
            }
            // essaye d'aller a droite
            if((file->sommet->i) < PROFONDEUR/2){
                bouge = move_right_IA(game->carte, p2, liste);
                if(!bouge){
                    if(!matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+1+PROFONDEUR/2]->vu){
                        for(int d=0; d<o; d++){
                            matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+1+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }
                        matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+1+PROFONDEUR/2]->chemin[o] = 1;
                        append_file(file, matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+1+PROFONDEUR/2]);
                    }
                }else{ //if(bouge){
                    matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+1+PROFONDEUR/2]->val = -99;
                }
                if(bouge != 1){
                    move_left_IA(game->carte, p2, liste);
                }
            }
            // essaye de monter
            if((file->sommet->j) < PROFONDEUR/2){
                bouge = move_up_IA(game->carte, p2, liste);
                if(!bouge){
                    if(!matrice_eval[PROFONDEUR/2-(file->sommet->j)-1][(file->sommet->i)+PROFONDEUR/2]->vu){
                        for(int d = 0; d<o; d++){
                            matrice_eval[PROFONDEUR/2-(file->sommet->j)-1][(file->sommet->i)+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(file->sommet->j)][(file->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }
                        matrice_eval[PROFONDEUR/2-(file->sommet->j)-1][(file->sommet->i)+PROFONDEUR/2]->chemin[o] = 0;
                        append_file(file, matrice_eval[PROFONDEUR/2-(file->sommet->j)-1][(file->sommet->i)+PROFONDEUR/2]);
                    }       
                }else{  //if(bouge){
                    matrice_eval[PROFONDEUR/2-(file->sommet->j)-1][(file->sommet->i)+PROFONDEUR/2]->val = -99;
                }
                if(bouge != 1){
                    move_down_IA(game->carte, p2, liste);
                }
            }
        }
        // printf("compteur : %d\n", compteur);
        // File *temp = file;
        // while(temp!=NULL){
        //     printf("%d %d\n", temp->sommet->i, temp->sommet->j);
        //     temp = temp->next;
        // }
        // printf("\n");
        free(p2);
        file = defile(file);
        // print_matrice(matrice_eval);

    }
    
    Point *max = NULL;
    int val_max = -99;
    for(int a=0; a<PROFONDEUR; a++){
        for(int b=0; b<PROFONDEUR; b++){
            // printf("max actuel : %d\n", max->val);
            if(matrice_eval[a][b]->vu){
                if(matrice_eval[a][b]->val > val_max){
                    //free(max->chemin);
                    //free(max);
                    max = matrice_eval[a][b];
                    val_max = matrice_eval[a][b]->val;
                }
            }else{
                matrice_eval[a][b]->val = -99;
            }
        }
    }
    // printf("point max %d %d\n", max->i, max->j);
    // int directive = 0;
    int directive = max->chemin[0];
    // printf("%d\n", directive);
    // print_matrice(matrice_eval);
    
    // free(max->chemin);
    // free(max);
    destroy_file(file);
    destroy_liste_mob(liste);
    destroy_matrice(matrice_eval);
    return directive;
}
