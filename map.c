#include "map.h"
#include "tuiles.h"
#include <stdio.h>
#include <stdlib.h>

Line* create_empty_line(Line* prev, Line* next){
    //create a new empty line and connects it with the previous one and the next one if there are
    Line* nl = malloc(sizeof(Line));
    nl->tiles = malloc(sizeof(Tile)*LENGTH);
    for(int i = 0; i<LENGTH; i++){
        (nl->tiles[i]).is_block = false;
        (nl->tiles[i]).is_danger = false;
        (nl->tiles[i]).type = _PLAIN;
    }
    if(next == NULL){
        nl->next = NULL;
    }
    if(prev == NULL){
        nl->prev = NULL;
    }
    if(next != NULL){
        nl->next = next;
        (nl->next)->prev = nl;
    }
    if(prev != NULL){
        nl->prev = prev;
        (nl->prev)->next = nl;
    }
    return nl;
}

void add_buffer_line(Map* map){
    //buffer ->* head ->* tail
    map->head_buffer = create_empty_line(NULL, map->head_buffer);
}

void remove_buffer_line(Map* map){
    //removes buffer lines if there are
    if (map->head_buffer != map->head_map){
        Line* tmp = map->head_buffer;
        map->head_buffer = (map->head_buffer)->next;
        free(tmp->tiles);
        free(tmp);
    }
}

Map* create_empty_map(void){
    Map* map = malloc(sizeof(Map));
    map->head_map = create_empty_line(NULL, NULL);
    map->head_buffer = map->head_map;
    Line* cur = map->head_map;
    for(int i = 1; i<HEIGTH; i++){
        cur = create_empty_line(cur, NULL);
    }
    map->tail = cur;
    map->tail_buffer = map->tail;
    map->tail_buffer_lenght = 0;
    return map;
}

bool carte_vide(Map* map){
    Line* cur = map->head_map;
    for(int i = 0; i<HEIGTH; i++){
        for(int j = 0; j<LENGTH; j++){
            if((cur->tiles[j]).type != _PLAIN){
                return false;
            }
        }
        cur = cur->next;
    }
    return true;
}

void switch_map(Map *carte){
    // ajoute la premiere ligne du buffer a la map, et retire la dernière
    carte->head_map = carte->head_map->prev;
    //Line* last = carte->tail;
    carte->tail = carte->tail->prev;
    carte->tail_buffer_lenght++;
    //carte->tail->next = NULL;
    //destroy_line(last);
}


Tile tile_map(Map *carte, int x, int y){
    // renvoie la tuile de la map a ces coordonnées, et empêche de sortir des côtés de la carte
    if((y>=HEIGTH)||(x>=LENGTH)||(y<0)||(x<0)){
        return danger;
    }
    Line* cur = carte->tail_buffer;
    while(y+carte->tail_buffer_lenght>0){
        cur = cur->prev;
        y--;
    }
    //printf("%d\n", cur->tiles[x].is_block);
    return ((cur->tiles)[x]);
}

void updateLocks(int* unlocked, int nb_exp){
    for (int i = 0; i<8; i++){
        if(i <= 1){
            unlocked[i] = 1;
        }
        else{
            unlocked[i] = 0;
        }
    }
    if(nb_exp >= 3){
        unlocked[KAIRON] = 1;
    }
    if(nb_exp >= 5){
        unlocked[RANTON] = 1;
    }
    if(nb_exp >= 7){
        unlocked[KAIRON] = 2;
        unlocked[SNITCH] = 1;
    }
    if(nb_exp >= 12){
        unlocked[RANTON] = 2;
    }
    if(nb_exp >= 20){
        unlocked[JOLINAR] = 1;
    }
    if(nb_exp >= 33){
        unlocked[SNITCH] = 2;
    }
    if(nb_exp >= 54){
        unlocked[KAIRON] = 3;
    }
    if(nb_exp >= 90){
        unlocked[OOGWAAY] = 1;
    }
    if(nb_exp >= 148){
        unlocked[JOLINAR] = 2;
    }
    if(nb_exp >= 244){
        unlocked[SNITCH] = 3;
    }
    if(nb_exp >= 403){
        unlocked[RANTON] = 3;
    }
    if(nb_exp >= 665){
        unlocked[OOGWAAY] = 2;
    }
    if(nb_exp >= 1096){
        unlocked[KABI] = 1;
    }
    if(nb_exp >= 1808){
        unlocked[JOLINAR] = 3;
    }
    if(nb_exp >= 2980){
        unlocked[KABI] = 2;
    }
    if(nb_exp >= 4030){
        unlocked[OOGWAAY] = 3;
    }
    if(nb_exp >= 5000){
        unlocked[KABI] = 3;
    }
}

