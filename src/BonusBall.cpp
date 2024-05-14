#include "../include/BonusBall.h"
#include <iostream>


BonusBall::BonusBall(SDLWrapper* wrapper, float x, float y, float vy): Ball(wrapper){
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    std::string imagePath = "../assets/images/snowball.png";
    SDL_Surface* tempSurface = IMG_Load(imagePath.c_str());
    if (!tempSurface) {
        std::cerr << "Erreur lors du chargement de l'image de la balle : " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(sdlWrapper->getRenderer(), tempSurface);
        if (!texture) {
            std::cerr << "Erreur lors de la création de la texture de la balle : " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(tempSurface);
    }
    setPosition(x, y);
    setVelocity(0, vy);
    lifes = 1;
}

BonusBall::~BonusBall(){
    std::cout << "Bonus Ball détruite" << std::endl;
}

bool BonusBall::isDead(){
    return lifes<=0;
}