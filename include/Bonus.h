#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

class Bonus {
public:
    static SDL_Texture *allTexture;
    static SDL_Rect bonusPlace[];

    enum class Type {
        DownPaddleSize = 0,
        FastBall = 1,
        SlowBall = 2,
        UpPaddleSize = 3,
        MultiBall = 4,
        Nothing = 5
    };

    Bonus(SDL_Renderer* renderer, Type type, float x, float y);
    virtual ~Bonus();

    bool move(float deltaTime);
    void render(SDL_Renderer* renderer);

    Type getType() const;
    SDL_FRect getRect() const;
    
    static void startBonus(SDL_Renderer* renderer, std::string path);
    static void stopBonus();
private:
    Type type;
    SDL_FRect rect;
};