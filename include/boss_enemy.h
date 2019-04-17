#ifndef BOSS_ENEMY_H
#define BOSS_ENEMY_H

#include "enemy.h"

class Boss_Enemy : public Enemy {
  public:
    Boss_Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY, int health);

    virtual doubleRect getDoubleRect();
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill() { hp--; } 
    int changePhase();
    int getHp(){ return hp;}
  protected:
    int hp = 50;
    int phase = 0;

};

#endif
