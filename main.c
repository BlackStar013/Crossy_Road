#include <ncurses.h>
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


//Global variable

bool emoji = false;
int character = KAIRON;
bool hascolor = true;

enum COLORS {
    GREEN,
    BLUE,
    WHITE,
    RED,
    YELLOW,
    NB_COLOR
};

int to_nb(int pix){
    if(pix%32 > 16){
        return (pix/32+1);
    } else {
        return (pix/32);
   }
}

bool in_map(int x, int y){
    if((x>=0)&&(y>=0)){
        if((x<LENGTH)&&(y<HEIGTH)){
            return true;
        }
    }
    return false;
}

void ncursesInit(void){
    initscr();
	noecho(); 
	keypad(stdscr, TRUE);
	cbreak();
    timeout(10);
    if(has_colors() ==FALSE){
        hascolor = false;
    } else {
        start_color();
        init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
        init_pair(RED, COLOR_RED, COLOR_BLACK);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    }
    curs_set(FALSE);
}

void draw_background(Game *game){
    Line *line = game->carte->head_map;
    for(int i = 0; i < HEIGTH; i++){
        for(int j = 0; j < LENGTH; j++){
            switch (line->tiles[j].type) {

                case _PLAIN :
                case _PLAIN_BLOCK :
                    if(hascolor){
                        attron(COLOR_PAIR(GREEN));
                    }
                    mvaddch(i,j,':');
                    break;

                case _ROAD :
                case _ROAD_DOWN :
                case _ROAD_MIDDLE :
                case _ROAD_UP :
                    if(hascolor){
                        attron(COLOR_PAIR(YELLOW));
                    }
                    mvaddch(i,j,'=');
                    if(hascolor){
                        attroff(COLOR_PAIR(YELLOW));
                    }
                    break;

                case _WATER :
                case _WATER_DOWN :
                case _WATER_MIDDLE :
                case _WATER_UP :
                    if(hascolor){
                        attron(COLOR_PAIR(BLUE));
                    }
                    mvaddch(i,j,'~');
                    if(hascolor){
                        attroff(COLOR_PAIR(BLUE));
                    }
                    break;

                case _TREE :
                    if(hascolor){
                        attron(COLOR_PAIR(GREEN));
                    }
                    mvaddch(i,j,'%');
                    if(hascolor){
                        attroff(COLOR_PAIR(GREEN));
                    }
                    break;

                case _WALL :
                    if(hascolor){
                        attron(COLOR_PAIR(WHITE));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

                case _DANGER :
                    if(hascolor){
                        attron(COLOR_PAIR(RED));
                    }
                    mvaddch(i,j,'!');
                    if(hascolor){
                        attroff(COLOR_PAIR(RED));
                    }
                    break;

                case _RUINS :
                    if(hascolor){
                        attron(COLOR_PAIR(WHITE));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

                case _BROKENPLANKS :
                    if(hascolor){
                        attron(COLOR_PAIR(RED));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(RED));
                    }
                    break;

                case _TRANSITION :
                    if(hascolor){
                        attron(COLOR_PAIR(WHITE));
                    }
                    mvaddch(i,j,'_');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

                case _PLANKS :
                    if(hascolor){
                        attron(COLOR_PAIR(YELLOW));
                    }
                    mvaddch(i,j,'=');
                    if(hascolor){
                        attroff(COLOR_PAIR(YELLOW));
                    }
                    break;

                case _CHANNEL :
                    if(hascolor){
                        attron(COLOR_PAIR(RED));
                    }
                    mvaddch(i,j,'-');
                    if(hascolor){
                        attroff(COLOR_PAIR(RED));
                    }
                    break;

                case _BASE :
                    if(hascolor){
                        attron(COLOR_PAIR(RED));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

                case _LOW_WALL :
                    if(hascolor){
                        attron(COLOR_PAIR(WHITE));
                    }
                    mvaddch(i,j,'-');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

                case _CAVE :
                    if(hascolor){
                        attron(COLOR_PAIR(WHITE));
                    }
                    mvaddch(i,j,'=');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

                case _STALAGMITE :
                    if(hascolor){
                        attron(COLOR_PAIR(WHITE));
                    }
                    mvaddch(i,j,'^');
                    if(hascolor){
                        attroff(COLOR_PAIR(WHITE));
                    }
                    break;

            }
        }
        if(line!=NULL){
            line=line->next;
        }
    }

}


void draw_character(Game* game){
    if(hascolor){
        attron(COLOR_PAIR(RED));
    }
    mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'"');
    if(hascolor){
        attroff(COLOR_PAIR(RED));
    }

}

void draw_mob(Mob* mob){
    switch(mob->id_mob){
        case CAR :
        case BUS :
            if(hascolor){
                attron(COLOR_PAIR(RED));
            }
            if(mob->offset_y == 1){
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'<');
                        }
                    }
                }
            } else {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'>');
                        }
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(RED));
            } 
            break;

        case PLANK :
            if(hascolor){
                attron(COLOR_PAIR(YELLOW));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'-');
                        }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(YELLOW));
            } 
            break;

        case CANON_MOB :
            if(hascolor){
                if(mob->offset_x>0){
                    attron(COLOR_PAIR(RED));
                } else {
                    attron(COLOR_PAIR(YELLOW));
                }
            }
            if(mob->offset_y == 1){
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'[');
                        }
                    }
                }
            } else {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,']');
                        }
                    }
                }
            }
            if(hascolor){
                if(mob->offset_x>0){
                    attroff(COLOR_PAIR(RED));
                } else {
                    attroff(COLOR_PAIR(YELLOW));
                }
            }
            break;

        case CANON_BALL :
            if(hascolor){
                attron(COLOR_PAIR(RED));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'o');
                        }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(RED));
            }
            break;


        case FIRECLOCK :
        case FIRECOUNTER :
            if(hascolor){
                attron(COLOR_PAIR(RED));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'*');
                        }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(RED));
            } 
            break;

        case BUSH :
            if(hascolor){
                if(mob->offset_y==1){
                    attron(COLOR_PAIR(RED));
                } else {
                    attron(COLOR_PAIR(GREEN));
                }
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                   if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'%');
                        }
                }
            }
            if(hascolor){
                if(mob->offset_y==1){
                    attroff(COLOR_PAIR(RED));
                } else {
                    attroff(COLOR_PAIR(GREEN));
                }
            }


        case MOB_CULTIST :
            if(hascolor){
                attron(COLOR_PAIR(RED));
            }
            if(mob->offset_y == 3){
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'<');
                        }
                    }
                }
            } else if (mob->offset_y == 2) {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'>');
                        }
                    }
                }
            } else if (mob->offset_y == 1) {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'^');
                        }
                    }
                }
            } else {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'v');
                        }
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(RED));
            } 
            break;



        case HIGHCULTIST :
            if(hascolor){
                attron(COLOR_PAIR(RED));
            }
            if(mob->offset_y == 3){
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'<');
                        }
                    }
                }
            } else if (mob->offset_y == 2) {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'>');
                        }
                    }
                }
            } else if (mob->offset_y == 1) {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'^');
                        }
                    }
                }
            } else {
                for(int i = 0; i<mob->length;i++){
                    for(int j = 0;j<mob->heigth;j++){
                        if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'v');
                        }
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(RED));
            } 
            break;


    }


}


