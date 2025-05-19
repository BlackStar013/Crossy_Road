#include <ncurses.h>
#include "map.h"
#include "tuiles.h"
#include "core.h"
#include "mob.h"
#include <stdbool.h>
#include "ia.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


//Global variable

bool emoji = false;
int character = KAIRON;
bool hascolor = true;

enum COLORS {
    LET_IT_GOOOOOO,
    ON_GRASS,
    ON_GRASS_EVIL,
    ON_GRASS_WEIRD_EVIL,
    ON_GRASS_CAILLOU,
    ON_PLANKS,
    ON_PLANKS_EVIL,
    ON_CAVE,
    ON_CAVE_EVIL,
    ON_WATER,
    ON_WATER_GOOD,
    PLAYER_KAIRON,
    PLAYER_SNITCH,
    PLAYER_AESIR,
    PLAYER_KABI,
    PLAYER_OOGWAY,
    PLAYER_RANTONS,
    PLAYER_JOLINAR,
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
    timeout(15);
    if(has_colors() ==FALSE){
        hascolor = false;
    } else {
        start_color();
        init_pair(ON_GRASS, COLOR_YELLOW, COLOR_GREEN);
        init_pair(ON_GRASS_CAILLOU, COLOR_WHITE, COLOR_GREEN);
        init_pair(ON_GRASS_EVIL, COLOR_RED, COLOR_GREEN);
        init_pair(ON_GRASS_WEIRD_EVIL, COLOR_MAGENTA, COLOR_GREEN);
        init_pair(ON_PLANKS, COLOR_BLACK, COLOR_YELLOW);
        init_pair(ON_PLANKS_EVIL, COLOR_RED, COLOR_YELLOW);
        init_pair(ON_CAVE, COLOR_WHITE, COLOR_BLACK);
        init_pair(ON_CAVE_EVIL, COLOR_RED, COLOR_BLACK);
        init_pair(ON_WATER, COLOR_CYAN, COLOR_BLUE);
        init_pair(ON_WATER_GOOD, COLOR_YELLOW, COLOR_BLUE);
        init_pair(PLAYER_KAIRON, COLOR_RED, COLOR_WHITE);
        init_pair(PLAYER_AESIR, COLOR_YELLOW, COLOR_WHITE);
        init_pair(PLAYER_SNITCH, COLOR_WHITE, COLOR_BLACK);
        init_pair(PLAYER_KABI, COLOR_YELLOW, COLOR_BLACK);
        init_pair(PLAYER_OOGWAY, COLOR_BLACK, COLOR_GREEN);
        init_pair(PLAYER_RANTONS, COLOR_GREEN, COLOR_WHITE);
        init_pair(PLAYER_JOLINAR, COLOR_GREEN, COLOR_BLACK);
    }
    curs_set(FALSE);
}

void draw_background(Game *game){
    Line *line = game->carte->head_map;
    for(int i = 0; i < HEIGTH; i++){
        for(int j = 0; j < LENGTH; j++){
            switch (line->tiles[j].type) {

                case _PLAIN :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS));
                    }
                    mvaddch(i,j,' ');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS));
                    }
                    break;

                 case _PLAIN_BLOCK_LEFT :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    mvaddch(i,j,'}');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    break;

                case _PLAIN_BLOCK_RIGHT:
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    mvaddch(i,j,'{');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    break;

                case _ROAD :
                case _ROAD_DOWN :
                case _ROAD_MIDDLE :
                case _ROAD_UP :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS));
                    }
                    mvaddch(i,j,'=');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS));
                    }
                    break;

                case _WATER :
                case _WATER_DOWN :
                case _WATER_MIDDLE :
                case _WATER_UP :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_WATER));
                    }
                    mvaddch(i,j,' ');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_WATER));
                    }
                    break;

                case _TREE :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS));
                    }
                    mvaddch(i,j,'%');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS));
                    }
                    break;

                case _WALL :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    break;

                case _DANGER :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS_EVIL));
                    }
                    mvaddch(i,j,'!');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS_EVIL));
                    }
                    break;

                case _RUINS :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_GRASS_CAILLOU));
                    }
                    break;

                case _BROKENPLANKS :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_PLANKS));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_PLANKS));
                    }
                    break;

                case _TRANSITION :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_PLANKS));
                    }
                    mvaddch(i,j,' ');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_PLANKS));
                    }
                    break;

                case _PLANKS :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_PLANKS));
                    }
                    mvaddch(i,j,' ');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_PLANKS));
                    }
                    break;

                case _CHANNEL :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_CAVE));
                    }
                    mvaddch(i,j,' ');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_CAVE));
                    }
                    break;

                case _BASE :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_CAVE_EVIL));
                    }
                    mvaddch(i,j,'0');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_CAVE_EVIL));
                    }
                    break;

                case _LOW_WALL :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_PLANKS));
                    }
                    mvaddch(i,j,'-');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_PLANKS));
                    }
                    break;

                case _CAVE :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_CAVE));
                    }
                    mvaddch(i,j,' ');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_CAVE));
                    }
                    break;

                case _STALAGMITE :
                    if(hascolor){
                        attron(COLOR_PAIR(ON_CAVE));
                    }
                    mvaddch(i,j,'^');
                    if(hascolor){
                        attroff(COLOR_PAIR(ON_CAVE));
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
    if((game->joueur->inv_frame%2)){
        return;
    }
    switch(game->joueur->character_id){
        case KAIRON:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_KAIRON));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'"');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_KAIRON));
            }
            break;

        case AESIR:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_AESIR));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'o');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_AESIR));
            }
            break;

        case SNITCH:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_SNITCH));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'n');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_SNITCH));
            }
            break;

        case KABI:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_KABI));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'^');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_KABI));
            }
            break;
        
        case OOGWAAY:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_OOGWAY));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'#');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_OOGWAY));
            }
            break;

        case RANTON:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_RANTONS));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'"');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_RANTONS));
            }
            break;

        case JOLINAR:
            if(hascolor){
            attron(COLOR_PAIR(PLAYER_JOLINAR));
            }
            mvaddch(HEIGTH - to_nb(game->joueur->_y_dest) - 1,to_nb(game->joueur->_x_dest),'&');
            if(hascolor){
                attroff(COLOR_PAIR(PLAYER_JOLINAR));
            }
            break;

    }

}

