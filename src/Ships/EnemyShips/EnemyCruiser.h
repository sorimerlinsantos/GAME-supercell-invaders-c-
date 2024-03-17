#pragma once
#include "enemy.h"


class EnemyCruiser : public EnemyShip {
    public:
        EnemyCruiser(int xpos, int ypos);

        void update(const ofPoint& playerPos) override;
        void draw() override;
        void shoot() override;

};