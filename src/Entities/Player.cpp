#include "Player.h"
#include <iterator> 

// Constructor. Initializes player attributes and sprites. 
Player::Player() 
{
    movements = {"IDLE","UP","DOWN","LEFT","RIGHT"}; 
    imagesPerMovement = 6 ; // according to what I have in Images/Player/Moves
    movementSpeed = 300.f ;
    time_to_live_fireball = 5 ; 
    maxHealth = 100 ; 
    health = maxHealth ; 
    numberOfDifferentMovements = 5 ; // idle, left,right,up,down
    width = 50.f ; 
    height = 50.f ; 
    size = {width,height}; 
    shape.setSize(size); 
    initialX = 400.f ;
    initialY = 400.f ;  
    initialPos = {initialX,initialY} ; 
    lastMovement = "IDLE" ; 
    lastMovingMovement = "UP" ; // in case a fireball is launched before moving
    entityImagesFolder = "Player/Moves/" ; // 40x64 px
    suffering = false ; 
    initSprites(); // to call last, after all attributes are set
    init_life_display();

}

Player::~Player() 
{
    // Destructor for Player, if needed
}

void Player::render(sf::RenderTarget* target){
    // BACKGROUND 
    if(suffering) target->draw(hurt);
    target->draw(this->sprite);
    for(sf::Sprite heart : this->health_hearts){
        target->draw(heart);
    }
    // FOREGROUND
    if(fireballBurning){
        target->draw(this->fireball);
    }
}

void Player::update(const float &deltaTime){

    manageFireballTrajectory(deltaTime);
    manageFireballLifetime(time_to_live_fireball);

}




/*
*   This is the function that makes the hero move. 
*   Arguments : dir_x and dir_y values are keyboard inputs
*   For correct behaviour only use this functions with   
*   dir_x belonging to [-1;1]
*   dir_y belonging to [-1;1]
*   Note: you only enter this function when Z,Q,S or D is pressed.
*/
void Player::move(const float& deltaTime, const float dir_x, const float dir_y){
    float k = 0 ; 
    if(dir_x == 0.f && dir_y == 0.f) nextSpriteIDLE();
    else nextSprite(dir_x, dir_y);

    sprite.move(dir_x*movementSpeed*deltaTime,dir_y*movementSpeed*deltaTime);
    pos = sprite.getPosition(); // update current position of player
    // update positions of hearts if hero still has some lives
    if (health_hearts.size() == 0){
        return ; 
    }
    float width_heart = health_hearts[0].getGlobalBounds().getSize().x ; 
    // dispose 3 hearts horizontally 
    for(sf::Sprite heart : health_hearts){
        float x = pos.x+(width_heart*k) + 40.f ; 
        health_hearts[k].setPosition(x,pos.y-100);
        k++;
    }
    hurt.setPosition(pos);
}

/*
*   Subroutine of Player::move()
*   Goals : - slow down the 6 frame movement cycle
*   - render movement cyclically
*/
void Player::nextSprite(float dir_x, float dir_y){
    static int cyclePos = 0 ; // position in the current movement cycle (0->5)
    static int i = 0 ; 
    int repeat = 4 ; // number of times each sprite is repeated, used to slow down the 6 frame movement cycle
    std::string movement ; // most up to date Movement request from user

    // HANDLE FRAME CYCLES //

    // Determine movement direction based on dir_x and dir_y (user input)
    if(dir_x == 0.f && dir_y == -1.f){movement = "UP";}
    else if(dir_x == -1.f && dir_y == 0.f){movement = "LEFT";}
    else if(dir_x == 1.f && dir_y == 0.f){movement = "RIGHT";}
    else if(dir_x == 0.f && dir_y == 1.f){movement = "DOWN";}
    else{
        std::cerr << "[Player::nextSprite] Unhandled dir_x,dir_y combination \n";
    }

    // hero walks in the same direction as last input
    if(lastMovement == movement){ 
        if(i < repeat) i++ ; // do nothing to slow down display a little
        else{ // take next sprite of same direction
            i = 0 ; 
            sprite = spriteMap[movement][cyclePos] ;
            sprite.setPosition(pos);
            cyclePos++;
            if(cyclePos >= 6) cyclePos = 0 ; // reset counter to not go outside the bounds of sprtUP
        }
    }
    // hero changes direction
    else{
        cyclePos = 0 ; // reset counter 
        sprite = spriteMap[movement][cyclePos] ;
        sprite.setPosition(pos);
        cyclePos++;
        lastMovement = movement;
    }
    return;
}


