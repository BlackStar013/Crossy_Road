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

bool test_death_IA(Player *joueur, Map *map, Liste_Mob *liste, Liste_Mob *projectiles){
    if(a_death_mob_collide(liste, joueur)){
        return true;
    }else if(a_death_mob_collide(projectiles, joueur)){
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

void update_game_IA(Liste_Mob *liste, Liste_Mob *projectiles, Player *joueur){
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
    move_all_mob(projectiles, joueur);
    joueur->_x = (joueur->_x + joueur->_x_dest)/2;
    joueur->_y = (joueur->_y + joueur->_y_dest)/2;
}

bool move_IA(int x, int y, Player *joueur, Map *map){
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

int move_up_IA(Map *map, Player *p2, Liste_Mob *liste, Liste_Mob *projectiles){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(0, 1, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste, projectiles)){
        return 2;   
    }
    return 0;
}
int move_down_IA(Map *map, Player *p2, Liste_Mob *liste, Liste_Mob *projectiles){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(0, -1, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste, projectiles)){
        return 2;   
    }
    return 0;
}

int move_left_IA(Map *map, Player *p2, Liste_Mob *liste, Liste_Mob *projectiles){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(-1, 0, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste, projectiles)){
        return 2;   
    }
    return 0;
}

int move_right_IA(Map *map, Player *p2, Liste_Mob *liste, Liste_Mob *projectiles){
    //renvoie 0 si il a pu bouger sans mourir, 1 si il n'a pas pu bouger, et 2 si il en meurt
    if(!move_IA(1, 0, p2, map)){
        return 1;
    }
    if(test_death_IA(p2, map, liste, projectiles)){
        return 2;   
    }
    return 0;
}

Point *create_point(int i, int j, int val){
    Point *new_point = malloc(sizeof(Point));
    new_point->vu = 99;
    new_point->val = val;
    new_point->i = i;
    new_point->j = j;
    new_point->chemin = calloc(PROFONDEUR, sizeof(int));
    for(int i=0; i<PROFONDEUR; i++){
        new_point->chemin[i] = 4;
    }
    return new_point;
}

Point ***matrice_evaluation(void){
    Point ***evaluation = malloc(PROFONDEUR*sizeof(Point));
    for(int i=0; i<PROFONDEUR; i++){
        int coef;
        if(i <= PROFONDEUR/2){
            coef = 1;
        }else{
            coef = -1;
        }
        evaluation[i] = malloc(PROFONDEUR*sizeof(Point));
        for(int j=0; j<PROFONDEUR; j++){
            evaluation[i][j] = create_point(-PROFONDEUR/2+j, PROFONDEUR/2-i, coef*(1 + val_absolue(PROFONDEUR/2-i) + val_absolue(PROFONDEUR/2-j)) );
            // evaluation[i][j] = create_point(-PROFONDEUR/2+j, PROFONDEUR/2-i, (1 + val_absolue(PROFONDEUR/2-i) + val_absolue(PROFONDEUR/2-j)) );
        }
    }
    evaluation[0][PROFONDEUR/2]->val += 1;
    return evaluation;
}

