#include "../include/Paddle.h"

#include <iostream>

Paddle::Paddle(SDLWrapper* sdlWrapper) : velocity(0) {
    rect.x = 450.0; // Initial position
    rect.y = 520.0; // Assuming the bottom of the window //default 580
    rect.w = 100.0; // Width of the paddle
    rect.h = 80.0;  // Height of the paddle //default 20

    std::string imagePath = "../assets/images/paddle.png";
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
}

Paddle::~Paddle() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Paddle::handleEvent(const SDL_Event& e) {
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if(keystates[SDL_SCANCODE_LEFT]) {
        velocity = -500;
    } else if(keystates[SDL_SCANCODE_RIGHT]) {
        velocity = 500;
    } else {
        velocity = 0;
    }
    //pour la souris
    if(e.type == SDL_MOUSEMOTION) {
        rect.x = e.motion.x - rect.w / 2;
    }

    
}

void Paddle::move(float deltaTime) {
    rect.x += velocity * deltaTime;

    if(rect.x < 0) {
        rect.x = 0;
    } else if(rect.x + rect.w > 800) {
        rect.x = 800 - rect.w;
    }
}

void Paddle::render(SDL_Renderer* renderer) {
    if(texture) {
        SDL_RenderCopyF(renderer, texture, nullptr, &rect);
    }
}

void Paddle::setPosition(float x, float y) {
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
}

SDL_FRect Paddle::getRect() const {
    return rect;
}

void Paddle::change_width_relative(float elargissement){
    rect.x = rect.x - rect.w*(elargissement-1)/2;
    rect.w = rect.w * elargissement;
    printf("width : %f\n", rect.w);
}