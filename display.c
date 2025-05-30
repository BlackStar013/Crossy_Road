#include "display.h"
#include "mob.h"

SDL_Surface** initSurfaceArrayMap(void){
    SDL_Surface** surfaceArrayMap = malloc(sizeof(SDL_Surface*)*NB_TILE);
    for(int i = 0; i<NB_TILE; i++){
        if(i==_PLAIN){              
            surfaceArrayMap[i] = IMG_Load("sprites/Plaines.png");
        }else if(i==_TREE){   
            surfaceArrayMap[i] = IMG_Load("sprites/Buisson.png");
        }else if(i==_ROAD){
            surfaceArrayMap[i] = IMG_Load("sprites/Routes.png");
        }else if(i==_PLAIN_BLOCK_LEFT){
            surfaceArrayMap[i] = IMG_Load("sprites/PlainesBlockLeft.png");
        }else if(i==_PLAIN_BLOCK_RIGHT){
            surfaceArrayMap[i] = IMG_Load("sprites/PlainesBlockRight.png");
        }else if(i==_ROAD_UP){
            surfaceArrayMap[i] = IMG_Load("sprites/RoutesLimiteHaut.png");
        }else if(i==_ROAD_DOWN){
            surfaceArrayMap[i] = IMG_Load("sprites/RouteLlimiteBas.png");
        }else if(i==_ROAD_MIDDLE){
            surfaceArrayMap[i] = IMG_Load("sprites/RouteCentre.png");
        }else if(i==_WALL){
            surfaceArrayMap[i] = IMG_Load("sprites/Rochers.png");
        }else if(i==_WATER){   
            surfaceArrayMap[i] = IMG_Load("sprites/Eaudoublebords.png");
        }else if(i==_WATER_UP){   
            surfaceArrayMap[i] = IMG_Load("sprites/Eaubordhaut.png");
        }else if(i==_WATER_DOWN){   
            surfaceArrayMap[i] = IMG_Load("sprites/Eaubordbas.png");
        }else if(i==_TREE){   
            surfaceArrayMap[i] = IMG_Load("sprites/Buisson.png");
        }else if(i==_RUINS){   
            surfaceArrayMap[i] = IMG_Load("sprites/DebrisRuines.png");
        }else if(i==_BROKENPLANKS){   
            surfaceArrayMap[i] = IMG_Load("sprites/Decombres.png");
        }else if(i==_TRANSITION){   
            surfaceArrayMap[i] = IMG_Load("sprites/TransitionManoir.png");
        }else if(i==_PLANKS){   
            surfaceArrayMap[i] = IMG_Load("sprites/Plancher.png");
        }else if(i==_CHANNEL){   
            surfaceArrayMap[i] = IMG_Load("sprites/Canal.png");
        }else if(i==_BASE){   
            surfaceArrayMap[i] = IMG_Load("sprites/Socle.png");
        }else if(i==_LOW_WALL){   
            surfaceArrayMap[i] = IMG_Load("sprites/Muret.png");
        }else if(i==_CAVE){   
            surfaceArrayMap[i] = IMG_Load("sprites/Grotte.png");
        }else if(i==_STALAGMITE){   
            surfaceArrayMap[i] = IMG_Load("sprites/Stalagmite.png");
        }else if(i==_MANOR_WALL){   
            surfaceArrayMap[i] = IMG_Load("sprites/MurSecret.png");
        }else if(i==_DOOR){   
            surfaceArrayMap[i] = IMG_Load("sprites/TransitionGrotte.png");
        }else if(i==_LEVER){   
            surfaceArrayMap[i] = IMG_Load("sprites/Grotte.png");
        }else{ //if(i==_WATER_MIDDLE){
            surfaceArrayMap[i] = IMG_Load("sprites/Eaumilieu.png");
        }
    }
    return surfaceArrayMap;
}

void freeSurfaceArrayMap(SDL_Surface** surfaceArrayMap){
    for(int i=0; i<NB_TILE; i++){
        SDL_FreeSurface(surfaceArrayMap[i]);
    }
    free(surfaceArrayMap);
}

SDL_Surface** initSurfaceArrayMob(void){
    SDL_Surface** surfaceArrayMob = malloc(sizeof(SDL_Surface*)*NB_MOB);
    for(int i = 0; i<NB_MOB; i++){
        if(i==CAR){              
            surfaceArrayMob[i] = IMG_Load("sprites/Chariot.png");
        }else if(i==BUS){   
            surfaceArrayMob[i] = IMG_Load("sprites/Patrouille.png");
        }else if(i==PLANK){
            surfaceArrayMob[i] = IMG_Load("sprites/Planche.png");
        }else if(i==CANON_MOB){
            surfaceArrayMob[i] = IMG_Load("sprites/Canon.png");
        }else if(i==BUSH){
            surfaceArrayMob[i] = IMG_Load("sprites/Buisson?.png");
        }else if(i==MAGIC_CAR){
            surfaceArrayMob[i] = IMG_Load("sprites/ChariotArcanique.png");
        }else if(i==MOB_CULTIST){   
            surfaceArrayMob[i] = IMG_Load("sprites/Cultiste.png");
        }else if(i==HIGHCULTIST){   
            surfaceArrayMob[i] = IMG_Load("sprites/HautCultiste.png");
        }else if(i==BEBERT){   
            surfaceArrayMob[i] = IMG_Load("sprites/Babelien.png");
        }//else{ //if(i==CURSE){
         //   surfaceArrayMob[i] = IMG_Load("sprites/ProjectileMaudit.png");
        // }
    }
    return surfaceArrayMob;
}

