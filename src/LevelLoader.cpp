#include "../include/LevelLoader.h"
#include <fstream>
#include <sstream>



LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

bool LevelLoader::loadLevel(SDLWrapper* sdlWrapper, const std::string& filepath, std::vector<std::unique_ptr<Brick>>& bricks) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    int row = 0;
    while (std::getline(file, line)) {
        parseLine(sdlWrapper, line, row++, bricks);
    }

    return true;
}

void LevelLoader::parseLine(SDLWrapper* sdlWrapper, const std::string& line, int row, std::vector<std::unique_ptr<Brick>>& bricks) {
    int col = 0;
    int hauteur = 50;
    int largeur = 50;
    
    std::istringstream lineStream(line);
    std::string token;
    while (std::getline(lineStream, token, ';')) { // Utilise ';' comme délimiteur
        if (!token.empty() && token != " ") { // Suppose que ' ' représente un espace vide
            int x = col * largeur; // Calcule x en fonction de la colonne
            int y = row * hauteur; // Calcule y en fonction de la ligne
            // Vous devez déterminer 'brick_type' en fonction de 'token'
             // Implementez cette fonction selon vos besoins
            bricks.push_back(std::make_unique<Brick>(sdlWrapper->getRenderer(), x, y, largeur, hauteur, token));
        }
        // Logique supplémentaire pour différents types de briques ou d'éléments peut être ajoutée ici
        col++;
    }
}

// Chargement de l'image de fond
SDL_Texture* LevelLoader::loadBackgroundTexture(SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load("chemin/vers/votre/image.jpg");
    if (!tempSurface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface); // Libérez la surface temporaire
    return texture;
}

// Fonction pour dessiner le fond
void LevelLoader::drawBackground(SDL_Renderer* renderer, SDL_Texture* texture) {
    // Assumer que le fond doit couvrir tout l'écran
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}