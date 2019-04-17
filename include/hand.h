#ifndef HANDS_H
#define HANDS_H

#define STATIONARY_TIME	1.0
#include "enemy.h"
#include <math.h>

class Hand : public Enemy {
  public:
    Hand(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY, int health);

    virtual doubleRect getDoubleRect();
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill(); 
    virtual bool isHand() { return true; }
   
    void goTo(int x, int y);
    bool isDead() { return (hp <= 0); }
    bool returned() { return (reachOrigX && reachOrigY); } 
    bool onGround() { return (reachGroundX && reachGroundY); }

  private:
    void goBack();

    int hp = 0;
    int fade = 255;
    int origPosX = 0;
    int origPosY = 0;

    bool reachOrigX = false;
    bool reachOrigY = false;
    bool reachGroundX = false;
    bool reachGroundY = false;

    double waitingTime = 0.0;
};

#endif
