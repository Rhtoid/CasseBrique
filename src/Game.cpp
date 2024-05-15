#include "../include/Game.h"
#include <iostream>

#include <fstream>
#include <iostream>
#include <unistd.h>
#include <algorithm>

#include <math.h>

#include <cstdlib>

#include <SDL2/SDL.h>


Game::Game(SDLWrapper* sdlWrapper) : sdlWrapper(sdlWrapper) {
    Bonus::startBonus(sdlWrapper->getRenderer(), "../assets/bonus/allBonus.png");
    Brick::startBrick(sdlWrapper->getRenderer(), "../assets/brickType/allBlock.png");
}

Game::~Game() {
    Bonus::stopBonus();
    Brick::stopBrick();
    close();
}

bool Game::init(int num_level) {
    // Initialize game objects
    srand(0);
    paddle = std::make_unique<Paddle>(sdlWrapper);
    ball = std::make_unique<Ball>(sdlWrapper);

    // Load initial level
    std::string level_path = "../assets/levels/level" + std::to_string(num_level) + ".txt";
    if (!levelLoader.loadLevel(sdlWrapper, level_path.c_str(), bricks)) {
        std::cerr << "Failed to load the initial level." << std::endl;
        return false;
    }

    //SDL_RenderCopy(sdlWrapper->getRenderer(), backgroundTexture, NULL, NULL);
    //pour afficher le background

    lastUpdateTime = SDL_GetTicks();

    //sdlWrapper->present();

    sdlWrapper->updateText("Vies restantes: 4"); // Mettez à jour le texte avec le nombre initial de vies

    
    return true;
}

void Game::handleEvent(const SDL_Event& e) {
    // Delegate event handling to paddle
    paddle->handleEvent(e);
}

void Game::update() {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f; // Convertissez en secondes
    lastUpdateTime = currentTime;

    checkCollisions(deltaTime);
    actualiseVitesse(deltaTime);
    ball->move(deltaTime);
    int lifes = ball->hitWalls(deltaTime);
    std::string lifesstring = "Vies restantes: " + std::to_string(lifes);
    sdlWrapper->updateText(lifesstring);
    
    if(lifes == 0){
        //on a perdu
        printf("Vous avez perdu\n");
        exit(0);
    }
    for(auto bonusball = bonusesballs.begin(); bonusball != bonusesballs.end();){
        bonusball->get()->move(deltaTime);
        if(bonusball->get()->hitWalls(deltaTime) == 0){
            std::cout << "Bonus Ball perdue" << std::endl;
            bonusball = bonusesballs.erase(bonusball);
        }
        else{
            bonusball++;
        }
    }
    paddle->move(deltaTime);
    for (auto bonus = bonuses.begin(); bonus != bonuses.end();) {
        if(bonus->get()->move(deltaTime)){
            bonus = bonuses.erase(bonus);
            std::cout << "Bonus détruit" << std::endl;
        }
        else{
            bonus++;
        }
    }
    
    // Nettoyage des briques détruites
    bricks.erase(std::remove_if(bricks.begin(), bricks.end(),
                                [](const std::unique_ptr<Brick>& brick) { return brick->isDestroyed(); }),
                 bricks.end());

}

void Game::render() {

    // Render all game objects
    paddle->render(sdlWrapper->getRenderer());
    ball->render(sdlWrapper->getRenderer());
    for (auto& brick : bricks) {
        brick->render(sdlWrapper->getRenderer());
    }
    for (auto& bonus : bonuses) {
        bonus->render(sdlWrapper->getRenderer());
    }
    for (auto& bonusball : bonusesballs) {
        bonusball->render(sdlWrapper->getRenderer());
    }
    sdlWrapper->present();

}

void Game::close() {
    if (sdlWrapper->getBackgroundTexture()) {
        SDL_DestroyTexture(sdlWrapper->getBackgroundTexture());
    }
    // Cleanup game objects
    bricks.clear();
    bonuses.clear();

    sdlWrapper->close();

}

void Game::checkCollisions(float deltaTime) {
    checkCollisionsPaddle(deltaTime);
    checkCollisionsBricks(deltaTime);
    checkCollisionsBonuses();
}

void Game::checkCollisionsPaddle(float deltaTime){
    SDL_FRect paddleRect = paddle->getRect();
    ball->hitPaddle(paddleRect);
    for(auto& bonusball: bonusesballs){
        bonusball->hitPaddle(paddleRect);
    }
}

