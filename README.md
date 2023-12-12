# DevRanger
A RPG where you progress by coding

The goal is to unlock access to the whole map by helping the goodies and defeating the baddies... with code(C,C++)


This is the command to compile and generate the executable game file : 

g++ -o DevRanger.out main.cpp Game.cpp States/GameState.cpp Entities/Entity.cpp GameWorld.cpp GameTile.cpp Entities/Player.cpp States/State.cpp -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

C++17 is required at least because of filesystem header being used
 
Credits : 
    SFML tutorial about game states : https://www.youtube.com/watch?v=WFJW9M852Ko (author : Suraj Sharma)
    SFML tutorial about maps : https://www.youtube.com/watch?v=_RLFI1D99Ow (author : Zenva)
    Sprites of the hero (opengameart.org) (author : Fry) (link : https://opengameart.org/users/fry)



Developer's journal : 

TODO : 
    1) Improve map from GameWorld  

