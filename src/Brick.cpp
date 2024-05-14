#include "../include/Brick.h"
#include "../include/SDLWrapper.h"

SDL_Texture *Brick::allTexture;
SDL_Rect Brick::brickPlace[] = {
    {0, 0, 160, 160},
    {160, 0, 160, 160},
    {320, 0, 160, 160},
    {480, 0, 160, 160},
    {0, 160, 160, 160},
    {160, 160, 160, 160},
    {320, 160, 160, 160},
    {480, 160, 160, 160},
    {0, 320, 160, 160},
    {160, 320, 160, 160},
};

Brick::Brick(SDL_Renderer* renderer, int x, int y, int width, int height, std::string brick_type):
    rect{static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)}, brick_type(brick_type) {
    std::string chemin = "../assets/brickType/"+ brick_type + ".png";
    if(brick_type == "t3"){
        health = 3;
    }else if(brick_type == "t2"){
        health = 2;
    }else{ 
        health = 1;
    }
    SDL_Surface* tempSurface = IMG_Load(chemin.c_str());
    if (!tempSurface) {
        printf("Erreur lors du chargement de l'image de la brique : %s\n", IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        if(!texture) {
            printf("Erreur lors de la création de la texture de la brique : %s\n", SDL_GetError());
        }
        SDL_FreeSurface(tempSurface);
    }
}

Brick::~Brick() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Brick::startBrick(SDL_Renderer* renderer, std::string path){
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

void Brick::stopBrick(){
    SDL_DestroyTexture(allTexture);
}

void Brick::render(SDL_Renderer* renderer) {
    if (!this->isDestroyed() && texture) {
        SDL_RenderCopyF(renderer, texture, nullptr, &rect);
    }
}

bool Brick::isDestroyed() const {
    return health <= 0;
}

SDL_FRect Brick::getRect() const {
    return rect;
}

int Brick::getHealth() const {
    return health;
}

void Brick::hit() {
    --health;
    if (health <= 0) {
        destroyed = true;
    }
}

std::string Brick::getBrickType() const {
    return brick_type;
}