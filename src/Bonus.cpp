#include "../include/Bonus.h"
#include <iostream>

SDL_Texture *Bonus::allTexture;

SDL_Rect Bonus::bonusPlace[] = {
    {0, 0, 320, 320},
    {320, 0, 320, 320},
    {0, 320, 320, 320},
    {320, 320, 320, 320},
    {640, 0, 320, 320},
    {0, 0, 320, 320}
};

Bonus::Bonus(SDL_Renderer* renderer, Type type, float x, float y) : type(type) {
    std::cout << "Création bonus" << std::endl;
    rect.x = x;
    rect.y = y;
    rect.w = 30; // Width of the bonus
    rect.h = 30; // Height of the bonus
}

Bonus::~Bonus() {}

bool Bonus::move(float deltaTime){
    rect.y += 200*deltaTime;
    if(rect.y >= 600)
        return true;
    return false;
}

void Bonus::render(SDL_Renderer* renderer) {
    SDL_RenderCopyF(renderer, allTexture, &bonusPlace[static_cast<int>(type)], &rect);
}

Bonus::Type Bonus::getType() const {
    return type;
}

SDL_FRect Bonus::getRect() const {
    return rect;
}

void Bonus::startBonus(SDL_Renderer* renderer, std::string path){
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    if (!tempSurface) {
        printf("Erreur lors du chargement de l'image de la brique : %s\n", IMG_GetError());
    } else {
        allTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        if(!allTexture) {
            printf("Erreur lors de la création de la texture de la brique : %s\n", SDL_GetError());
        }
        SDL_FreeSurface(tempSurface);
    }
}

void Bonus::stopBonus(){
    SDL_DestroyTexture(allTexture);
}