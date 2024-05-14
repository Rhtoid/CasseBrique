cmake_minimum_required(VERSION 3.10) # ou une version plus récente si nécessaire
project(CasseBrique)

# Définir le standard C++ et s'assurer qu'il est requis
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter l'exécutable
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/Game.cpp 
    src/Ball.cpp 
    src/Paddle.cpp 
    src/Brick.cpp 
    src/Bonus.cpp 
    src/LevelLoader.cpp 
    src/SDLWrapper.cpp
    src/BonusBall.cpp
)

# Inclure les dossiers d'en-tête pour les headers du projet
include_directories(include)

# Utiliser pkg-config pour les modules SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LDFLAGS} ${SDL2IMAGE_LDFLAGS} ${SDL2TTF_LDFLAGS})


# Ajouter les drapeaux de compilation de SDL2, SDL2_image et SDL2_ttf
target_compile_options(${PROJECT_NAME} PUBLIC ${SDL2_CFLAGS_OTHER} ${SDL2IMAGE_CFLAGS_OTHER} ${SDL2TTF_CFLAGS_OTHER})
