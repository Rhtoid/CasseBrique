#pragma once

#include <vector>
#include <string>
#include <iostream> 
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../include/SDLWrapper.h"
#include "../include/Brick.h"


class LevelLoader {
public:
    LevelLoader();
    ~LevelLoader();

    bool loadLevel(SDLWrapper* sdlWrapper, const std::string& filepath, std::vector<std::unique_ptr<Brick>>& bricks);
    SDL_Texture* loadBackgroundTexture(SDL_Renderer* renderer);
    void drawBackground(SDL_Renderer* renderer,SDL_Texture* texture);

private:
    void parseLine(SDLWrapper* sdlWrapper, const std::string& line, int row, std::vector<std::unique_ptr<Brick>>& bricks);
};