void Game::checkCollisionsBricks(float deltaTime){
    SDL_FRect ballRect = ball->getRect();
    for (auto& brick : bricks) {
        if (!brick->isDestroyed()) {
            SDL_FRect brickRect = brick->getRect();
            if (HasIntersectionF(&ballRect, &brickRect)) {
                ball->hitBrick(brickRect);
                brick->hit();
            }

            for(auto& bonusball : bonusesballs){
                SDL_FRect bonusballRect = bonusball->getRect();
                if (HasIntersectionF(&bonusballRect, &brickRect)) {
                    bonusball->hitBrick(brickRect);
                    brick->hit();
                }
            }

            if(brick->isDestroyed()){
                SDL_FRect r = brick->getRect();
                createBonus(brick->getBrickType(), r.x, r.y);
            }
        }
    }
}
void Game::checkCollisionsBonuses(){
    SDL_FRect paddleRect = paddle->getRect(); 
    for (auto bonus = bonuses.begin(); bonus != bonuses.end(); ){
        SDL_FRect bonusRect = bonus->get()->getRect();
        if(HasIntersectionF(&paddleRect, &bonusRect)){
            effects(bonus->get()->getType());
            bonus = bonuses.erase(bonus);
            std::cout << "Bonus récupéré" << std::endl;
        }
        else{
            bonus++;
        }
    }
}

void Game::actualiseVitesse(float deltaTime){
    ball->actualiseVitesse();
    for(auto& bonusball : bonusesballs){
        bonusball->actualiseVitesse();
    }
}

bool Game::PlayLevel(int level) {
    SDL_Event e;

    while (true) {
        
        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || e.type == SDL_QUIT){
                return false;
            } else {
                handleEvent(e);
            }
        }
        update();

        //afficher la vélocité de la balle
        //printf("Vélocité: x: %f y: %f\n",ball->getVelocity().x,ball->getVelocity().y);

        SDL_RenderClear(sdlWrapper->getRenderer());

        SDL_RenderCopy(sdlWrapper->getRenderer(), sdlWrapper->getBackgroundTexture(), NULL, NULL);

        render();
        
        if(bricks.size()==0){
            std::cout << "Jeu terminé" << std::endl;
            return true;
        }
    }
}




void Game::effects(Bonus::Type type){
    std::cout<<"effects est appellée et brick_type est #"<<"#"<<std::endl;
    float up_taille_paddle=1.05, down_taille_paddle = 0.952;

    float coeff_ral=0.9;
    float coeff_acc=1/coeff_ral;

    SDL_FRect rect = paddle->getRect();

    switch(type){
        case Bonus::Type::UpPaddleSize:
            paddle->change_width_relative(up_taille_paddle);
            break;
        case Bonus::Type::DownPaddleSize:
            paddle->change_width_relative(down_taille_paddle);
            break;
        case Bonus::Type::SlowBall:
            for(auto& bonusball : bonusesballs){
                bonusball->setVelocity(bonusball->getVelocity().x*coeff_ral, bonusball->getVelocity().y*coeff_ral);
            }
            ball->setVelocity(ball->getVelocity().x*coeff_ral,ball->getVelocity().y*coeff_ral);  
            break;
        case Bonus::Type::FastBall:
            for(auto& bonusball : bonusesballs){
                bonusball->setVelocity(bonusball->getVelocity().x*coeff_acc, bonusball->getVelocity().y*coeff_acc);
            }
            ball->setVelocity(ball->getVelocity().x*coeff_acc,ball->getVelocity().y*coeff_acc);  
            break;
        case Bonus::Type::MultiBall:
            std::cout << "Bonus Ball [" << rect.x+rect.w/2 << ", " << rect.y-RADIUS_BALL*1.1 << "]" << std::endl;
            bonusesballs.insert(std::make_unique<BonusBall>(sdlWrapper, rect.x+rect.w/2, rect.y-RADIUS_BALL*1.1, sqrt(ball->getVelocity().x*ball->getVelocity().x+ball->getVelocity().y*ball->getVelocity().y)));
            break;
        case Bonus::Type::Nothing:
            break;
        default:
            std::cerr << "Bonus inconnu" << std::endl;
    }    
}

void Game::createBonus(std::string brick_type, float x, float y){
    Bonus::Type type = Bonus::Type::Nothing;
    if(rand()%100 < 60){
        return;
    }
    if(brick_type == "b1"){
        type = Bonus::Type::UpPaddleSize;
    }
    else if(brick_type == "b2"){
        type = Bonus::Type::MultiBall;
    }
    else if(brick_type == "b3"){
        type = Bonus::Type::SlowBall;
    }
    else if(brick_type == "m1"){
        type = Bonus::Type::DownPaddleSize;
    }
    else if(brick_type == "m2"){
        type = Bonus::Type::FastBall;
    }
    /*else if(brick_type == "m3"){
        type = Bonus::Type::MultiBall;
    }*/
    if(type != Bonus::Type::Nothing){
        SDL_Renderer* renderer = sdlWrapper->getRenderer();
        bonuses.insert(std::make_unique<Bonus>(renderer, type, x, y));
    }
}

const std::vector<std::unique_ptr<Brick>>& Game::get_bricks() const {
    return bricks;
}
