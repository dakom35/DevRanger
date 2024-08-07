#include "Monster.h"

// Constructor. Initializes player attributes and sprites. 
Monster::Monster() 
{
    movements = {"IDLE"} ;
    imagesPerMovement = 1 ; // according to what I have in entityImagesFolder
    movementSpeed = 50.f ; 
    numberOfDifferentMovements = 1 ; // idle
    width = 50.f ; 
    height = 50.f ; 
    size = {width,height}; 
    shape.setSize(size); 
    initialX = 0.f ;
    initialY = 0.f ;  
    initialPos = {initialX,initialY} ; 
    pos = initialPos ;
    lastMovement = "IDLE" ; 
    entityImagesFolder = "Monster/" ; // 68x71 px
    this->initSprites(); // to call last, after all attributes are set
    
}

void Monster::move(const float& deltaTime, const float dir_x, const float dir_y)
{
    /*
    *       -1 <= dir_x <= 1
    *       -1 <= dir_y <= 1
    */
    time1 = clock.getElapsedTime();
    sf::Vector2f position = sprite.getPosition(); // = (15, 55)
    if(time1.asSeconds() > 5)
    { 

        std::cerr << "Monster.pos :" ;
        std::cerr << position.x << "," << position.y << "\n" ;
        clock.restart();
    }
    

    sprite.move(dir_x*this->movementSpeed*deltaTime,dir_y*this->movementSpeed*deltaTime);
}