void print_matrice(Point ***evaluation){
    for(int i=0; i<PROFONDEUR; i++){
        for(int j=0; j<PROFONDEUR; j++){
            // printf("(%d,%d) ", evaluation[i][j]->i, evaluation[i][j]->j);
            // printf("(%d,%d, %d) ", evaluation[i][j]->chemin[0], evaluation[i][j]->chemin[1], evaluation[i][j]->chemin[2]);
            printf("%d ", evaluation[i][j]->val);
            // printf("%d ", evaluation[i][j]->vu);
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
    Point ***matrice_eval = matrice_evaluation();
    File *encours = create_file(matrice_eval[PROFONDEUR/2][PROFONDEUR/2]);
    matrice_eval[PROFONDEUR/2][PROFONDEUR/2]->vu = 0;
    File *futur = NULL;
    if(game->joueur->_x_dest/32 >= LENGTH/2){
        matrice_eval[PROFONDEUR/2][0]->val += 1;
        for(int i=0; i<PROFONDEUR/2; i++){
            for(int j=0; j<PROFONDEUR; j++){
                matrice_eval[i][j]->val += val_absolue((matrice_eval[i][j]->val)-((matrice_eval[PROFONDEUR/2][0]->val)));
            }
        }
    }else{
        matrice_eval[PROFONDEUR/2][PROFONDEUR-1]->val += 1;
        for(int i=0; i<PROFONDEUR/2; i++){
            for(int j=0; j<PROFONDEUR; j++){
                matrice_eval[i][j]->val += val_absolue((matrice_eval[i][j]->val)-((matrice_eval[PROFONDEUR/2][PROFONDEUR-1]->val)));
            }
        }
    }
    int n = 0;
    while(n < PROFONDEUR/2){
        while(encours != NULL){
            Player *p2 = malloc(sizeof(Player));
            p2->_x_dest = game->joueur->_x_dest;
            p2->_y_dest = game->joueur->_y_dest;
            Liste_Mob *liste = clone_listemob(game->mobs);
            Liste_Mob *projectiles = clone_listemob(game->projectile);
            for(int k=0; k<n; k++){
                if(matrice_eval[PROFONDEUR/2-encours->sommet->j][PROFONDEUR/2+encours->sommet->i]->chemin[k] == 0){
                    p2->_y_dest += 32;
                    for(int l=0; l<DELAI+1; l++){
                        update_game_IA(liste, projectiles, p2);
                    }
                }else if(matrice_eval[PROFONDEUR/2-encours->sommet->j][PROFONDEUR/2+encours->sommet->i]->chemin[k] == 1){
                    p2->_x_dest += 32;
                    for(int l=0; l<DELAI+1; l++){
                        update_game_IA(liste, projectiles, p2);
                    }
                }else if(matrice_eval[PROFONDEUR/2-encours->sommet->j][PROFONDEUR/2+encours->sommet->i]->chemin[k] == 2){
                    p2->_y_dest -= 32;
                    for(int l=0; l<DELAI+1; l++){
                        update_game_IA(liste, projectiles, p2);
                    }
                }else if(matrice_eval[PROFONDEUR/2-encours->sommet->j][PROFONDEUR/2+encours->sommet->i]->chemin[k] == 3){
                    p2->_x_dest -= 32;
                    for(int l=0; l<DELAI+1; l++){
                        update_game_IA(liste, projectiles, p2);
                    }
                }
            }

            bool bouge = true;
            p2->_x_dest = (game->joueur->_x_dest)+32*encours->sommet->i;
            p2->_y_dest = (game->joueur->_y_dest)+32*encours->sommet->j;
            // essaye de monter
            if((!IS_BLOCK(tile_map(game->carte, (p2->_x_dest/32), (p2->_y_dest)/32 + 1))) && ((encours->sommet->j) < PROFONDEUR/2)){
                move_up_IA(game->carte, p2, liste, projectiles);
                Liste_Mob *liste_temp = clone_listemob(liste);
                Liste_Mob *projectiles_temp = clone_listemob(projectiles);
                for(int k=0; k<DELAI+5; k++){
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                    update_game_IA(liste_temp, projectiles_temp, p2);
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                }
                if(bouge){
                    if(n < matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]->vu){
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]->vu = n;
                        for(int d = 0; d<n; d++){
                            matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]->chemin[n] = 0;
                        if(futur == NULL){
                            futur = create_file(matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]);
                        }else{
                            append_file(futur, matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]);
                        }
                    }
                }else{  //if(bouge){
                    matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]->val = -99;
                }
                destroy_liste_mob(liste_temp);
                destroy_liste_mob(projectiles_temp);
            }else{
                matrice_eval[PROFONDEUR/2-(encours->sommet->j)-1][(encours->sommet->i)+PROFONDEUR/2]->val = -99;
            }
            bouge = true;
            p2->_x_dest = (game->joueur->_x_dest)+32*encours->sommet->i;
            p2->_y_dest = (game->joueur->_y_dest)+32*encours->sommet->j;
            // essaye d'aller a gauche
            if((!IS_BLOCK(tile_map(game->carte, p2->_x_dest/32 - 1, (p2->_y_dest)/32))) && (encours->sommet->i)+PROFONDEUR/2 > 0){
                move_left_IA(game->carte, p2, liste, projectiles);
                Liste_Mob *liste_temp = clone_listemob(liste);
                Liste_Mob *projectiles_temp = clone_listemob(projectiles);
                for(int k=0; k<DELAI+5; k++){
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                    update_game_IA(liste_temp, projectiles_temp, p2);
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                }
                if(bouge){
                    if(n < matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]->vu){
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]->vu = n;
                        for(int d = 0; d<n; d++){
                            matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }  
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]->chemin[n] = 3;
                        if(futur == NULL){
                            futur = create_file(matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]);
                        }else{
                            append_file(futur, matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]);
                        }
                    }
                }else{ //if(bouge){
                    matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]->val = -99;
                }
                destroy_liste_mob(liste_temp);
                destroy_liste_mob(projectiles_temp);
            }else{
                matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)-1+PROFONDEUR/2]->val = -99;
            }
            bouge = true;
            p2->_x_dest = (game->joueur->_x_dest)+32*encours->sommet->i;
            p2->_y_dest = (game->joueur->_y_dest)+32*encours->sommet->j;
            if((!IS_BLOCK(tile_map(game->carte, p2->_x_dest/32 + 1, (p2->_y_dest)/32))) && ((encours->sommet->i) < PROFONDEUR/2)){
                move_right_IA(game->carte, p2, liste, projectiles);
                Liste_Mob *liste_temp = clone_listemob(liste);
                Liste_Mob *projectiles_temp = clone_listemob(projectiles);
                // essaye d'aller a droite
                for(int k=0; k<DELAI+5; k++){
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                    update_game_IA(liste_temp, projectiles_temp, p2);
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                }
                if(bouge){
                    if(n < matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]->vu){
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]->vu = n;
                        for(int d=0; d<n; d++){
                            matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]->chemin[n] = 1;
                        if(futur == NULL){
                            futur = create_file(matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]);
                        }else{
                            append_file(futur, matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]);  
                        }
                    }
                }else{ //if(bouge){
                    matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]->val = -99;
                }
                destroy_liste_mob(liste_temp);
                destroy_liste_mob(projectiles_temp);
            }else{
                matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+1+PROFONDEUR/2]->val = -99;
            }
            bouge = true;
            p2->_x_dest = (game->joueur->_x_dest)+32*encours->sommet->i;
            p2->_y_dest = (game->joueur->_y_dest)+32*encours->sommet->j;
            if((!IS_BLOCK(tile_map(game->carte, p2->_x_dest/32, (p2->_y_dest)/32 - 1))) && (encours->sommet->j)+PROFONDEUR/2 > 0){
                move_down_IA(game->carte, p2, liste, projectiles);
                Liste_Mob *liste_temp = clone_listemob(liste);
                Liste_Mob *projectiles_temp = clone_listemob(projectiles);
                for(int k=0; k<DELAI+5; k++){
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                    update_game_IA(liste_temp, projectiles_temp, p2);
                    if(test_death_IA(p2, game->carte, liste_temp, projectiles_temp)){
                        bouge = false;
                    }
                }
                if(bouge){
                    if(n < matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]->vu){
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]->vu = n;
                        for(int d=0; d<n; d++){
                            matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]->chemin[d] = matrice_eval[PROFONDEUR/2-(encours->sommet->j)][(encours->sommet->i)+PROFONDEUR/2]->chemin[d];
                        }
                        matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]->chemin[n] = 2;
                        if(futur == NULL){
                            futur = create_file(matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]);
                        }else{
                            append_file(futur, matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]);
                        }    
                    }
                }else{ //if(bouge){
                    matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]->val = -99;
                }
                destroy_liste_mob(liste_temp);
                destroy_liste_mob(projectiles_temp);
            }else{
                matrice_eval[PROFONDEUR/2-(encours->sommet->j)+1][(encours->sommet->i)+PROFONDEUR/2]->val = -99;
            }
            free(p2);
            encours = defile(encours);
            destroy_liste_mob(liste);
            destroy_liste_mob(projectiles);
            
        }
        n++;
        while(futur != NULL){
            if(encours != NULL){
                append_file(encours, futur->sommet);
            }else{
                free(encours);
                encours = create_file(futur->sommet);
            }
            futur = defile(futur);
        }
    }
    Point *max = NULL;
    int val_max = -99;
    for(int a=0; a<PROFONDEUR; a++){
        for(int b=0; b<PROFONDEUR; b++){
            if(matrice_eval[a][b]->vu != 99){
                if(matrice_eval[a][b]->val > val_max){
                    max = matrice_eval[a][b];
                    val_max = matrice_eval[a][b]->val;
                }
            }else{
                matrice_eval[a][b]->val = -50;
            }
        }
    }
    int directive = 4;
    if(max != NULL){
        directive = max->chemin[0];
    }

    destroy_file(encours);
    destroy_file(futur);
    destroy_matrice(matrice_eval);
    return directive;
}
