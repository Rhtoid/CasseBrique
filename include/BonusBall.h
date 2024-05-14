#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/Ball.h"

class BonusBall: public Ball{
    public:
        BonusBall(SDLWrapper*wrapper, float x, float y, float vy);
        ~BonusBall();
        bool isDead();
};