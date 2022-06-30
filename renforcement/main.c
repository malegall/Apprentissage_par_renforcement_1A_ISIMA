#include "game.h"

int main()
{
    srand(time(NULL));
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Window *window;
    SDL_Texture *texture[16], *sprite;
    SDL_Surface* image[16];

    Character_t character;
    Teleporter_t tabTeleporter[NB_TELEPORTER];
    
    StateList_t stateList[500];
    stateList[0].reward = 0;
    int hasWon = 0;

    initTeleporter(tabTeleporter);

    SDL_Rect source = {0};
	float zoom = 0.8, max, eps = 1.0, epsRandom, qTable[NB_STATE][DIRECTION], xi = 0.99, gamma = 0.9;

    initQtable(qTable);

    int running = 1, i = 0, j = 0, n =0, generation = 0, argmax, offset_w, offset_h, cptCharac = 0, cptCharacMax = 6, action = -1, 
    movement = 1, teleport = -1, hasTeleported = 0, firstTeleport = 1, nbState = 0,
    	grille [][25]={{1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1},
                       {1,2,2,4,2,2,2,2,   1,2,2,2,12,2,2,2, 1,2,2,4,2,2,2,2,1},
                       {1,2,2,2,2,14,2,2,  1,2,2,2,2,4,2,2,  1,2,2,2,2,7,2,2,1},
                       {1,2,13,2,2,2,2,2,  1,2,2,3,2,2,2,2,  1,2,2,2,2,2,2,2,1},
                       {1,2,2,2,2,2,4,2,   1,2,2,2,2,2,2,2,  1,2,2,2,2,2,2,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,2,2,2,2,4,2,  1,2,4,2,2,2,2,2,1},
                       {1,2,4,2,2,2,2,2,   1,2,2,2,2,9,2,2,  1,2,2,2,2,2,4,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,2,2,2,2,2,2,  1,2,2,2,15,2,2,2,1},
                       {1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1},

                       {1,2,2,2,4,2,2,2,   1,2,2,2,14,2,2,2, 1,2,2,11,2,2,2,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,4,2,2,2,2,2,  1,2,2,2,2,9,2,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,2,2,2,2,4,2,  1,2,2,4,2,2,2,2,1},
                       {1,12,2,2,2,11,2,2, 1,2,2,2,5,2,2,2,  1,2,2,2,2,2,2,4,1},
                       {1,2,2,2,2,2,2,2,   1,2,3,2,2,2,2,2,  1,2,2,2,2,2,2,2,1},
                       {1,2,4,2,2,2,2,4,   1,2,2,2,2,2,2,15, 1,2,4,2,2,2,2,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,2,2,4,2,2,2,  1,2,2,2,8,2,2,2,1},
                       {1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1},

                       {1,2,2,2,2,2,2,6,   1,2,2,2,4,2,2,2,  1,2,2,2,7,2,2,4,1},
                       {1,2,2,4,2,2,2,2,   1,2,10,2,2,2,4,2, 1,2,2,2,2,2,2,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,2,2,2,2,2,2,  1,2,4,2,2,2,2,2,1},
                       {1,2,2,2,2,2,2,4,   1,2,2,2,2,2,2,6,  1,2,2,2,2,2,2,10,1},
                       {1,2,2,2,8,2,2,2,   1,2,2,2,2,2,2,2,  1,13,2,2,2,2,2,2,1},
                       {1,2,2,2,2,2,2,2,   1,2,4,2,2,2,2,2,  1,2,2,2,2,2,2,2,1},
                       {1,2,2,4,2,2,2,2,   1,2,2,2,0,2,2,2,  1,2,2,2,2,4,2,2,1},
                       {1,1,1,1,1,1,1,1,   1,1,1,1,1,1,1,1,  1,1,1,1,1,1,1,1,1},
                       };

    char * link[16]={"./img/green_button06.png",
                       "./img/tile_0000.png",
                       "./img/grey_button09.png",
                       "./img/blue_button06.png",
                       "./img/tile_0043.png",
                       "./img/green_button07.png",
                       "./img/redbutton.png",
                       "./img/yellow_button06.png",
                       "./img/magenta_button.png",
                       "./img/sky_button.png",
                       "./img/olive_button.png",
                       "./img/pink_button.png",
                       "./img/purple_button.png",
                       "./img/brown_button.png",
                       "./img/red_button03.png",
                       "./img/grey_button.png"
                       };

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    /* Initialisation des variables */
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              600, 600,
                              SDL_WINDOW_RESIZABLE);
    if (window == 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //  SDL_RENDERER_SOFTWARE
    if (renderer == 0)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
    }

    sprite = IMG_LoadTexture(renderer,"./img/george.png");
    if (sprite == NULL)
    {
         fprintf(stderr, "Erreur de chargement de l'image : %s\n", SDL_GetError());
    }
    character.sprite = sprite;
    
    SDL_QueryTexture(character.sprite,NULL,NULL,&source.w, &source.h);
    offset_w = source.w/4;
    offset_h = source.h/4;  
    
    (character.position).x = 12*24-8;
    (character.position).y = 22*24+12;
    (character.position).w = offset_w*0.8;
    (character.position).h = offset_h*0.8;
    (character.state).x = 0;
    (character.state).y = 0;
    (character.state).w = offset_w;
    (character.state).h = offset_h;
    character.row = 23;
    character.column = 12;

    for(i=0;i<16;i++){
        image[i]=IMG_Load(link[i]);
        if(!image[i])
        {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
        }
        texture[i]=SDL_CreateTextureFromSurface(renderer,image[i]);
        SDL_FreeSurface(image[i]);
    }

    for (i = 0; i < 1000; i++) 
    {
      while (!hasWon && nbState < 400) 
      {
        /* Animation personnage */
        if (movement) 
        {
          if (cptCharac == 0) 
          {
            argmax = 0;
            max = maxStateQtable(qTable, stateList[nbState].state, & argmax);
            epsRandom = (float) rand() / RAND_MAX;

            if (epsRandom > eps) {
              if (max > 0) {
                action = argmax;
              } else {
                action = rand() % 4;
              }
            } else {
              action = rand() % 4;
            }
          }

          switch (action)
          {
          case UP:
            (character.state).x = 2 * offset_w;
            if (grille[character.row - 1][character.column] != 1) {
              if (cptCharac != cptCharacMax) {
                (character.position).y -= SPRITE_STEP;
                (character.state).y += offset_h;
                (character.state).y %= source.h;
                cptCharac++;
                stateList[nbState].state = 23 * (character.row - 1) + (character.column - 1);
              } else {
                character.row--;
                cptCharac = 0;
                stateList[nbState].action = action;
                switch (grille[character.row][character.column]) {
                case 5:
                  stateList[nbState + 1].reward = 10;
                  break;

                default:
                  stateList[nbState + 1].reward = 0;
                  break;
                }
                nbState++;
                movement = 0;
                action = -1;
              }
            }

            break;

          case RIGHT:
            (character.state).x = 3 * offset_w;
            if (grille[character.row][character.column + 1] != 1) {
              if (cptCharac != cptCharacMax) {
                (character.position).x += SPRITE_STEP;
                (character.state).y += offset_h;
                (character.state).y %= source.h;
                cptCharac++;
                stateList[nbState].state = 23 * (character.row - 1) + (character.column - 1);
              } else {
                character.column++;
                cptCharac = 0;
                stateList[nbState].action = action;
                switch (grille[character.row][character.column]) {
                case 5:
                  stateList[nbState + 1].reward = 10;
                  break;

                default:
                  stateList[nbState + 1].reward = 0;
                  break;
                }
                nbState++;
                movement = 0;
                action = -1;
              }
            }
            break;

          case DOWN:
            (character.state).x = 0;
            if (grille[character.row + 1][character.column] != 1) 
            {
              if (cptCharac != cptCharacMax) {
                (character.position).y += SPRITE_STEP;
                (character.state).y += offset_h;
                (character.state).y %= source.h;
                cptCharac++;
                stateList[nbState].state = 23 * (character.row - 1) + (character.column - 1);
              } else {
                character.row++;
                cptCharac = 0;
                stateList[nbState].action = action;
                switch (grille[character.row][character.column]) {
                case 5:
                  stateList[nbState + 1].reward = 10;
                  break;

                default:
                  stateList[nbState + 1].reward = 0;
                  break;
                }
                nbState++;
                movement = 0;
                action = -1;
              }
            }
            break;

          case LEFT:
            (character.state).x = offset_w;
            if (grille[character.row][character.column - 1] != 1) 
            {
              if (cptCharac != cptCharacMax) {
                (character.position).x -= SPRITE_STEP;
                (character.state).y += offset_h;
                (character.state).y %= source.h;
                cptCharac++;
                stateList[nbState].state = 23 * (character.row - 1) + (character.column - 1);
              } else {
                character.column--;
                cptCharac = 0;
                stateList[nbState].action = action;
                switch (grille[character.row][character.column]) {
                case 5:
                  stateList[nbState + 1].reward = 10;
                  break;

                default:
                  stateList[nbState + 1].reward = 0;
                  break;
                }
                nbState++;
                movement = 0;
                action = -1;
              }
            }
            break;
          }
        }

        if (!movement && grille[character.row][character.column] == 4) 
        {
          if (zoom > 0) {
            (character.state).x += offset_w; // La première vignette est en début de ligne
            (character.state).x %= source.w; // La vignette qui suit celle de fin de ligne est
            (character.position).w = offset_w * zoom; // Largeur du sprite à l'écran
            (character.position).h = offset_h * zoom; // Hauteur du sprite à l'écran 
            (character.position).x = (character.position).x + (offset_w * 0.05); // pour qu'il reste centré quand il diminue
            (character.position).y = (character.position).y + (offset_h * 0.05);
            zoom = zoom - 0.1;
          } else {
            (character.position).x = 12 * 24 - 8;
            (character.position).y = 22 * 24 + 12;
            (character.position).w = offset_w * 0.8;
            (character.position).h = offset_h * 0.8;
            (character.state).x = 0;
            (character.state).y = 0;
            (character.state).w = offset_w;
            (character.state).h = offset_h;
            character.row = 23;
            character.column = 12;
            generation++;
            zoom = 0.8;
            updateQtable(qTable, stateList, nbState, xi, gamma);
          }
        } 
        else 
        {
          teleport = hasToTeleport(tabTeleporter, character);

          if (teleport != -1 && movement == 0) {
            if (firstTeleport) {
              if (zoom > 0) {
                (character.state).x += offset_w; // La première vignette est en début de ligne
                (character.state).x %= source.w; // La vignette qui suit celle de fin de ligne est
                (character.position).w = offset_w * zoom; // Largeur du sprite à l'écran
                (character.position).h = offset_h * zoom; // Hauteur du sprite à l'écran 
                (character.position).x = (character.position).x + (offset_w * 0.05); // pour qu'il reste centré quand il diminue
                (character.position).y = (character.position).y + (offset_h * 0.05);
                zoom = zoom - 0.1;
              } else {
                (character.position).x = 24 * ((tabTeleporter[teleport].destinationColumn)) + 17;
                (character.position).y = 24 * ((tabTeleporter[teleport].destinationRow)) + 10;
                character.row = tabTeleporter[teleport].destinationRow;
                character.column = tabTeleporter[teleport].destinationColumn;
                firstTeleport = 0;
                hasTeleported = 1;
              }
            }

            if (hasTeleported) {
              if (zoom < 0.8) {
                zoom += 0.1;
                (character.state).x += offset_w; // La première vignette est en début de ligne
                (character.state).x %= source.w; // La vignette qui suit celle de fin de ligne est
                (character.position).w = offset_w * zoom; // Largeur du sprite à l'écran
                (character.position).h = offset_h * zoom; // Hauteur du sprite à l'écran
                (character.position).x = (character.position).x - (offset_w * 0.05); // pour qu'il reste centré quand il diminue
                (character.position).y = (character.position).y - (offset_h * 0.05);
              } else {
                hasTeleported = 0;
                firstTeleport = 1;
                movement = 1;
              }
            }
          } else {
            if (grille[character.row][character.column] == 5) {
              stateList[nbState].state = 23 * (character.row - 1) + (character.column - 1);
              //stateList[nbState].action = -1;
              nbState++;
              (character.position).x = 12 * 24 - 8;
              (character.position).y = 22 * 24 + 12;
              (character.position).w = offset_w * 0.8;
              (character.position).h = offset_h * 0.8;
              (character.state).x = 0;
              (character.state).y = 0;
              (character.state).w = offset_w;
              (character.state).h = offset_h;
              character.row = 23;
              character.column = 12;
              generation++;
              zoom = 0.8;
              updateQtable(qTable, stateList, nbState, xi, gamma);
              //printStateList(stateList,nbState);
              sauvegarder("sauvegarde.txt", qTable);
              hasWon = 1;
            } else {
              movement = 1;
            }
          }
        }

        if (nbState % 500 == 0) 
        {
          eps *= 0.99;
        }
      }
      printStateList(stateList, nbState);
      printf("hasWon:%d\n", hasWon);
      printf("nbstate:%d\n",nbState);
      printf("eps:%f\n", eps);
      printf("i:%d\n", i);
      // if (hasWon)
      // {
      //   printQtable(qTable);
      // }
      nbState = 0;
      hasWon = 0;
      movement = 1;
      printf("%d %d\n", nbState, hasWon);
    }
    
   /* for(i=0;i<15;i++){
        SDL_DestroyTexture(texture[i]);
    }
    SDL_Delay(500);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();*/
}
