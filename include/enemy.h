#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_FPS             5
#define ENEMY_IDLE_FRAMES     1
#define ENEMY_MOVING_FRAMES   4

#define ENEMY_DIE_POS         44
#define ENEMY_R_MOVING_POS    28
#define ENEMY_L_MOVING_POS    24
#define ENEMY_UP_MOVING_POS   20
#define ENEMY_DOWN_MOVING_POS 16

#include "sprite.h"
#include "constants.h"
#include "event_handler.h"
#include "types.h"
#include "util.h"

//Header file for enemy class that holds enemy position and its rect
class Enemy : public Sprite {
  public:
    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    Enemy(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int pos_x, int pos_y, double velocityX, double velocityY);

    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);

    void thaw();
    void freeze();
    virtual void kill() { exploding = true; } 
    bool isDead() { return dead; }
    void addFood() { food = true; }
    bool hasFood() { return food; }
    bool isEnemy() { return true; }

    std::string dir="right";

    void followWhenClose(Image* sprite, double radius);
    bool checkDistance(Image* sprite, double radius);
    void followSprite();
    virtual doubleRect getDoubleRect();
    bool attacked(){return wasAttacked;}
    void nullAttacked(){wasAttacked = false;}
  protected:
    virtual void die(double);
    void attemptFollow();

    int fadeOut(int, double, double);

    Image* shouldFollow = nullptr;

    bool mini = false;
    bool dead = false;
    bool food = false;
    bool frozen = false;
    bool exploding = false;
    bool following = false;
    bool flipXVelocity = false;
    bool flipYVelocity = false;
    bool choosingDirection = false;

    double radiusFollow;
    double explodingTimer = 0;

    int movement = 0;
    double totalTime = 0;
    double waitingTime = 0;

    bool isMini() { return mini; }
    bool wasAttacked = false;

    double newA = 0;
    double speed = 50;
    void idleAnimation(double);

  private:
    void moveRandomly(double);
};

#endif
