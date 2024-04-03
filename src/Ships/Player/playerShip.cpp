#include "playerShip.h"


//Parametrized Constructor for the playerShip
Player::Player(int Xposition, int Yposition){
    pos.x = Xposition;
    pos.y = Yposition;
    health = 100;
    livesRemaining = 4;
    velocity.set(0, 0);
    this->shipSprite.load("ShipModels/shipModel2.png");

    this->shipOrientation = 0;
    accelerationAmount = 5.0; // Adjust the value as needed
    score = 0;
    hitBox =  HitBox(pos, shipSprite.getWidth() * 0.25, shipSprite.getHeight() * 0.15);
            
    lastShotTime = 0;
    shotCooldown = 0.1;  // Set the cooldown duration to 0.5 seconds (adjust as needed)

}

Player::Player(){
    Player(ofGetWidth()/2, ofGetHeight()/2);
}

int Player::getScore() { return score; }
void Player::setScore(int score) { this->score = score; }
int Player::getHealth(){return health; }
void Player::setHealth(int health){this-> health=health; }


void Player::draw() {
        // Draw the ship sprite with the calculated rotation
            ofPushMatrix();
            ofTranslate(this->pos.x, this->pos.y);
            ofRotateDeg(shipOrientation);

            this->shipSprite.draw(-20, -20, 45, 45);

            ofPopMatrix();
                
        // Draw the hitbox around the player ship. Uncomment this line for testing purposes
            if(showHitbox)  this->hitBox.draw();

            // i
}

void Player::update() {
    processPressedKeys();  // Process the pressed keys and calculate orientation change

    velocity.limit(maxSpeed); // Limit the velocity to the maximum speed
            
    pos += velocity; // Update position based on velocity
    this->hitBox.box.setPosition(pos.x - 15, pos.y - 15);
            
    velocity *= damping; // Apply damping to slow down the ship

    draw();  // Draw the ship


    if(found==true){  //created the sprinting
        maxSpeed = 10;
    }
    else{
        maxSpeed=5;
    }
    if(this->NewBoss_dead==true && !this->newPlayerLoaded){
        this->shipSprite.load("ShipModels/newplayer.png");
        this->newPlayerLoaded = true;
    }
    if(this->new_city_AND_gameMode==true){
        this->shipSprite.load("ShipModels/NewCityShip.png");

    }

}

void Player::shoot() { 
    // Calculate the current time
    float currentTime = ofGetElapsedTimef();

    // Check if enough time has passed since the last shot
        if (currentTime - lastShotTime >= shotCooldown) {

                Projectiles p = Projectiles(ofPoint(this->pos.x, this->pos.y), this->shipOrientation);
                p.setColors(ofColor::azure, ofColor::blueViolet);
                if(this->NewBoss_dead==true && new_city_AND_gameMode==false){
                    p.setColors(ofColor::green, ofColor::white);
                }
                if(this->fire_active==true && new_city_AND_gameMode==true){
                    p.setColors(ofColor::orange, ofColor::red);
                    p.setWidth(15);
                    p.setHeight(25);
                    p.setSpeed(80);
                    p.setDamage(700);
                }
                else if(this->ice_active==true && new_city_AND_gameMode==true ){
                    p.setColors(ofColor::blue, ofColor::white);
                    p.setWidth(10);
                    p.setHeight(33);
                    p.setSpeed(80);
                    p.setDamage(800);

                }
                 else if(this->laser_active==true && new_city_AND_gameMode==true){
                    p.setColors(ofColor::blue, ofColor::blueViolet);
                    p.setWidth(5);
                    p.setHeight(60);
                    p.setSpeed(80);
                    p.setDamage(1000);

                }


    

                this->bullets.push_back(p);

            // SoundManager::playSong("bulletSound", false);
            SoundManager::playSong("Beam", false);

            // Update the last shot time to the current time
            lastShotTime = currentTime;
        }

}

void Player::setShotCooldown(float shotCooldown) { this->shotCooldown = shotCooldown; }

void Player::removeMarkedBullets(){
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                        [](const Projectiles& p) { return p.markedForDeletion; }),
    bullets.end());
}

void Player::addPressedKey(int key) {
    key = tolower(key);

    keyMap[key] = true;
    isMoving = true; // Set the movement flag
}

void Player::processPressedKeys() {
    if(keyMap['w']) movement('w');
    if(keyMap['s']) movement('s');
    if(keyMap['d']) movement('d');
    if(keyMap['a']) movement('a');

    if(keyMap[' ']) shoot();

    
            
    if (!isMoving) {
        // Apply damping to gradually slow down the ship when no keys are being pressed
        velocity *= damping; 
    }

    if(keyMap[OF_KEY_SHIFT] == true){  //logic that if el shift bottom is press, found is true. 
        found= true;
    }
    else{
        found=false;
    }
   
}   
    

void Player::removePressedKey(int key) {
    key = tolower(key);
    keyMap[key] = false;
}

void Player::movement(char keyPressed) {
    ofVec2f acceleration;

    if (keyPressed == 'w') {
        // Apply acceleration in the direction of the ship's orientation
        acceleration.set(cos(ofDegToRad(shipOrientation)) * accelerationAmount, sin(ofDegToRad(shipOrientation)) * accelerationAmount);
        }
    if (keyPressed == 's') {
        // Apply acceleration in the opposite direction of the ship's orientation
        acceleration.set(-cos(ofDegToRad(shipOrientation)) * accelerationAmount, -sin(ofDegToRad(shipOrientation)) * accelerationAmount);
        }

    // Apply force to velocity
    velocity += acceleration;
        
    if (keyPressed == 'a') {
        // Rotate the ship counterclockwise
        shipOrientation -= rotationSpeed;
        }
    if (keyPressed == 'd') {
        // Rotate the ship clockwise
        shipOrientation += rotationSpeed;
        }
}   
//tengo que saber cuando tengo el shift precionado y cuando no, si no lo tengo presionado ----
// necesitamos dejar el key press por eso crearemos la logica en "proces press keys "