#ifndef MAIN_BOSS_H
#define MAIN_BOSS_H

#define INVINCIBILITY_TIME 4
#define MAIN_BOSS_TRANSFORM     88
#define MAIN_BOSS_IDLE     72
#define MAIN_BOSS_ANGRY     92
#define MAIN_BOSS_DEATH     84
#define MAIN_BOSS_THROWR     76
#define MAIN_BOSS_THROWL     80

#include "boss_enemy.h"
#include "map.h"
#include "projectile.h"

class MainBoss : public Boss_Enemy {
  public:
    MainBoss(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY, int hp, Map*);

    virtual void update(double seconds);
    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);
    virtual void kill(); 
    void killed(){dead = true;}
    void setClone(bool cl){ clone = cl;}
    bool isClone(){ return clone;}
    virtual bool isMainBoss() { return true; }
    std::vector< Sprite*> projectiles;
    MainBoss* clone1 = nullptr;
    MainBoss* clone2 = nullptr;
    Sprite* target = nullptr;
    Projectile* fireballs[3] = {nullptr, nullptr, nullptr};
  protected: 
    virtual void die(double);
  private:
    void gettingAngry(double);
    
    bool angry = false;
    double angryTimer = 0.0;
    bool invincible = false;
    double invincibilityTimer = 0.0;
    bool clone = false;
    bool positionChosen = false;
    void choosePositions();
    Map* map;
    bool chase = false;
    void firstPhase(double);
    void secondPhase(double);
    void thirdPhase(double);
    double targetX  = 0.0;
    double targetY  = 0.0;
    double targetX2  = 0.0;
    double targetY2  = 0.0;
    double targetX3  = 0.0;
    double targetY3  = 0.0;

};

#endif