void draw_mob(Mob* mob){
    switch(mob->id_mob){
        case CAR :
        case BUS :
            if(hascolor){
                attron(COLOR_PAIR(ON_GRASS_EVIL));
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
                attroff(COLOR_PAIR(ON_GRASS_EVIL));
            } 
            break;

        case MAGIC_CAR :
            if(hascolor){
                attron(COLOR_PAIR(ON_GRASS_WEIRD_EVIL));
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
                attroff(COLOR_PAIR(ON_GRASS_WEIRD_EVIL));
            } 
            break;

        case PLANK :
            if(hascolor){
                attron(COLOR_PAIR(ON_WATER_GOOD));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'-');
                        }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(ON_WATER_GOOD));
            } 
            break;

        case CANON_MOB :
            if(hascolor){
                if(mob->offset_x==1){
                    attron(COLOR_PAIR(ON_GRASS_EVIL));
                } else  if(mob->offset_x==2) {
                    attron(COLOR_PAIR(ON_GRASS));
                } else {
                    attron(COLOR_PAIR(ON_GRASS_CAILLOU));
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
                if(mob->offset_x==1){
                    attroff(COLOR_PAIR(ON_GRASS_EVIL));
                } else  if(mob->offset_x==2) {
                    attroff(COLOR_PAIR(ON_GRASS));
                } else {
                    attroff(COLOR_PAIR(ON_GRASS_CAILLOU));
                }
            }
            break;

        case BUSH :
            if(hascolor){
                if(mob->offset_y==1){
                    attron(COLOR_PAIR(ON_GRASS_EVIL));
                } else {
                    attron(COLOR_PAIR(ON_GRASS));
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
                    attroff(COLOR_PAIR(ON_GRASS_EVIL));
                } else {
                    attroff(COLOR_PAIR(ON_GRASS));
                }
            }
            break;


        case MOB_CULTIST :
        case BEBERT :
            if(hascolor){
                attron(COLOR_PAIR(ON_CAVE_EVIL));
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
                attroff(COLOR_PAIR(ON_CAVE_EVIL));
            } 
            break;



        case HIGHCULTIST :
            if(hascolor){
                attron(COLOR_PAIR(ON_CAVE_EVIL));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                        mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'8');
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(ON_CAVE_EVIL));
            } 
            break;

         case PEBBLE :
            if(hascolor){
                attron(COLOR_PAIR(ON_CAVE_EVIL));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                        if(i){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,')');
                        } else {
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'(');
                        }
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(ON_CAVE_EVIL));
            } 
            break;


    }


}


void draw_projectile(Mob* mob){
    switch(mob->id_mob){
        case CURSE :
            if(hascolor){
                attron(COLOR_PAIR(ON_CAVE_EVIL));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                        mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'o');
                    }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(ON_CAVE_EVIL));
            } 
            break;
        
        case CANON_BALL :
            if(hascolor){
                attron(COLOR_PAIR(ON_GRASS_EVIL));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'o');
                        }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(ON_GRASS_EVIL));
            }
            break;


        case FIRE_MOB :
            if(hascolor){
                attron(COLOR_PAIR(ON_PLANKS_EVIL));
            }
            for(int i = 0; i<mob->length;i++){
                for(int j = 0;j<mob->heigth;j++){
                    if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                            mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'*');
                        }
                }
            }
            if(hascolor){
                attroff(COLOR_PAIR(ON_PLANKS_EVIL));
            } 
            break;

        case ARROW_KABI:
            if(hascolor){
                attron(COLOR_PAIR(PLAYER_KABI));
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
                attroff(COLOR_PAIR(PLAYER_KABI));
            } 
            break;
    }
}