Player* initialise_player(int id, int tot_exp){
    // intitialise le joueur au milieu en bas de la carte
    int* lvls = malloc(sizeof(int)*8);
    updateLocks(lvls, tot_exp);
    Player* joueur = malloc(sizeof(Player));
    joueur->_x = ((LENGTH-1)/2)*32;
    joueur->_y = 2*32;
    joueur->_x_dest = ((LENGTH-1)/2)*32;
    joueur->_y_dest = 2*32;
    joueur->offset_x = 0;
    joueur->offset_y = 1;
    joueur->character_id = id;
    joueur->delai = DELAI;
    joueur->character_delay = 0;
    joueur->nb_frame_to_change = -1;
    joueur->offset_sup = 0;
    joueur->character_lvl = lvls[id];
    joueur->inv_frame = 0;
    switch(id){
        case KAIRON:
            joueur->character_value = joueur->character_lvl + 255;
            joueur->character_max_delay = 0;
            break;

        case SNITCH:
            switch(joueur->character_lvl){
                case 1:
                    joueur->character_value = 64;
                    joueur->character_max_delay = 28;
                    break;

                case 2:
                    joueur->character_value = 96;
                    joueur->character_max_delay = 28;
                    break;

                case 3:
                    joueur->character_value = 96;
                    joueur->character_max_delay = 22;
                    break;

                default:
                    joueur->character_value = 0;
                    joueur->character_max_delay = -1;
                    break;

            }
            break;
        
        case KABI:
        switch(joueur->character_lvl){
                case 1:
                    joueur->character_max_delay = 12;
                    break;

                case 2:
                    joueur->character_max_delay = 6;
                    break;

                case 3:
                    joueur->character_max_delay = 3;
                    break;

                default:
                    joueur->character_max_delay = -1;
                    break;

            }
            joueur->character_value = 42;
            break;

        case OOGWAAY:
            switch(joueur->character_lvl){
                case 1:
                    joueur->character_max_delay = 64*5;
                    break;

                case 2:
                    joueur->character_max_delay = 64*3;
                    break;

                case 3:
                    joueur->character_max_delay = 64;
                    break;

                default:
                    joueur->character_max_delay = -1;
                    break;

            }
            joueur->character_value = 0;
            break;

        case RANTON:
            if(joueur->character_lvl>=3){
                joueur->character_value = 2;
            } else {
                joueur->character_value = 1;
            }
            break;

        case JOLINAR:
            joueur->character_value = 2;
             switch(joueur->character_lvl){
                case 1:
                    joueur->character_max_delay = 28;
                    break;

                case 2:
                    joueur->character_max_delay = 20;
                    break;

                case 3:
                    joueur->character_max_delay = 12;
                    break;

                default:
                    joueur->character_max_delay = -1;
                    break;
            }
            break;

        default:
            joueur->character_value = 0;
            joueur->character_max_delay = 0;
            break;
    }
    joueur->character_lvl = lvls[id];
    free(lvls);
    return joueur;
}

void destroy_line(Line *line){
    free(line->tiles);
    free(line);
}

void destroy_map(Map *map){
    Line *line = map->head_buffer;
    while(line != NULL){
        Line *mem = line->next;
        destroy_line(line);
        line = mem;
    }
    free(map);
}

void destroy_tail_buffer(Map *map){
    Line *line = map->tail->next;
    while(line != NULL){
        Line *mem = line->next;
        destroy_line(line);
        line = mem;
    }
    map->tail->next = NULL;
    map->tail_buffer = map->tail;
    map->tail_buffer_lenght = 0;
}

void destroy_last_buffer(Map* map){
    if(map->tail_buffer!=map->tail){
        Line* line = map->tail_buffer;
        map->tail_buffer = map->tail_buffer->prev;
        map->tail_buffer->next = NULL;
        map->tail_buffer_lenght--;
        destroy_line(line);
    }
}
