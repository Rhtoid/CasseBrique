#include "../include/SDLWrapper.h"
#include <iostream>

SDLWrapper::SDLWrapper() : window(nullptr), renderer(nullptr), backgroundTexture(nullptr), screenWidth(800), screenHeight(600), windowTitle("Casse Brique") {}

SDLWrapper::~SDLWrapper() {
    close();
}

bool SDLWrapper::init(std::string image_name) {
// Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL ne peut pas s'initialiser : %s", SDL_GetError());
        return false;
    }

    // Initialisation de SDL_image
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("SDL_image ne peut pas s'initialiser : %s", IMG_GetError());
        SDL_Quit();
        return false;
    }

    // Assurez-vous d'ajouter cette ligne pour initialiser SDL_ttf
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf ne peut pas s'initialiser : %s", TTF_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }


    font = TTF_OpenFont("../assets/fonts/Montserrat-Bold.ttf", 24);
    if (!font) {
        SDL_Log("Erreur lors du chargement de la police : %s", TTF_GetError());
        return false;
    }


    // Initialisez textTexture à nullptr
    textTexture = nullptr;
    updateText("Vies restantes : 4"); // Initialisez avec un texte par défaut

    // Création de la fenêtre
    window = SDL_CreateWindow("Déplacement de Rectangle avec Image de Fond", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0); //SDL_WINDOW_RESIZABLE
    if (!window) {
        SDL_Log("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Erreur lors de la création du renderer : %s", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Chargement de l'image de fond
    //concaténation du chemin de l'image
    std::string chemin = "../assets/images/";
    chemin += image_name;
    std::cout << "\"" << chemin << "\"" << std::endl;
    backgroundTexture = IMG_LoadTexture(renderer, chemin.c_str());
    if (!backgroundTexture) {
        SDL_Log("Erreur lors du chargement de l'image de fond : %s", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    
    return true;
}

void SDLWrapper::close() {

    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}

void SDLWrapper::clear() {
    //SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // Black background
    SDL_RenderClear(renderer);
}

void SDLWrapper::present() {
    // Afficher le texte en bas à droite
    if (textTexture) {
        SDL_Rect dstRect = {screenWidth - textWidth - 10, screenHeight - textHeight - 10, textWidth, textHeight};
        SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
    }


    SDL_RenderPresent(renderer);
}

SDL_Renderer* SDLWrapper::getRenderer() const {
    return renderer;
}

SDL_Window* SDLWrapper::getWindow() const {
    return window;
}

SDL_Texture* SDLWrapper::getBackgroundTexture() const {
    return backgroundTexture;
}

//Mettre à jour un texte en bas à gauche de l'écran indiquant le nombre de vies restantes
void SDLWrapper::updateText(const std::string& text) {
    // Libération de l'ancienne texture si elle existe
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    // Création de la nouvelle surface de texte et texture
    SDL_Color color = {255, 255, 255, 255}; // Blanc
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    textWidth = surface->w;
    textHeight = surface->h;
    SDL_FreeSurface(surface);
}

SDL_bool HasIntersectionF(const SDL_FRect * A, const SDL_FRect * B){
    if(A->x+A->w >= B->x && A->y+A->h >= B->y && B->x+B->w >= A->x && B->y+B->h >= A->y)
        return SDL_TRUE;
    return SDL_FALSE;
}