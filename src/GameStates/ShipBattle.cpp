#include "ShipBattle.h"

// ====================================
// Constructor & Destructor Section
// ====================================

ShipBattle::ShipBattle() {
    this->player = new Player(ofGetWidth() / 2, ofGetHeight() / 2);
    this->playerScore = 0;
    this->killspreeTimer = 0;
    
    font.load("Fonts/Orbitron.ttf", 20, true);
    indicatorFont.load("Fonts/Orbitron.ttf", 10, true);
    backgroundImage.load("Menu_Images/BattleAreanew.jpg");
    ForceShield.load("CompressedImages/ForceShield.png");
    NewBomb.load("CompressedImages/Bomb-min.png");
    FirePower.load("CompressedImages/firePOWER.png");
    IcePower.load("CompressedImages/freezePOWER.png");
    LaserPower.load("CompressedImages/laserPOWER.png");
    NewCity.load("Menu_Images/newcity.jpg");
    Hearth.load("CompressedImages/livesRemaining.png");




        


}

// ====================================
// Core Functionality Section
// ====================================


// Update Method
void ShipBattle::update() {
    // Boss spawn logic
    if (EnemyManager::isBossSpawning()) {
        displayBossWarning = true;
        SoundManager::stopSong("battle");
    } 
    else {
        displayBossWarning = false;
    }

    // Update enemies and player
    EnemyManager::updateEnemies(player);
    this->player->processPressedKeys();
    this->player->update();
    wrapCoords(this->player->pos);

    // Update scoring and kill spree timer
    playerScore += EnemyManager::getPointsScored() * scoreMultiplier() * killSpreeMode();
    this->player->setScore(playerScore);
    killspreeTimer = EnemyManager::getKillSpreeTimer();

    // Update player projectiles
    if (!this->player->bullets.empty()) {
        updateBullets();
    }

    // State switching logic for when the player dies, to actually have more lifes
    if (player->health <= 0) {
        if(player->livesRemaining >1){
            player-> setHealth(100);
            player->livesRemaining--;
        }
        else{
            this->setNextState("GameOverState");
            SoundManager::stopSong("battle");
            EnemyManager::cleanUp();
            if(EnemyManager::getSpawningBossType() != ""){
                SoundManager::stopSong(EnemyManager::getSpawningBossType());
            }
                // Write the current score to a file
                ofstream scoreFile("currentScore.txt");
                if (scoreFile.is_open()) {
                    scoreFile << playerScore;
                    scoreFile.close();
                }
                this->setFinished(true);
    }
   
}
}

