#include "NewEnemy.h"

NewEnemy::NewEnemy(int xpos, int ypos) : EnemyShip(xpos, ypos, 1.0, 20, 75) {
    amIBoss = false;
    enemyShipSprite.load("CompressedImages/newEnemy-min.png");
    enemyHitBox = new HitBox(pos, enemyShipSprite.getWidth() * 0.15, enemyShipSprite.getHeight() * 0.15);
}


void NewEnemy::update(const ofPoint& playerPos) {
    // Calculate the angle to the player
        ofVec2f toPlayer = playerPos - pos;
        float angleToPlayer = ofRadToDeg(-atan2(toPlayer.x, toPlayer.y));

    // Update ship's orientation
        shipOrientation = angleToPlayer;

    // Move towards the player
        pos += toPlayer.getNormalized() * (speed/2);
        enemyHitBox->box.setPosition(pos.x - 30, pos.y - 30);

    this->shotTimer++;
    // Check if the ship should shoot
        if (shotTimer % 100 == 0) {
            shoot();
            shotTimer = 0; // resets the timer
        } 
}


void NewEnemy::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateDeg(shipOrientation);
    enemyShipSprite.draw(-32, -32, 60, 60);
    ofPopMatrix();
    
    if(showHitboxes){
        enemyHitBox->draw();
    }
}

ofPoint bulletPos = {20, 0}; //left - and right +
void NewEnemy::shoot() {
    // Create a new projectile and add it to the list of bullets
    Projectiles pL = Projectiles(pos - bulletPos, shipOrientation + 90); 
    Projectiles pR = Projectiles(pos + bulletPos, shipOrientation + 90);
    pL.setColors(ofColor::red, ofColor::orange);
    pR.setColors(ofColor::red, ofColor::orange);
    enemyBullets.push_back(pL);
    enemyBullets.push_back(pR);
    SoundManager::playSong("bulletSound", false);
}