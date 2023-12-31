#include "GameTile.h"

#ifndef GAMEWORLD_H
#define GAMEWORLD_H

class GameWorld
{
    sf::Vector2i exitPos ; 
    sf::Vector2i playerPos ; 
    std::vector<sf::Vector2i> enemyPositions ;
    void setUpInitialState();
    void setUpEnemyPositions();
    void setUpTiles();



    public : 
        std::vector<std::vector<GameTile*>> tiles ; 
        sf::Vector2i gridSize ; // x = width , y = length 
        GameWorld();

};

#endif