void freeSurfaceArrayMob(SDL_Surface** surfaceArrayMob){
    for(int i=0; i<NB_MOB; i++){
        SDL_FreeSurface(surfaceArrayMob[i]);
    }
    free(surfaceArrayMob);
}


SDL_Surface** initSurfaceExp(void){
    SDL_Surface** surfaceExp = malloc(sizeof(SDL_Surface*)*NB_EXP);
    for(int i = 0; i<NB_EXP; i++){
        if(i==EXP){              
            surfaceExp[i] = IMG_Load("sprites/xp.png");
        }else if(i==CIBLE){
            surfaceExp[i] = IMG_Load("sprites/Cible.png");
        }else {
            surfaceExp[i] = IMG_Load("sprites/Champignon.png");
        }
    }
    return surfaceExp;
}

void freeSurfaceExp(SDL_Surface** surfaceExp){
    for(int i = 0; i < NB_EXP;i++){
        SDL_FreeSurface(surfaceExp[i]);
    }
    free(surfaceExp);
}


SDL_Surface** initSurfaceArrayProjectile(void){
    SDL_Surface** surfaceArrayProjectile = malloc(sizeof(SDL_Surface*)*NB_PROJECTILE);
    for(int i = 0; i<NB_PROJECTILE; i++){
        if(i==CURSE){              
            surfaceArrayProjectile[i] = IMG_Load("sprites/ProjectileMaudit.png");
        }else if(i==FIRE_MOB){
            surfaceArrayProjectile[i] = IMG_Load("sprites/FEU.png");
        }else if(i==PEBBLE){   
            surfaceArrayProjectile[i] = IMG_Load("sprites/GrosRocher.png");
        }else if(i==LEVER){   
            surfaceArrayProjectile[i] = IMG_Load("sprites/Lever.png");
        }else if(i==CANON_BALL){
            surfaceArrayProjectile[i] = IMG_Load("sprites/Boulet.png");
        }else if(i==ARROW_KABI){
            surfaceArrayProjectile[i] = IMG_Load("sprites/Fleche.png");
        }else if(i==SPIKE_GROWTH){
            surfaceArrayProjectile[i] = IMG_Load("sprites/Epines.png");
        }else if(i==SMOKE1x1){
            surfaceArrayProjectile[i] = IMG_Load("sprites/part1x1.png");
        }else if(i==SMOKE1x2){
            surfaceArrayProjectile[i] = IMG_Load("sprites/part1x2.png");
        }else if(i==SMOKE2x1){
            surfaceArrayProjectile[i] = IMG_Load("sprites/part2x1.png");
        }else if(i==SMOKE2x2){
            surfaceArrayProjectile[i] = IMG_Load("sprites/part2x2.png");
        }else{ //if(i==SMOKE4x1){
            surfaceArrayProjectile[i] = IMG_Load("sprites/part4x1.png");
        }
    }
    return surfaceArrayProjectile;
}

void freeSurfaceArrayProjectile(SDL_Surface** surfaceArrayProjectile){
    for(int i=0; i<NB_PROJECTILE; i++){
        SDL_FreeSurface(surfaceArrayProjectile[i]);
    }
    free(surfaceArrayProjectile);
}

//IMG_Load("sprites/Kairon.png");

SDL_Surface** initSurfaceArrayPlayer(void){
    SDL_Surface** surfaceArrayPlayer = malloc(sizeof(SDL_Surface*)*NB_PERSO);
    for(int i = 0; i<NB_PERSO; i++){
        if(i==KAIRON){              
            surfaceArrayPlayer[i] = IMG_Load("sprites/Kairon.png");
        }else if(i==SNITCH){   
            surfaceArrayPlayer[i] = IMG_Load("sprites/Snitch.png");
        }else if(i==AESIR){
            surfaceArrayPlayer[i] = IMG_Load("sprites/Aesir.png");
        }else if(i==KABI){
            surfaceArrayPlayer[i] = IMG_Load("sprites/Kabi.png");
        }else if(i==OOGWAAY){
            surfaceArrayPlayer[i] = IMG_Load("sprites/Oogwaay.png");
        }else if(i==RANTON){
            surfaceArrayPlayer[i] = IMG_Load("sprites/Rantons.png");
        }else if(i==JOLINAR){
            surfaceArrayPlayer[i] = IMG_Load("sprites/Jolinar.png");
        }else{
            surfaceArrayPlayer[i] = IMG_Load("sprites/DeTruque.png");
        }
    }
    return surfaceArrayPlayer;
}

void freeSurfaceArrayPlayer(SDL_Surface** surfaceArrayPlayer){
    for(int i=0; i<NB_PERSO; i++){
        SDL_FreeSurface(surfaceArrayPlayer[i]);
    }
    free(surfaceArrayPlayer);
}