void draw_projectile(Mob* mob){
    switch(mob->id_mob){
        case CURSE :
            if(hascolor){
                attron(COLOR_PAIR(RED));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                        mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'o');
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(RED));
            } 
            break;
    }
}

void draw_exp(Mob* mob){
    if(hascolor){
        attron(COLOR_PAIR(BLUE));
    }
    for(int i = 0; i<mob->length;i++){
        for(int j = 0;j<mob->heigth;j++){
            if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'+');
            }
        }
    }
    if(hascolor){
        attroff(COLOR_PAIR(BLUE));
    } 
}

void draw_all_mob(Liste_Mob* list){
    while(list!=NULL){
        draw_mob(list->mob);
        list = list->next;
    }
}

void draw_all_exp(Liste_Mob* list){
    while(list!=NULL){
        draw_exp(list->mob);
        list = list->next;
    }
}

void draw_all_proj(Liste_Mob* list){
    while(list!=NULL){
        draw_projectile(list->mob);
        list = list->next;
    }
}

void draw_all_mob_game(Game* game){
    draw_all_exp(game->exp);
    draw_all_mob(game->mobs);
    draw_all_proj(game->projectile);
}


void draw_all(Game* game){
    clear();
    draw_background(game);
    draw_all_mob_game(game);
    draw_character(game);
    refresh();
}

int main(void){
    ncursesInit();
    Game *game = initialise_game(character);
    bool quit = false;
    int ch;
    while(!quit){
        if(test_death(game)){
            quit = true;
        }
        update_game(game);
        ch = getch();
        if((ch == KEY_UP) || (ch == 'z') || (ch == 'Z')){
                // si le joueur avance de plus de 3 cases, on avance la carte, sinon c'est qu'il avait reculé donc on avance pas toute la map
                if(move_up(game)&&game->joueur->_y_dest>=3*32){
                    switch_game(game);
                    game->mobs = destroy_liste_mob_from(-1, game->mobs);
                    game->exp = destroy_liste_mob_from(-1, game->exp);
                    game->projectile = destroy_liste_mob_from(-2*HEIGTH, game->projectile);
                    destroy_tail_buffer(game->carte);
                }
                //sert à changement l'image du joueur quand il se déplace
            }else if((ch == KEY_LEFT) || (ch == 'q') || (ch == 'Q')){
                move_left(game);
            }else if((ch == KEY_DOWN) || (ch == 's') || (ch == 'S')){
                move_down(game);
            }else if((ch == KEY_RIGHT) || (ch == 'd') || (ch == 'D')){
                move_right(game);
            }else if(ch == 'm' || (ch == 'M')){
                quit = true;
            }
        draw_all(game);
        
    }
    destroy_game(game);
    nocbreak();
    endwin();
    endwin();
    return 0;
}