/*
*   Method to transition from moving mode
*   to IDLE mode. Does nothing if already 
*   IDLE.   
*/
void Player::nextSpriteIDLE()
{
    std::string movement = "IDLE";
    // if already IDLE, do nothing
    if(lastMovement == movement){return;}
    else{
        if(lastMovement =="UP") sprite = spriteMap[movement][3];
        else if(lastMovement =="DOWN") sprite = spriteMap[movement][0];
        else if(lastMovement == "LEFT") sprite = spriteMap[movement][1];
        else if(lastMovement == "RIGHT") sprite = spriteMap[movement][2];
        else{std::cerr << "[Player::nextSpriteIDLE] lastMovement unknown \n";}
        sprite.setPosition(pos);
        lastMovingMovement = lastMovement ; // save Player's direction before going "IDLE" (used for fireball)
    }
    return ;
}

/*
*   Method to initialize hearts sprites (part of HUD)
*   to IDLE mode. Does nothing if already 
*   IDLE.   
*/
void Player::init_life_display(){
    // Initialize the life display of the hero
    std::string path_to_heart_img = "./"+imagesFolder+"Player/heart.png";
    sf::Sprite heart_sprite ;
    sf::Vector2f pos_player = sprite.getPosition();
    if(!heart_texture.loadFromFile(path_to_heart_img)){
        std::cerr << "Player constructor failed loading img from : " << path_to_heart_img << "\n";
        return ;
    }
    heart_texture.setSmooth(true); // blurs edges
    heart_sprite.setTexture(heart_texture); 
    float width_heart = heart_sprite.getGlobalBounds().getSize().x ; 
    // dispose 3 hearts horizontally 
    for(float i = 0 ; i < 3 ; i++){
        heart_sprite.setPosition(pos_player.x+(width_heart*i),pos_player.y-100);
        health_hearts.push_back(heart_sprite);
    }
}

/*
*   
*   Handles how many hearts you see depending on remaining health (1,2 or 3)
*   Player dies when he has no hearts left (Game over). 
*   
*   
*/
void Player::manage_life_display(){
    float life_ratio = static_cast<float>(health)/static_cast<float>(maxHealth) ; 
    int num_hearts ; // number of hearts that should be displayed
    if(life_ratio > 0.66f) num_hearts = 3 ; 
    else if(life_ratio > 0.33f && life_ratio <0.66f) num_hearts = 2 ; 
    else if(life_ratio < 0.33f && life_ratio >0.f) num_hearts=1 ; 
    else{
        num_hearts = 0 ;
        this->alive = false ; // Game over
    }

    // Verify that num_hearts hearts are visible as they should be
    if(health_hearts.size()==num_hearts) return ; 
    else{
        health_hearts.clear();
        sf::Sprite heart_sprite ;
        heart_sprite.setTexture(heart_texture);
        float width_heart = heart_sprite.getGlobalBounds().getSize().x ;
        for(float i=0 ; i < num_hearts ; i++){
            heart_sprite.setPosition(pos.x+(width_heart*i),pos.y-100);
            health_hearts.push_back(heart_sprite);
        }
    }
}

