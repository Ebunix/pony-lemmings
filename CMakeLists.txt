cmake_minimum_required( VERSION 3.12 )

project( ponies VERSION 0.1 )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED True )

add_executable( ponies 
    src/Sprites/gifdec.c
    src/Submodules/SDL_FontCache/SDL_FontCache.c

    src/Sprites/SpriteAtlas.cpp
    src/Sprites/SpriteRegistry.cpp

    src/main.cpp
    src/Window.cpp
    src/Game.cpp
    src/AssetLoader.cpp
    src/Input.cpp

    src/UI/UIElement.cpp
    src/UI/UIAbilityButton.cpp
    src/UI/UIText.cpp

    src/Level/Level.cpp
    src/Level/LevelHitmap.cpp

    src/Scenes/GameScene.cpp
    src/Scenes/LevelScene.cpp

    src/Mechanics/Pony.cpp
)

find_package( SDL2 REQUIRED )
find_package( Freetype REQUIRED )

include_directories( ${SDL2_INCLUDE_DIRS} )
include_directories( ${SDL2_ttf_INCLUDE_DIRS} )
include_directories( "src/Submodules/SDL_FontCache" )

target_link_libraries( ponies ${SDL2_LIBRARIES} )
target_link_libraries( ponies "SDL2_ttf" )

