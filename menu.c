#include "menu.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Texture* strToTexture(char* str, TTF_Font* font, SDL_Renderer* renderer){
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, textColor);
    if (textSurface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Unable to render text surface! SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_FreeSurface(textSurface);
    return textTexture;
}

void displayMainMenu(int cursor, SDL_Renderer *renderer, TTF_Font* font, int width, int heigth){
    //printf("%d %d\n", width, heigth);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Surface* titleSurface = IMG_Load("sprites/titre.png");
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

    SDL_Surface* backSurface = IMG_Load("sprites/menu.png");
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);

    SDL_Texture* textTextures[] = {strToTexture("START", font, renderer), strToTexture("SELECT CHARACTER", font, renderer), strToTexture("OPTIONS", font, renderer), strToTexture("QUIT", font, renderer)};

    SDL_Rect backRect = {0, 0, width, heigth};
    SDL_RenderCopy(renderer, backTexture, NULL, &backRect);

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 150*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*5)/2, (23*8)/2, 80, (23*4)/2};

    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*50*ECHELLE)-40*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - ((23*5)/2)*ECHELLE, heigth / 2 - 20*ECHELLE, 23*5*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - ((23*8)/2)*ECHELLE, heigth / 2 +30*ECHELLE, 23*8*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - 80*ECHELLE, heigth / 2 + 80*ECHELLE, 160*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - ((23*4)/2)*ECHELLE, heigth / 2 + 130*ECHELLE, 23*4*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<4; i++){
        SDL_DestroyTexture(textTextures[i]);
    }
    SDL_FreeSurface(backSurface);
    SDL_DestroyTexture(backTexture);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(cursorSurface);
    SDL_DestroyTexture(cursorTexture);
}

void gameOverMenu(SDL_bool* main_menu, SDL_bool* quit, Game** game, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, int chara){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    while(!quit2){
        displayGameOverMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE));
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                cursor = (cursor-1)%NB_GOOPTION;
                if(cursor < 0){
                    cursor = NB_GOOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                cursor = (cursor+1)%NB_GOOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(cursor == RETRY){
                    destroy_game(*game);;
                    *game = initialise_game(chara,0);
                    *main_menu = SDL_FALSE;
                    quit2 = SDL_TRUE;
                }
                if(cursor == GOGOMAINMENU){
                    *main_menu = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
                if(cursor == GOQUIT){
                    *quit = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_m){
                *quit = SDL_TRUE;
                quit2 = SDL_TRUE;
            }
        }
    }
}

void displayGameOverMenu(int cursor, SDL_Renderer *renderer, TTF_Font* font, int width, int heigth){
    //printf("%d %d\n", width, heigth);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Surface* titleSurface = IMG_Load("sprites/game_over.png");
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);

    SDL_Texture* textTextures[] = {strToTexture("RETRY", font, renderer), strToTexture("MAIN MENU", font, renderer), strToTexture("QUIT", font, renderer)};

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 150*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*5)/2, (23*9)/2, (23*4)/2};
    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*50)*ECHELLE-20*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - ((23*5)/2)*ECHELLE, heigth / 2, (23*5)*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - ((23*9)/2)*ECHELLE, heigth / 2 + 50*ECHELLE, (23*9)*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - ((23*4)/2)*ECHELLE, heigth / 2 + 100*ECHELLE, (23*4)*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<3; i++){
        SDL_DestroyTexture(textTextures[i]);
    }
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(cursorSurface);
    SDL_DestroyTexture(cursorTexture);
}

