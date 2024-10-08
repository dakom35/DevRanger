#ifndef MONSTER_H
#define MONSTER_H

#include "Entity.h"

// Monster class is a specialization of Entity class
class Monster : public Entity {
private:
    std::string lastMovement  ; // last movement of the player ("UP","DOWN","LEFT","RIGHT","IDLE")
    
    

    



public:
    // default constructor 
    Monster();
    void move(const float& deltaTime, const float x, const float y);

};


#endif 