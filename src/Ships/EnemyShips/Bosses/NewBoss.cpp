#include "NewBoss.h"

NewBoss::NewBoss(int xpos, int ypos, string name) : Boss(xpos, ypos,0.0, 750, name) {
    enemyShipSprite.load("CompressedImages/Space_Station-min.png");
    enemyHitBox = new HitBox(pos.x - 95, pos.y - 30, enemyShipSprite.getWidth() * 0.35, enemyShipSprite.getHeight() * 0.35);
    
    shootingPoint = ofPoint(ofGetWidth() / 2, ofGetHeight() / 2);
    
    
}
    
void NewBoss::update(const ofPoint& playerPos) {
    // pos.x += speed;
    // enemyHitBox->box.setPosition(pos.x + 10, pos.y - 30);   // Update the hitbox to follow the boss
    // shootingPoint = ofPoint(pos.x + enemyShipSprite.getWidth() / 2, pos.y + enemyShipSprite.getHeight() / 2);;   // Update the shooting point


    // Shoot bullets in a semi-circular motion
    if (shotTimer % 5  == 0) { // Adjust the value for the frequency of shooting
        shoot();
    }

    if(enemyBullets.size() > 0) {
        // Update bullets
        for (Projectiles& bullet : enemyBullets) {
            bullet.update();
        }
    }

    // Increment shot timer
    shotTimer++;

}

void NewBoss::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    // ofRotateDeg(shipOrientation);
    enemyShipSprite.draw(ofGetWidth()/ 2 - (enemyShipSprite.getWidth()+165), ofGetHeight()/ 2 - enemyShipSprite.getHeight() , 200, 200);
    ofPopMatrix();
    
    // Uncomment this if you want to see the hitbox for the enemy
    if(showHitboxes){
        enemyHitBox->draw();
    }
    
    showBossHealth();
}


void NewBoss::shoot() {
    const int numBullets = 36; // Increase the number of bullets for a smoother spiral
    const float angleStep = 360.0 / numBullets; // Ensure a full circle is covered
    
    // Calculate the angle for each bullet, incorporating the shootAngleOffset
    float angle = bulletsShot * angleStep + shootAngleOffset;

    // Create and initialize the bullet at the boss's position with the given angle
    // Note: The angle is directly used in the Projectiles constructor
    Projectiles bullet(shootingPoint, angle);
    bullet.setSpeed(3);
    bullet.setColors(ofColor::red, ofColor::orange); // Set desired colors 
    enemyBullets.push_back(bullet);  
    ++bulletsShot;     

    // Increment the shootAngleOffset for the next call to shoot() to continue the spiral
    shootAngleOffset += 10.0; // Adjust this value for the desired spiral rotation speed

    // Optional: Reset shootAngleOffset to keep it within 0-360 degrees to avoid overflow
    if(shootAngleOffset >= 360.0) {
        shootAngleOffset -= 360.0;
    }

    if (bulletsShot >= numBullets){
        bulletsShot=0;
    }

    SoundManager::playSong("bulletSound", false);
}