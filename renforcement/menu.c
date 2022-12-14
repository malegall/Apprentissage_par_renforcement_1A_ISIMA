#include "game.h"



void menu()
{
    SDL_Window* window; // fenetre de jeu
    SDL_Renderer* renderer; 
    SDL_Texture *texture_mode_normal, *texture_titre, *texture_mode_ia, *sprite_georges, *bg;
    int running=1, offset_x=0,offset_y=0, direction=RIGHT, cpt_x=0, cpt_xmax=62, cpt_y=0, cpt_ymax=43;
    SDL_Event event;
	float zoom = 1.5;
    Character_t character;
	SDL_Rect rect_mode_normal, rect_titre, rect_mode_ia, source = {0}, window_dimensions = {0};
    TTF_Font *font_titre, *font_mode;

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        // return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Teleporting Georges", 
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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

        // faire ce qu'il faut pour quitter proprement
    }
    TTF_Init();
    font_mode = TTF_OpenFont("./font/04B_30__.TTF", 20);
    if (font_mode == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
    font_titre = TTF_OpenFont("./font/04B_30__.TTF", 28);

    bg= IMG_LoadTexture(renderer,"./img/game-modified.png");
    if (bg == NULL) 
        fprintf(stderr, "Erreur d'initialisation de la texture : %s\n", SDL_GetError());  

    display_background(bg,window,renderer);
    
    sprite_georges = IMG_LoadTexture(renderer,"./img/george.png");
	if (sprite_georges == NULL)
	{
		 fprintf(stderr, "Erreur de chargement de l'image : %s\n", SDL_GetError());
	}
    SDL_GetWindowSize(window,&window_dimensions.w,&window_dimensions.h);
    SDL_QueryTexture(sprite_georges,NULL,NULL,&source.w, &source.h);

    offset_x = source.w/4;
    offset_y = source.h/4;				   // Je n'ai qu'une seule ligne dans mon sprite

    (character.state).x = 3*offset_x;                          // La premi??re vignette est en d??but de ligne
    (character.state).y = 0;                    // Une seule ligne
    (character.state).w = offset_x;                    // Largeur de la vignette
    (character.state).h = offset_y;  

    (character.position).w = offset_x * zoom;       // Largeur du sprite ?? l'??cran
    (character.position).h = offset_y * zoom;       // Hauteur du sprite ?? l'??cran
    (character.position).x = 115;
    (character.position).y = 180;


    while(running)
    {
        switch(direction){
            case UP:
                (character.state).x = 2*offset_x;
                if(cpt_y<cpt_ymax){
                    (character.position).y -=5;
                    (character.state).y += offset_y;
                    (character.state).y %= source.h;
                    cpt_y++;
                }
                else{
                    direction=RIGHT;
                    cpt_y=0;
                    (character.state).y = 0;
                }
            break;
            case DOWN:
                (character.state).x = 0;
                if(cpt_y<cpt_ymax){
                    (character.position).y+=5;
                    (character.state).y += offset_y;
                    (character.state).y %= source.h;
                    cpt_y++;
                }
                else{
                    direction=LEFT;
                    cpt_y=0;
                    (character.state).y = 0;
                }
                
            break;
            case LEFT:
                (character.state).x = offset_x;
                if(cpt_x<cpt_xmax){
                    (character.position).x-=5;
                    (character.state).y += offset_y;
                    (character.state).y %= source.h;
                    cpt_x++;
                }
                else{
                    direction=UP;
                    cpt_x=0;
                    (character.state).y = 0;
                }
            break;
            case RIGHT:
                (character.state).x = 3*offset_x;
                if(cpt_x<cpt_xmax){
                    (character.position).x+=5;
                    (character.state).y += offset_y;
                    (character.state).y %= source.h;
                    cpt_x++;
                }
                else{
                    direction=DOWN;
                    cpt_x=0;
                    (character.state).y = 0;
                }
            break;
        }
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_WINDOWEVENT:

                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        break;
                    
                    default:
                        break;      
                }
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                default:
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
              if (   event.button.y > 255
                  && event.button.y <= 255+rect_mode_normal.h   
                  && event.button.x > 180
                  && event.button.x <= 180+rect_mode_normal.w)
                  {

                    SDL_DestroyTexture(sprite_georges);
                    SDL_DestroyTexture(texture_mode_normal);
                    SDL_DestroyTexture(texture_titre);
                    SDL_DestroyTexture(texture_mode_ia);
                    SDL_DestroyTexture(bg);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);

                    game_normal(); 
                    running=0;
                        
                  }
                  else if ( event.button.y > 350
                  && event.button.y <= 350+rect_mode_normal.h   
                  && event.button.x > 210
                  && event.button.x <= 210+rect_mode_normal.w)
                  {
                    SDL_DestroyTexture(sprite_georges);
                    SDL_DestroyTexture(texture_mode_normal);
                    SDL_DestroyTexture(texture_titre);
                    SDL_DestroyTexture(texture_mode_ia);
                    SDL_DestroyTexture(bg);
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);

                    game_ia(); 
                    running=0;
                  }

            break;
            case SDL_QUIT:
                running = 0;
            break;
            }
        }
        
        SDL_RenderClear(renderer);
        display_background(bg,window,renderer);
        SDL_RenderCopy(renderer, sprite_georges, &(character.state), &(character.position));  
        get_text(renderer, 40, 50, "TELEPORTING GEORGES",  font_titre, &texture_titre, &rect_titre);
        get_text(renderer, 190, 255, "Mode normal",  font_mode, &texture_mode_normal, &rect_mode_normal);
        get_text(renderer, 225, 350, "Mode IA",  font_mode, &texture_mode_ia, &rect_mode_ia);
        SDL_RenderCopy(renderer, texture_mode_normal, NULL, &rect_mode_normal);
        SDL_RenderCopy(renderer, texture_titre, NULL, &rect_titre);
        SDL_RenderCopy(renderer, texture_mode_ia, NULL, &rect_mode_ia);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
        
    }
    
    SDL_DestroyTexture(sprite_georges);
    TTF_CloseFont(font_mode);
    TTF_CloseFont(font_titre);
    SDL_DestroyTexture(texture_mode_normal);
    SDL_DestroyTexture(texture_titre);
    SDL_DestroyTexture(texture_mode_ia);
    SDL_DestroyTexture(bg);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}