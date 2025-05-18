#include "SDLdisplay.h"

void display_map(Map *map, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayMap){
    //parcours toute la map ligne par ligne et charge le sprite correspondant
    Line *line = map->head_map;
    for (int i = 0; i<HEIGTH+map->tail_buffer_lenght;i++){
        for (int j = 0; j<LENGTH; j++){
            SDL_Surface* spriteSurface;
            spriteSurface = surfaceArrayMap[line->tiles[j].type];
            // creer la surface pour afficher
            if (!spriteSurface) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            //creer la textire avec la surface et le sprite
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) { 
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            // spriteWidth correspond à la largeur du fichier, ici je me sers de la taille des cases simples pour l'échelle, donc pas de division ni produit
            int spriteWidth = spriteFullWidth;   //sinon (spriteFullWidth / nbSpritesInSprite)*ECHELLE
            int spriteHeight = spriteFullHeight;
            // recupère dans le fichier la position depuis en haut à gauche (d'ou le (0, 0)pour x et y) et la taille calculée au dessus
            SDL_Rect spriteRect = {.x = 0, .y = 0, .w = spriteWidth, .h = spriteHeight};
            // x et y correspondent à la position où on va l'afficher, et w et h sa taille
            SDL_Rect destRect = {.x = 32*j*ECHELLE, .y = (32*(i)-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE}; //
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        }
        if(line!=NULL){
            line=line->next;
        }
    }
}

void display_joueur(Player *joueur, SDL_Renderer *renderer,int pix_defil, SDL_Surface** surfaceArrayPlayer){
    // affiche le joueur selon le même principe que ci-dessus
    SDL_Surface *spriteSurface = surfaceArrayPlayer[joueur->character_id];
    if (!spriteSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
        exit(-1);
    }
    SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
    if(!spriteTexture){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
        exit(-1);
    }
    int spriteFullWidth;
    int spriteFullHeight;
    if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
        exit(-1);
    }
    int spriteWidth = (spriteFullWidth / 4 ); //ici pour le joueur, on a 4 lignes et 4 colonnes pour les 4 orientations et 4 "gestes" d'où le /4
    int spriteHeight = (spriteFullHeight / 4 );
    SDL_Rect spriteRect = {.x = joueur->offset_x*spriteWidth, .y = joueur->offset_y*spriteWidth, .w = spriteWidth, .h = spriteHeight};
    SDL_Rect destRect = {.x = (joueur->_x)*ECHELLE, .y = (HEIGTH*32-joueur->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
    if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
    }
    SDL_DestroyTexture(spriteTexture);
}

void display_exp(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceExp){
    // affiche les projectiles un par un selon le même principe que ci-ci-dessus-dessus
    while(liste!=NULL){
        SDL_Surface *spriteSurface = *surfaceExp;
        if (!spriteSurface){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
            exit(-1);
        }
        SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
        if(!spriteTexture){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
            exit(-1);
        }
        int spriteFullWidth;
        int spriteFullHeight;
        if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
            exit(-1);
        }
        int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
        int spriteHeight = spriteFullHeight;
        SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
        SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
        if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
        }
        SDL_DestroyTexture(spriteTexture);
        liste = liste->next;
    }
}