void pauseMenu(SDL_bool* main_menu, SDL_bool* quit, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font, Game *game, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    while(!quit2){
        displayPauseMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE), game, pix_defil, surfaceArrayMap, surfaceArrayMob, surfaceArrayPlayer, surfaceArrayProjectile, surfaceExp);
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                cursor = (cursor-1)%NB_PMOPTION;
                if(cursor < 0){
                    cursor = NB_PMOPTION-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                cursor = (cursor+1)%NB_PMOPTION;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(cursor == RESUME){
                    quit2 = SDL_TRUE;
                }
                if(cursor == GOMAINMENU){
                    *main_menu = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
                if(cursor == PMOPTIONS){

                }
                if(cursor == PMQUIT){
                    *quit = SDL_TRUE;
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_m){
                *quit = SDL_TRUE;
                quit2 = SDL_TRUE;
            }
        }
    }
}

void displayPauseMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth, Game *game, int pix_defil, SDL_Surface** surfaceArrayMap, SDL_Surface** surfaceArrayMob, SDL_Surface** surfaceArrayPlayer, SDL_Surface** surfaceArrayProjectile, SDL_Surface** surfaceExp){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Texture* titleTexture = strToTexture("PAUSE", font, renderer);

    //SDL_Surface* backSurface = IMG_Load("sprites/filtrepause.png");
    //SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);

    display_map(game->carte, renderer,pix_defil, surfaceArrayMap);
    display_exp(game->exp, renderer,pix_defil, surfaceExp);
    display_mob(game->mobs, renderer,pix_defil, surfaceArrayMob);
    display_projectile(game->projectile, renderer,pix_defil, surfaceArrayProjectile);
    display_joueur(game->joueur, renderer,pix_defil, surfaceArrayPlayer);

    SDL_Texture* textTextures[] = {strToTexture("RESUME", font, renderer), strToTexture("MAIN MENU", font, renderer), strToTexture("OPTIONS", font, renderer), strToTexture("QUIT", font, renderer)};

    //SDL_Rect backRect = {0, 0, width, heigth};
    //SDL_RenderCopy(renderer, backTexture, NULL, &backRect);

    SDL_Rect titleRect = {width / 2 - 120*ECHELLE, heigth / 2 - 180*ECHELLE, 240*ECHELLE, 96*ECHELLE};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    int decals[] = {(23*6)/2, (23*9)/2, 80, (23*4)/2};

    SDL_Rect curRect = {width / 2 - (decals[cursor]+50)*ECHELLE, heigth / 2+(cursor*50*ECHELLE)-40*ECHELLE, 2*32*ECHELLE, 2*32*ECHELLE};
    SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);

    SDL_Rect rect1 = {width / 2 - ((23*6)/2)*ECHELLE, heigth / 2 - 20*ECHELLE, 23*6*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - ((23*9)/2)*ECHELLE, heigth / 2 +30*ECHELLE, 23*9*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - 80*ECHELLE, heigth / 2 + 80*ECHELLE, 160*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - ((23*4)/2)*ECHELLE, heigth / 2 + 130*ECHELLE, 23*4*ECHELLE, 32*ECHELLE};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);
    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<4; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    //SDL_DestroyTexture(screenTexture);

    //SDL_FreeSurface(backSurface);
    //SDL_DestroyTexture(backTexture);

    SDL_DestroyTexture(titleTexture);

    SDL_FreeSurface(cursorSurface);
    SDL_DestroyTexture(cursorTexture);
}

SDL_Texture* takeScreenshot(SDL_Renderer* renderer) {
    const Uint32 format = SDL_PIXELFORMAT_ARGB8888;
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format);
    SDL_RenderReadPixels(renderer, NULL, format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, "screenshot.png");

    IMG_SavePNG(surface, "screenshot.png");

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }

    SDL_FreeSurface(surface);
    return texture;
}