// Initializes sprites of the hero
int Player::initSprites(){
    // Number of movements for each walk cycle and IDLE. Must all be equal to use a map
    std::string mvmtID ; 
    std::vector<std::string> fireballs_colors = {"pink","red","blue"} ; 
    int N_fireballs = 6 ; // number of sprites for each fireball 


    std::string filename;
    std::string currentPath = "./" ;
    std::string imgPath = currentPath+imagesFolder+entityImagesFolder; 
    std::string fireballPath = "./" + imagesFolder+"Player/fireball/" ;
    
    // LOAD EVERY TEXTURE OF THE ENTITY //

    for(auto move : movements){
        mvmtID = move ;
        // Initialize sf::Sprite & sf::Texture vectors
        textureMap[mvmtID] = std::vector<sf::Texture>(imagesPerMovement); 
        spriteMap[mvmtID] = std::vector<sf::Sprite>(imagesPerMovement); 
        // For the number of sprites each movement has
        for(int i = 0 ; i < imagesPerMovement ; i++){
            filename = mvmtID + +"_"+std::to_string(i+1) + ".png" ; 
            if(!textureMap[mvmtID][i].loadFromFile(imgPath+filename)){
                std::cerr << "The image"+mvmtID+"_"+ std::to_string(i) + " was not found \n" ;
                return -1 ; 
            }
            spriteMap[mvmtID][i].setTexture(textureMap[mvmtID][i]);
        }
    }
    // Load hurt sprite
    std::string blood_path = currentPath+imagesFolder+"Player/blood.png" ;
    if(!hurt_texture.loadFromFile(blood_path)){
        std::cerr << "Failed loading hurt_texture from : "<< blood_path << "\n" ; 
        return -1 ; 
    }
    hurt.setTexture(hurt_texture);

    // FIREBALLS //
    // loading fireballs textures
    for(std::vector<std::string>::iterator color = fireballs_colors.begin() ; color != fireballs_colors.end() ; color++){
        std::vector<sf::Texture> texture_vector ;
        for(int i = 0 ; i < N_fireballs ; i++){
            sf::Texture fireballTexture ;
            std::string path = fireballPath+*color+"/"+std::to_string(i+1)+".png" ;
            if(!fireballTexture.loadFromFile(path)){
                std::cerr << "Failed loading texture for fireball #"<<i<<" from: "<<path<< "\n" ;
                return -1 ; 
            }
            texture_vector.push_back(fireballTexture);
        }
        this->fireball_color_textures[*color] = texture_vector ;
    }
    // loading fireballs sprites
    for(std::vector<std::string>::iterator color = fireballs_colors.begin() ; color != fireballs_colors.end() ; color++){
        std::vector<sf::Sprite> sprite_vector ; 
        for(int i = 0 ; i < N_fireballs ; i++){
            sf::Sprite fireballSprite ;
            fireballSprite.setTexture(fireball_color_textures[*color][i]) ;
            fireballSprite.setPosition(initialPos);
            sprite_vector.push_back(fireballSprite);
        }
        this->fireball_color_sprites[*color] = sprite_vector ;
        
    }
    this->fireball=fireball_color_sprites["pink"][0] ;

    // make entity start IDLE facing the user
    sprite = spriteMap["IDLE"][0] ; // IDLE facing down
    std::cerr << "[INFO] Entity::initSprites() achieved" << "\n" ; 
    sprite.setPosition(initialPos);
    return 0 ;
}

/*
*   This method allows the hero to throw a fireball.
*   Only one fireball can exist at a time.
*   Fireball can go LEFT,RIGHT,UP or DOWN
*   Player can throw another fireball as soon as
*   the previous one is extinguished.
*   Fireballs extinguish after 5s
*
*/
void Player::attack(){

    fireballBurning = true ; 
    dirFireball = lastMovingMovement ; // direction of player
    fireball.setPosition(pos); // initiate fireball at current player's position
}

void Player::manageFireballLifetime(float seconds_to_live){
    // Makes fireball burn for seconds_to_live seconds 
    // Must be called every frame
    static sf::Clock clock ;
    sf::Time time ; 
    if(!fireballBurning){ 
        // Fireball shouldn't burn, nothing to do here
        clock.restart();
    }  
    else{
        time = clock.getElapsedTime(); // first value = time for 1 frame (not 0)
        if(time.asSeconds() > seconds_to_live){
            fireballBurning = false ;
            clock.restart(); // reset clock 
        }
    }
}

void Player::manageFireballTrajectory(float deltaTime){
    if(!fireballBurning) return ; // nothing to do here
    if(dirFireball=="UP"){
        this->fireball.move(0.f,-1.f*this->movementSpeed*deltaTime);
    }
    else if(dirFireball=="DOWN"){
        this->fireball.move(0.f,1.f*this->movementSpeed*deltaTime);
    }
    else if(dirFireball=="LEFT"){
        this->fireball.move(-1.f*this->movementSpeed*deltaTime,0.f);
    }
    else if(dirFireball=="RIGHT"){
        this->fireball.move(1.f*this->movementSpeed*deltaTime,0.f);
    }
}