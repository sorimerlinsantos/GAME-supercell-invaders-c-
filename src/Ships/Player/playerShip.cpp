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
    if(this->NewBoss_dead==true && !this->newPlayerLoaded){ //Load the new ship 
        this->shipSprite.load("ShipModels/newplayer.png");
        this->newPlayerLoaded = true;
    }
    if(this->new_city_AND_gameMode){
        this->shipSprite.load("ShipModels/NewCityShip.png");

    }

}

void Player::shoot() {
    // Calculate the current time
    float currentTime = ofGetElapsedTimef();

    // Check if enough time has passed since the last shot
    if (currentTime - lastShotTime < shotCooldown) {
        return; // Exit early if shot cooldown is not expired
    }

    // Create a new projectile
    Projectiles p = Projectiles(ofPoint(this->pos.x, this->pos.y), this->shipOrientation);

    // Set default projectile colors
    p.setColors(ofColor::azure, ofColor::blueViolet);

    // Adjust projectile properties based on active abilities and game mode
    if (new_city_AND_gameMode) {
        if (fire_active) {
            p.setColors(ofColor::orange, ofColor::red);
            p.setWidth(15);
            p.setHeight(25);
            p.setDamage(700);
        } else if (ice_active) {
            p.setColors(ofColor::blue, ofColor::white);
            p.setWidth(10.0);
            p.setHeight(33.0);
            p.setDamage(800);
        } else if (laser_active) {
            p.setColors(ofColor::blue, ofColor::blueViolet);
            p.setWidth(5);
            p.setHeight(60);
            p.setDamage(1000);
        }
    } 
    else if (NewBoss_dead) {
        p.setColors(ofColor::green, ofColor::white);
    }

    // Add the projectile to the bullets vector
    this->bullets.push_back(p);

    // Play shooting sound effect
    SoundManager::playSong("Beam", false);

    // Update the last shot time to the current time
    lastShotTime = currentTime;
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