void displayCharaMenu(int cursor, SDL_Renderer* renderer, TTF_Font* font, int width, int heigth){
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    SDL_Surface* cursorSurface = IMG_Load("sprites/Selectionneur.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);

    SDL_Texture* titleTexture = strToTexture("CHARACTERS", font, renderer);

    SDL_Texture* textTextures[] = {strToTexture("KAIRON", font, renderer), strToTexture("SNITCH", font, renderer), strToTexture("AESIR", font, renderer), strToTexture("KABI", font, renderer), strToTexture("OOGWAAY", font, renderer), strToTexture("JOLINAR", font, renderer), strToTexture("RANTON", font, renderer), strToTexture("LEDE", font, renderer)};

    //SDL_Rect backRect = {0, 0, width, heigth};
    //SDL_RenderCopy(renderer, backTexture, NULL, &backRect);

    SDL_Rect titleRect = {width / 2 - 120, heigth / 2 - 180, 240, 96};
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    if (0<=cursor && cursor <=3){
        SDL_Rect curRect = {width / 2 - 100 - 130, heigth / 2+(cursor*50)-20, 2*32, 2*32};
        SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);
    } else if (4<=cursor && cursor<=7){
        SDL_Rect curRect = {width / 2 + 100 - 130, heigth / 2+((cursor-4)*50)-20, 2*32, 2*32};
        SDL_RenderCopy(renderer, cursorTexture, NULL, &curRect);
    }

    SDL_Rect rect1 = {width / 2 - 100 - 80, heigth / 2, 160, 32};
    SDL_RenderCopy(renderer, textTextures[0], NULL, &rect1);
    SDL_Rect rect2 = {width / 2 - 100 - 80, heigth / 2 + 50, 160, 32};
    SDL_RenderCopy(renderer, textTextures[1], NULL, &rect2);
    SDL_Rect rect3 = {width / 2 - 100 - 80, heigth / 2 + 100, 160, 32};
    SDL_RenderCopy(renderer, textTextures[2], NULL, &rect3);
    SDL_Rect rect4 = {width / 2 - 100 - 80, heigth / 2 + 150, 160, 32};
    SDL_RenderCopy(renderer, textTextures[3], NULL, &rect4);

    SDL_Rect rect5 = {width / 2 + 100 - 80, heigth / 2, 160, 32};
    SDL_RenderCopy(renderer, textTextures[4], NULL, &rect5);
    SDL_Rect rect6 = {width / 2 + 100 - 80, heigth / 2 + 50, 160, 32};
    SDL_RenderCopy(renderer, textTextures[5], NULL, &rect6);
    SDL_Rect rect7 = {width / 2 + 100 - 80, heigth / 2 + 100, 160, 32};
    SDL_RenderCopy(renderer, textTextures[6], NULL, &rect7);
    SDL_Rect rect8 = {width / 2 + 100 - 80, heigth / 2 + 150, 160, 32};
    SDL_RenderCopy(renderer, textTextures[7], NULL, &rect8);

    
    SDL_RenderPresent(renderer);
    for(int i = 0; i<8; i++){
        SDL_DestroyTexture(textTextures[i]);
    }

    //SDL_DestroyTexture(screenTexture);

    SDL_DestroyTexture(titleTexture);

    SDL_FreeSurface(cursorSurface);
    SDL_DestroyTexture(cursorTexture);
}

