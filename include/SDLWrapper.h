#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> // Updated include statement
#include <SDL2/SDL_image.h>


#include <string>

class SDLWrapper {
public:
    SDLWrapper();
    ~SDLWrapper();

    bool init(std::string image_name);
    void close();

    void clear();
    void present();

    SDL_Renderer* getRenderer() const;
    SDL_Window* getWindow() const;
    SDL_Texture* getBackgroundTexture() const;
    void updateText(const std::string& text); // Mis à jour pour accepter le texte directement


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;

    int screenWidth;
    int screenHeight;
    std::string windowTitle;

    SDL_Texture* textTexture; // Texture pour le texte
    TTF_Font* font; // Police pour le texte
    int textWidth, textHeight; // Dimensions du texte


};


SDL_bool HasIntersectionF(const SDL_FRect * A, const SDL_FRect * B);