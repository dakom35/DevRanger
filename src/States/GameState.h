#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"

class GameState : 
    public State
    {

        private:
            // Main and only player
            Player statePlayer ; 
            GameWorld* gameWorld; // Use a pointer to the singleton instance of GameWorld
            Monster stateMonster ;
            sf::Clock clock; // // starts the clock
            sf::Time time1,time2 ; // 
            std::vector<GameTile*> tiles ; // copy of the vector generated in GameWorld
            
        public:
            GameState(sf::RenderWindow* window, sf::View view);
            virtual ~GameState();

            void endState();
            void updateInput(const float& deltaTime);
            void updateMonsterMoveDirection(const float& deltaTime);
            void update(const float& deltaTime);
            void render(sf::RenderTarget* target = nullptr);
            void receiveMap(std::vector<GameTile*> tiles);
            bool isPlayerTouchingWall();
            bool isPlayerTouchingMonster();
            void MonsterAttacksPlayer();
    };

#endif
