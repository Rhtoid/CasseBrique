#pragma once
#include "../include/SDLWrapper.h"

#include "../include/Paddle.h"
#include "../include/Ball.h"
#include "../include/Brick.h"
#include "../include/Bonus.h"
#include "../include/LevelLoader.h"
#include "../include/BonusBall.h"

#include <memory>
#include <vector>
#include <set>

class Game {
public:
    Game(SDLWrapper* sdlWrapper);
    ~Game();

    bool init(int num_level);
    void handleEvent(const SDL_Event& e);
    void update();
    void render();
    void close();

    

    int collisions_a_la_suite=0;

    const std::vector<std::unique_ptr<Brick>>& get_bricks() const;

    bool PlayLevel(int num_level);



private:
    SDLWrapper* sdlWrapper; // Non-owning pointer to SDL wrapper
    std::unique_ptr<Paddle> paddle;
    std::unique_ptr<Ball> ball;
    std::set<std::unique_ptr<BonusBall>> bonusesballs;
    std::vector<std::unique_ptr<Brick>> bricks;
    std::set<std::unique_ptr<Bonus>> bonuses;
    LevelLoader levelLoader;
    Uint32 lastUpdateTime;
    

    void checkCollisions(float deltaTime);
    void checkCollisionsPaddle(float deltaTime);
    void checkCollisionsBricks(float deltaTime);
    void checkCollisionsBonuses();
    void actualiseVitesse(float deltaTime);
    void effects(Bonus::Type type);
    void createBonus(std::string brick_type, float x, float y);
};