//====== Draw Method ====== 
void ShipBattle::draw() {
    ofSetBackgroundColor(ofColor::black);
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    

    //drawing New city 

    if(player->new_city_AND_gameMode==true){
        ofSetBackgroundColor(ofColor::black);
        NewCity.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    // Draw the score
    ofSetColor(ofColor::white);
    font.drawString("SCORE " + to_string(playerScore), ofGetWidth() / 2 - 50, 50);

    // Draw lives remaining
    if (player->livesRemaining == 1){
        Hearth.draw(ofGetWidth()/2, 50, 45, 45);
    }
    else if (player->livesRemaining == 2){
        Hearth.draw(ofGetWidth()/2 - 22, 50, 45, 45);
        Hearth.draw(ofGetWidth()/2 + 22, 50, 45, 45);
    }
    else if (player->livesRemaining == 3){
        Hearth.draw(ofGetWidth()/2 - 45, 50, 45, 45);
        Hearth.draw(ofGetWidth()/2 + 45, 50, 45, 45);
        Hearth.draw(ofGetWidth()/2, 50, 45, 45);
    }
    
    // font.drawString("Lives Remaining   "+to_string(lives_remaining), ofGetWidth()/2 - 50, 100);


    // Draw enemies and player
    EnemyManager::drawEnemies();
    player->draw();
    draw_bullets();
    
    
    // Draw boss warning if needed
    if (displayBossWarning) {
        // Semi-transparent red screen tint
        ofSetColor(255, 0, 0, 128);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

        // Warning text
        ofSetColor(255);
        font.drawString("BOSS APPROACHING!", ofGetWidth() / 2 - 100, ofGetHeight() / 2);
    }

    // Draw UI elements
    healthBar(player->health, 100);
    killSpreeTimer(this->killspreeTimer, 150);
    shieldBar(player->shield, 100);
    //ofDrawBitmapString(player->shield, 10, 180);
    
    //Draw a mini box for the bomb. Make sure to draw the bomb inside this box.
        ofNoFill();
        ofDrawRectangle(ofGetWidth() - 150, 30, 50, 50);
        ofFill();

    //drawing the shield 
    if(player->show_shield==true){
        ForceShield.draw(this->player->pos.x - this->player->hitBox.box.width,this->player->pos.y - this->player->hitBox.box.height,70,70);

    }
    if(player->count==1){
        NewBomb.draw(ofGetWidth() - 150, 30, 50, 50);
    }
    if(player->new_city_AND_gameMode==true){
        //1
        ofNoFill();
        ofDrawRectangle(ofGetWidth() - 250, 30, 50, 50);
        ofFill();

        //DRAWING FIRE POWER 
        FirePower.draw(ofGetWidth() - 250, 30, 50, 50);
        

        //2
        ofNoFill();
        ofDrawRectangle(ofGetWidth() - 350, 30, 50, 50);
        ofFill();

        //DRAWING THE FREEZE POWER 
        IcePower.draw(ofGetWidth() - 350, 30, 50, 50);
        
        //3
        ofNoFill();
        ofDrawRectangle(ofGetWidth() - 450, 30, 50, 50);
        ofFill();

        //DRAWING LASER POWER 
        LaserPower.draw(ofGetWidth() - 450, 30, 50, 50);


    }

}



// ====================================
// Input Handling Section
// ====================================

void ShipBattle::keyPressed(int key) {
    player->addPressedKey(key);
    
    //DEBUG KEYS - Press these keys to ease the debugging with the game
    if(key == 'k') {
        EnemyManager::toggleHitBoxes = !EnemyManager::toggleHitBoxes; 
        player->showHitbox = !player->showHitbox;
    }
    if(key == 'o')  player->health = 100;
    if(key == 'p')  playerScore += 10000;
    if(key== 'q' ){
        if(player->shield==100){
            player->current_health_for_shield= player->health; 
            player->show_shield=true;
        }
        else{
            player->show_shield=false;
        }
    };
    if(key=='e'){
        if(player->count==1)
            player->bomb_active=true;
        }
    if(key=='n'){
        player->new_city_AND_gameMode=true;

    }
    if(key=='f'){
        player->fire_active=true;
        player->ice_active=false;
        player->laser_active=false;
    }
    if(key=='i'){
        player->ice_active=true;
        player->fire_active=false;
        player->laser_active=false;

    }
    if(key=='l'){
        player->laser_active=true;
        player->ice_active=false;
        player->fire_active=false;


    }

    

}

void ShipBattle::keyReleased(int key) {
    key = tolower(key);
    this->player->removePressedKey(key);
}

void ShipBattle::mousePressed(int x, int y, int button) {
    // Implementation for mousePressed event in case you want to do something with this

}

// ====================================
// Helper Methods Section
// ====================================

void ShipBattle::wrapCoords(ofPoint &currentPos) {
    // Screen wrapping logic for player ship
    if (currentPos.x < 0.0) currentPos.x = ofGetWidth() - 10;
    if (currentPos.x >= ofGetWidth()) currentPos.x = 10;
    if (currentPos.y < 0.0) currentPos.y = ofGetHeight() - 10;
    if (currentPos.y >= ofGetHeight()) currentPos.y = 10;
}

void ShipBattle::draw_bullets() {
    // Draw all player bullets
    for (auto &bullet : this->player->bullets) {
        bullet.draw();
    }
}

void ShipBattle::updateBullets() {
    // Update logic for player bullets
    for (auto &bullet : this->player->bullets) {
        bullet.update();
    }
}

// ====================================
// UI and Feedback Methods Section
// ====================================
void ShipBattle::healthBar(int currHealth, int maxHealth) {
    indicatorFont.drawString("HEALTH", 10, 30);
    ofNoFill();
    ofDrawRectangle(10, 40, maxHealth *2, 20);
    ofFill();
    ofSetColor(ofColor::green);
    ofDrawRectangle(10, 40, currHealth *2, 20);
    ofSetColor(ofColor::white);
}

void ShipBattle::killSpreeTimer(int currTimer, int maxTimer) {
    indicatorFont.drawString("KILL SPREE", 10, 80);
    ofNoFill();
    ofDrawRectangle(10, 90, maxTimer, 10);
    ofFill();
    ofSetColor(ofColor::red);
    ofDrawRectangle(10, 90, currTimer, 10);
    ofSetColor(ofColor::white);
}
void ShipBattle::shieldBar(int theShield, int maxHealth) {
    indicatorFont.drawString("SHIELD", 10, 140);
    ofNoFill();
    ofDrawRectangle(10, 150, 100, 30);
    ofFill();
    ofSetColor(ofColor::green);
    ofDrawRectangle(10, 150, theShield, 30);
    ofSetColor(ofColor::white);
}






// ====================================
// Game Mechanics Methods Section
// ====================================

double ShipBattle::killSpreeMode() {
    // Logic for kill spree mode bonus
    return this->killspreeTimer > 0 ? 1.5 : 1.0;
}

double ShipBattle::scoreMultiplier() {
    // Logic for score multiplier based on current score
    if (playerScore >= 350) return 3.5;
    else if (playerScore >= 200) return 2.5;
    else if (playerScore >= 100) return 1.5;
    else return 1.0; // Default multiplier
}

void ShipBattle::reset(){
    setFinished(false);
    setNextState("");
}


                                        
