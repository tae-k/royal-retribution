#ifndef MINI_ENEMY_H
#define MINI_ENEMY_H

#define M_ENEMY_TRANSFORM_POS   52
#define M_ENEMY_D_MOVING_POS    56
#define M_ENEMY_L_MOVING_POS    60
#define M_ENEMY_R_MOVING_POS    64
#define M_ENEMY_U_MOVING_POS    68

#include "enemy.h"

//Header file for enemy class that holds enemy position and its rect
class Mini_Enemy : public Enemy {
  public:
    Mini_Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    std::string dir = "down";
    virtual doubleRect getDoubleRect();
    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill() { exploding = true; } 

    bool isMini() { return true; }

  private:
    bool transformed = false;
    double transformingTimer = 0;

    void transform(double);
    void attemptFollow(double);
};

#endif
