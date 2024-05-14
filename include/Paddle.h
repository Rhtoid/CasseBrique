#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <../include/SDLWrapper.h>

class Paddle {
public:
    Paddle(SDLWrapper* sdlWrapper);
    ~Paddle();

    void handleEvent(const SDL_Event& e);
    void move(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Getters and setters for paddle properties
    void setPosition(float x, float y);
    SDL_FRect getRect() const;
    void change_width_relative(float elargissement);

private:
    SDL_FRect rect;
    float velocity;
    SDL_Texture* texture;

};