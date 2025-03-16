#include "PrototypeRoutes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void init_display(void){
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            printf("#");
        }
        printf("\n");
    }
}

void update_display(map* place){
    srand(time(NULL));
    printf("\033[%dA\r", HEIGTH);
    for (int i = 0; i<HEIGTH;i++){
        for (int j = 0; j<LENGTH; j++){
            printf("\033[32m%c",((tile_map(j, i, place))->type));
        }
        printf("\n");
    }
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    map* place = initialize_map();
    init_display();
    srand(time(NULL));
    //int ch = getch();
    while(1){
        place = initialize_map();
        int i = rand()%16;
        int j = rand()%16;
        TYPE(tile_map(j, i, place)) = ' ';
        update_display(place);
        //ch = getch();
        //printf("%d", ch);
        sleep(1);
    }
    return 0;
}