void draw_exp(Mob* mob){
    if(hascolor){
        attron(COLOR_PAIR(ON_WATER));
    }
    for(int i = 0; i<mob->length;i++){
        for(int j = 0;j<mob->heigth;j++){
            if(in_map(to_nb(mob->_x)+i, to_nb(mob->_y) +j)){
                mvaddch(HEIGTH - to_nb(mob->_y) - 1 - j,to_nb(mob->_x)+i,'+');
            }
        }
    }
    if(hascolor){
        attroff(COLOR_PAIR(ON_WATER));
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



void draw_power(Game* game){
    switch(game->joueur->character_id){
        case KAIRON :
            wmove(stdscr,0,LENGTH);
            printw("Life : %d",game->joueur->character_value);
            break;

        case SNITCH:
        case KABI:
        case OOGWAAY:
        case JOLINAR:
            if(game->joueur->character_delay){
                wmove(stdscr,0,LENGTH+1);
                printw("Power : Not enough energy");
                wmove(stdscr,1,LENGTH+1);
                printw("(%d step to go) ",game->joueur->character_delay);
            } else {
                wmove(stdscr,0,LENGTH+1);
                printw("Power : Ready !              ");
                wmove(stdscr,1,LENGTH+1);
                printw("                             ");
            }
            break;
    }
}

void draw_all(Game* game){
    draw_background(game);
    draw_all_mob_game(game);
    draw_character(game);
    draw_power(game);
    wmove(stdscr,HEIGTH,0);
    printw("Score : %d",game->score);
    refresh();
}


void draw_menu(void){
    mvaddstr(0,0,"  --------    Choose your  ");
    mvaddstr(1,0,"  |      |     character   ");
    mvaddstr(2,0,"  |    o |                 ");
    mvaddstr(3,0,"  |      |                 ");
    switch (character) {
        case KAIRON:
            mvaddstr(4,0,"  |      |  <|--KAIRON--|> ");
            break;
        
        case SNITCH:
            mvaddstr(4,0,"  |      |  <|--SNITCH--|> ");
            break;

        case AESIR:
            mvaddstr(4,0,"  |      |  <|--AESIR --|> ");
            break;
        
        case KABI:
            mvaddstr(4,0,"  |      |  <|-- KABI --|> ");
            break;
        
        case OOGWAAY:
            mvaddstr(4,0,"  |      |  <|--OOGWAAY--|>");
            break;

        case JOLINAR:
            mvaddstr(4,0,"  |      |  <|--JOLINAR--|>");
            break;

        case RANTON:
            mvaddstr(4,0,"  |      |  <|--RANTON--|> ");
            break;
        
    
    }
    mvaddstr(4,0,"  |      |  ");
    mvaddstr(5,0,"  --------  ");
    mvaddstr(6,0,"Press enter to open the door");
    mvaddstr(7,0,"of the incredible Crossy Road !");
    mvaddstr(9,0,"Or press m to quit...");

    refresh();
}

void draw_pause(void){
    mvaddstr(HEIGTH+1,0,"Press enter to resume the game !");
    refresh();
}

int main(void){
    int ch;
    bool quit = false;
    bool quit_game = true;
    bool pause = false;
    ncursesInit();
    while(!quit){
        draw_menu();
        ch = getch();
        if(ch=='\n'){
            clear();
            quit_game = false;
        } else if(ch=='m'||ch=='M'){
            quit = true;
        } else if((ch == KEY_RIGHT) || (ch == 'd') || (ch == 'D')){
            character++;
            character%=NB_PERSO;
        } else if((ch == KEY_LEFT) || (ch == 'q') || (ch == 'Q')){
            character--;
            if(character<0){
                character = NB_PERSO-1;
            }
        }
        if(!quit_game){
            Game *game = initialise_game(character,0);
            while(!quit_game){
                while(pause){
                    ch = getch();
                    draw_pause();
                    if(ch=='\n'){
                        clear();
                        pause=false;
                    }
                    
                }
                if(test_death(game)){
                    quit_game = true;
                }
                update_game(game);
                draw_all(game);
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
                        quit_game = true;
                    }else if(ch == '\n'){
                        pause = true;
                    }else if(ch == ' '){
                        use_power(game);
                    }

            }
            clear();
            destroy_game(game);
        }
    }
    nocbreak();
    endwin();
    endwin();
    return 0;
}
