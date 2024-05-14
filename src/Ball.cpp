#include "../include/Ball.h"

Ball::Ball(SDLWrapper* sdlWrapper) : radius(RADIUS_BALL), texture(nullptr), sdlWrapper(sdlWrapper), lifes(4){
    // Initial position and velocity
    setPosition(400, 300); //originally 400, 300 ; 250, 300 pour tests
    setVelocity(-10, 200); //originally -10, 200 ; -20, -20 pour tests


    std::string imagePath = "../assets/images/ball.png";
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


Ball::~Ball() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Ball::move(float deltaTime) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Ball::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_Rect destRect;
        destRect.x = static_cast<int>(position.x - radius); // Ajustez ces valeurs selon la logique de positionnement
        destRect.y = static_cast<int>(position.y - radius); // et la taille de votre texture.
        destRect.w = radius * 2;
        destRect.h = radius * 2;
        
        // Render the texture
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }


}

void Ball::setPosition(float x, float y) {
    position.x = static_cast<int>(x);
    position.y = static_cast<int>(y);
}

SDL_FRect Ball::getRect() const {
    return {position.x - radius, position.y - radius, 2.0f * radius, 2.0f * radius};
}

void Ball::setVelocity(float vx, float vy) {
    velocity.x = static_cast<float>(vx);
    velocity.y = static_cast<float>(vy);
}

SDL_FPoint Ball::getVelocity() const {
    return velocity;
}

SDL_FPoint Ball::getPosition() const {
    return position;
}

int Ball::hitWalls(float deltaTime){
    // Collision detection with window boundaries
    if (position.x - radius < 0 || position.x + radius > 800) { // Assuming window width of 800 for simplicity
        velocity.x = -velocity.x; // Bounce back horizontally
        //mettre à jour la position TRES IMPORTANT
        position.x += velocity.x * deltaTime;
    }
    if (position.y - radius < 0) { // Assuming window height of 600 for simplicity
        velocity.y = -velocity.y; // Bounce back vertically
        //mettre à jour la position TRES IMPORTANT
        position.y += velocity.y * deltaTime;
    }
    if ( position.y + radius > 600) { //On a débordé en bas
        //on a perdu...A CONTINUER
        lifes--;

        //rencentrer la balle avec une velocité pointant vers le haut
        position.x = 400;
        position.y = 300;
        velocity.x = -10;
        velocity.y = -200;
    }
    return lifes;
}

void Ball::hitBrick(const SDL_FRect &brickRect){
    SDL_FRect ballRect = getRect();

    float ballBottom = ballRect.y + ballRect.h;
    float brickTop = brickRect.y;
    float ballTop = ballRect.y;
    float brickBottom = brickRect.y + brickRect.h;
    float ballRight = ballRect.x + ballRect.w;
    float brickLeft = brickRect.x;
    float ballLeft = ballRect.x;
    float brickRight = brickRect.x + brickRect.w;

    float timeVerticale = 0, timeHorizontale = 0;
    if(ballTop<=brickBottom || ballBottom>=brickTop){
        timeVerticale = std::min(brickBottom-ballTop, ballBottom-brickTop)/std::abs(velocity.y);
    }
    if(ballRight>=brickLeft || ballLeft <= brickRight){
        timeHorizontale = std::min(ballRight-brickLeft, brickRight-ballLeft)/std::abs(velocity.x);
    }
    if(timeVerticale<=timeHorizontale){
        move(-timeVerticale);
        dirChoc[0] = true;
        //setVelocity(velocity.x, -velocity.y);
        
    }
    else{
        move(-timeHorizontale);
        dirChoc[1] = true;
        //setVelocity(-velocity.x, velocity.y);
    }
    std::cout << "Time Verticale: " << timeVerticale << " Time Horizontale: " << timeHorizontale << std::endl;
}

void Ball::hitPaddle(const SDL_FRect &paddleRect){
    SDL_FRect ballRect = getRect();
    if (HasIntersectionF(&ballRect, &paddleRect)) {
        printf("Collision balle-paddle\n");

        //si la balle touche un côté du paddle, on la fait rebondir. ABANDONNÉ
        
        //s'assurer que c'est le haut du paddle qui touche la balle

        // Calculez le point central de la balle
        float ballCenterX = ballRect.x + ballRect.w / 2.0f;

        // Calculez le point central du paddle
        float paddleCenterX = paddleRect.x + paddleRect.w / 2.0f;

        // Calculez la différence en X entre le centre de la balle et le centre du paddle
        float diffX = ballCenterX - paddleCenterX;

        float norme_v= sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
        printf("norme_v (doit rester inchangée): %f\n",norme_v);

        int mult;
        if(diffX<0){mult=-1;}
        else{mult=1;}

        float angle_deviation_vx;
        float angle_limite=M_PI/4;
        float coeff_dir=-(M_PI/2-angle_limite)/(paddleRect.w/2);

        if(diffX<0){
            angle_deviation_vx=-coeff_dir*diffX+M_PI/2;
        }else{
            angle_deviation_vx=coeff_dir*diffX+M_PI/2;
            }

        float norme_deviation_vx=norme_v*cos(angle_deviation_vx);
        float norme_deviation_vy=norme_v*sin(angle_deviation_vx);

        //afficher les coordonnées de la ball
        printf(".x: %f .y: %f\n",ballRect.x, ballRect.y);


        velocity.x = mult*norme_deviation_vx,
        velocity.y = -norme_deviation_vy;
        //déplacer la balle

        /* Pas nécessaire à priori
        SDL_FPoint ballPosition = ball->getPosition();
        ball->setPosition(ballPosition.x + ball->getVelocity().x * deltaTime, ballPosition.y + ball->getVelocity().y * deltaTime);
        */
        
    }
}

void Ball::actualiseVitesse(){
    if(dirChoc[0]){
        setVelocity(velocity.x, -velocity.y);
    }
    if(dirChoc[1]){
        setVelocity(-velocity.x, velocity.y);
    }
    for(bool& i : dirChoc){
        i = false;
    }
}