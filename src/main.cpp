#include "../include/SDLWrapper.h"
#include "../include/Game.h"

#include <time.h>

// Définir le nombre de FPS souhaité
const int FPS = 60;
// Calculer le délai par image en millisecondes pour atteindre le nombre de FPS souhaité
const int frameDelay = 1000 / FPS;

Uint32 frameStart;
int frameTime;


int main(int argc, char* argv[]) {
    if(argc == 1 || argc > 2){
        SDL_Log("Usage: %s <[num_level]>", argv[0]);
        return -1;
    }
    std::string image_name = "image.png";
    int num_level;
    if(argc == 2){
        num_level = atoi(argv[1]);
    }else{
        num_level = 1;
    }    
    SDLWrapper sdlWrapper; // Encapsulate SDL functionalities
    if (!sdlWrapper.init(image_name)) {
        SDL_Log("Failed to initialize SDLWrapper.");
        return -1;
    }

    

   
    Game game(&sdlWrapper); // Game instance
    while(num_level<=10){
        printf("Commencement du niveau #%d#\n",num_level);
        if (!game.init(num_level)) {
            SDL_Log("Failed to initialize game.");
            return -1;
        }
        bool gagne=game.PlayLevel(num_level);
        if(!gagne){
            printf("STOP\n");
            break;
        }else{
            num_level++;
            printf("Niveau suivant\n");
        }
    }

    // Clean up
    game.close();
    sdlWrapper.close();

    return 0;
}