void charaMenu(SDL_bool* quit, int* pchara, SDL_Renderer* renderer, SDL_Event* pevent, TTF_Font* font){
    SDL_bool quit2 = SDL_FALSE;
    int cursor = 0;
    while(!quit2){
        displayCharaMenu(cursor, renderer, font, (LENGTH*32*ECHELLE), (HEIGTH*32*ECHELLE));
        SDL_PollEvent(pevent);
        if(pevent->type == SDL_QUIT){
            *quit = SDL_TRUE;
            quit2 = SDL_TRUE;
        }else if(pevent->type == SDL_KEYUP){
            if((pevent->key.keysym.sym == SDLK_z) || (pevent->key.keysym.sym == SDLK_UP)){
                cursor = (cursor-1)%NB_PERSO;
                if(cursor < 0){
                    cursor = NB_PERSO-1;
                }
            }else if((pevent->key.keysym.sym == SDLK_s) || (pevent->key.keysym.sym == SDLK_DOWN)){
                cursor = (cursor+1)%NB_PERSO;
            }else if((pevent->key.keysym.sym == SDLK_SPACE) || (pevent->key.keysym.sym == SDLK_RETURN)){
                if(cursor == KAIRON){
                    *pchara = KAIRON;
                    quit2 = SDL_TRUE;
                }if(cursor == SNITCH){
                    *pchara = SNITCH;
                    quit2 = SDL_TRUE;
                }if(cursor == AESIR){
                    *pchara = AESIR;
                    quit2 = SDL_TRUE;
                }if(cursor == KABI){
                    *pchara = KABI;
                    quit2 = SDL_TRUE;
                }if(cursor == OOGWAAY){
                    *pchara = OOGWAAY;
                    quit2 = SDL_TRUE;
                }if(cursor == JOLINAR){
                    *pchara = JOLINAR;
                    quit2 = SDL_TRUE;
                }if(cursor == RANTON){
                    *pchara = RANTON;
                    quit2 = SDL_TRUE;
                }if(cursor == LEDE){
                    *pchara = LEDE;
                    quit2 = SDL_TRUE;
                }
            }else if(pevent->key.keysym.sym == SDLK_TAB){
                quit2 = SDL_TRUE;
            }else if(pevent->key.keysym.sym == SDLK_m){
                *quit = SDL_TRUE;
                quit2 = SDL_TRUE;
            }
        }
    }
}

// int main(int argc, char* argv[]) {
//     SDL_Window* window = NULL;
//     SDL_Renderer* renderer = NULL;
//     TTF_Font* font = NULL;
//     SDL_Color textColor = {255, 255, 255, 255};
//     bool quit = false;
//     SDL_Event e;

//     // Initialize SDL
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Initialize SDL_ttf
//     if (TTF_Init() == -1) {
//         printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
//         return 1;
//     }

//     // Create window
//     window = SDL_CreateWindow("Simple SDL Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//     if (window == NULL) {
//         printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Create renderer for window
//     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (renderer == NULL) {
//         printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Load font
//     font = TTF_OpenFont("font/PixelPurl.ttf", 24); // Replace with the path to your font file
//     if (font == NULL) {
//         printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
//         return 1;
//     }

//     // Render text surface
//     SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello, SDL_ttf!", textColor);
//     if (textSurface == NULL) {
//         printf("Unable to render text surface! SDL_Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Create texture from surface
//     SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//     if (textTexture == NULL) {
//         printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Main loop flag
//     while (!quit) {
//         // Handle events on queue
//         while (SDL_PollEvent(&e) != 0) {
//             // User requests quit
//             if (e.type == SDL_QUIT) {
//                 quit = true;
//             }
//             // Handle key presses
//             else if (e.type == SDL_KEYDOWN) {
//                 switch (e.key.keysym.sym) {
//                     case SDLK_1:
//                         printf("Option 1 selected\n");
//                         break;
//                     case SDLK_2:
//                         printf("Option 2 selected\n");
//                         break;
//                     case SDLK_3:
//                         printf("Option 3 selected\n");
//                         break;
//                     case SDLK_q:
//                         quit = true;
//                         break;
//                 }
//             }
//         }

//         // Clear screen
//         SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
//         SDL_RenderClear(renderer);

//         // Render menu
//         SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//         // You can use SDL_ttf to render text, but for simplicity, we'll just draw rectangles
//         SDL_Rect rect1 = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 30};
//         SDL_RenderCopy(renderer, textTexture, NULL, &rect1);
//         SDL_Rect rect2 = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 30};
//         SDL_RenderDrawRect(renderer, &rect2);
//         SDL_Rect rect3 = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, 200, 30};
//         SDL_RenderDrawRect(renderer, &rect3);

//         // Update screen
//         SDL_RenderPresent(renderer);
//     }

//     // Destroy window
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);

//     // Quit SDL subsystems
//     SDL_Quit();

//     return 0;
// }
