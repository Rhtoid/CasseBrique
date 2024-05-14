#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/SDLWrapper.h"
#include <iostream>

#define RADIUS_BALL 10

class Ball {
public:
    Ball(SDLWrapper*);
    ~Ball();

    void move(float deltaTime);
    void render(SDL_Renderer* renderer);

    // Getters and setters for ball properties
    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    SDL_FRect getRect() const;
    SDL_FPoint getVelocity() const;
    SDL_FPoint getPosition() const;

    int hitWalls(float deltaTime);
    void hitBrick(const SDL_FRect &brickRect);
    void hitPaddle(const SDL_FRect &paddleRect);
    void actualiseVitesse();

protected:
    SDL_Texture* texture;
    int lifes;
    SDLWrapper* sdlWrapper;
    
private:
    SDL_FPoint position;
    SDL_FPoint velocity;
    int radius;
    bool dirChoc[2];
};