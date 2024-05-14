#pragma once

#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class Brick {
public:
    static SDL_Texture *allTexture;
    static SDL_Rect brickPlace[];

    enum class Type{
        t1,
        t2,
        t3,
        b1,
        b2,
        b3,
        m1,
        m2,
        m3
    };

    Brick(SDL_Renderer* renderer,int x, int y, int width, int height, std::string brick_type);
    ~Brick();
    void render(SDL_Renderer* renderer);
    bool isDestroyed() const;
    SDL_FRect getRect() const;
    int getHealth() const;
    void hit();
    std::string getBrickType() const;

    static void startBrick(SDL_Renderer* renderer, std::string path);
    static void stopBrick();

private:
    SDL_FRect rect;
    int health;
    bool destroyed = false;
    SDL_Texture* texture = nullptr;
    std::string brick_type;
};
