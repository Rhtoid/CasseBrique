Groupe:
    MALPOT Nicolas  
    DUCLOS Elouan  

Pour compiler le CMAKE, se mettre à la racine puis:

mkdir build  
cd build  
cmake ..  
make  

Pour lancer:

./CasseBrique [<numlevel>] 
avec numlevel entre 0 et 9

Systèmes Linux Ubuntu:  
    sudo apt-get update  
    sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev  
    sudo apt-get install pkg-config  

Systèmes MacOS:  
    brew install sdl2 sdl2_image sdl2_ttf  
    brew install pkg-config  

Bonus:  
    Élargissement de la plateforme  
    Vitesse ralenti de la balle 
    Multi-balle  


Malus:  
    Rétrécissement de la plateforme  
    Vitesse accéléré de la balle  