void display_mob(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayMob){
    // affiche les mobs un par un selon le même principe que ci-ci-dessus-dessus
    while(liste!=NULL){
        SDL_Surface *spriteSurface = surfaceArrayMob[liste->mob->id_mob];
        if((liste->mob->id_mob==CAR)||(liste->mob->id_mob==MAGIC_CAR)){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = (spriteFullWidth / 2); // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = (spriteFullHeight / 2);
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y = (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        }else if(liste->mob->id_mob==PLANK){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y = 0, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==CANON_MOB){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/3; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==CANON_BALL){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/3; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==BUS){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==FIRE_MOB){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==BUSH){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/2; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==MOB_CULTIST){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/4;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        } else if(liste->mob->id_mob==HIGHCULTIST){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/4;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        }  else if(liste->mob->id_mob==PEBBLE){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/8; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/2;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        }else if(liste->mob->id_mob==BEBERT){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/4;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        }// else if(liste->mob->id_mob==CURSE){
         //   if (!spriteSurface){
         //       SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
         //       exit(-1);
         //   }
         //   SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
         //   if(!spriteTexture){
         //       SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
         //       exit(-1);
         //   }
         //   int spriteFullWidth;
         //   int spriteFullHeight;
         //   if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
         //       SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
         //       exit(-1);
         //   }
         //   int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
         //   int spriteHeight = spriteFullHeight/8;
         //   SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
         //   SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
         //   if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
         //       SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
         //   }
         //   SDL_DestroyTexture(spriteTexture);
        // }
        liste = liste->next;
    }
}

void display_projectile(Liste_Mob *liste, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayProjectile){
    // affiche les projectiles un par un selon le même principe que ci-ci-dessus-dessus
    while(liste!=NULL){
        SDL_Surface *spriteSurface = surfaceArrayProjectile[liste->mob->id_mob];//liste->mob->id_mob];
         if(liste->mob->id_mob==CURSE){
            if (!spriteSurface){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation du sprite : %s", IMG_GetError());
                exit(-1);
            }
            SDL_Texture *spriteTexture=SDL_CreateTextureFromSurface(renderer, spriteSurface);
            if(!spriteTexture){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur a l'initialisation de la texture : %s", SDL_GetError());
                exit(-1);
            }
            int spriteFullWidth;
            int spriteFullHeight;
            if (SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteFullWidth, &spriteFullHeight)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur au chargement de la texture: %s", SDL_GetError());
                exit(-1);
            }
            int spriteWidth = spriteFullWidth/4; // pour les chariots, on a 2 lignes de 2 cases, pour le sens et le déplacement, d'où le /2
            int spriteHeight = spriteFullHeight/8;
            SDL_Rect spriteRect = {.x = liste->mob->offset_x*spriteWidth, .y =  (liste->mob->offset_y)*spriteHeight, .w = spriteWidth, .h = spriteHeight};
            SDL_Rect destRect = {.x = liste->mob->_x*ECHELLE, .y = (HEIGTH*32-liste->mob->_y-32-pix_defil)*ECHELLE, .w = spriteWidth*ECHELLE, .h = spriteHeight*ECHELLE};
            if (SDL_RenderCopy(renderer, spriteTexture, &spriteRect, &destRect)){
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans l'affichage: %s", SDL_GetError());
            }
            SDL_DestroyTexture(spriteTexture);
        }
        liste = liste->next;
    }
}

void display_score(Game *game, SDL_Renderer *renderer, TTF_Font* font){
    int score = game->score;
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "score: %d", score);
    SDL_Texture* texture = strToTexture(buffer, font, renderer);
    SDL_Rect rect1 = {(LENGTH*32*ECHELLE) / 2 - ((15*strlen(buffer))/2)*ECHELLE + 120*ECHELLE, 0, 15*strlen(buffer)*ECHELLE, 25*ECHELLE};
    SDL_RenderCopy(renderer, texture, NULL, &rect1);
    SDL_DestroyTexture(texture);
}

void display_all(Game *game, SDL_Renderer *renderer, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp, TTF_Font* font){
    // affiche toute la carte dans l'ordre, map puis mobs puis joueur
    // if(SDL_RenderClear(renderer)){
    //     SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur dans le fond vide : %s", SDL_GetError());
    // }
    display_map(game->carte, renderer,pix_defil, surfaceArrayMap);
    display_exp(game->exp, renderer,pix_defil, surfaceExp);
    display_mob(game->mobs, renderer,pix_defil, surfaceArrayMob);
    display_projectile(game->projectile, renderer,pix_defil, surfaceArrayProjectile);
    display_joueur(game->joueur, renderer,pix_defil, surfaceArrayPlayer);
    display_score(game, renderer, font);
    SDL_RenderPresent(renderer);
}
