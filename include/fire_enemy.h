#ifndef FIRE_ENEMY_H
#define FIRE_ENEMY_H

#include "enemy.h"
#include "projectile.h"

//Header file for enemy class that holds enemy position and its rect
class Fire_Enemy : public Enemy {
  public:
    Fire_Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y);

    std::string dir = "down";
    virtual void update(double seconds);
    virtual void kill() { exploding = true; } 

    bool isMini() { return false; }
    void setFireball(Projectile* p){ fireball = p;}

  private:
    Projectile* fireball = nullptr;
    double targetX = 0.0;
    double targetY = 0.0;
    void spitFire(double x, double y);
};

#endif
