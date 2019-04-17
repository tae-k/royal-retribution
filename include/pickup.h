#ifndef PICKUP_H
#define PICKUP_H

#include "sprite.h"
enum item_type {keyNum, coinNum, foodNum, healthNum};

class Pickup : public Sprite {
  public:
    Pickup(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, bool collidable, bool powerup_p, int theType);

    Pickup(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int pos_x, int pos_y, bool collidable, bool powerup, int theType);

    void pickUp();
    void activate();
    void eat() { eaten = true; }
    bool isPowerup() { return powerup; }
    bool isPickedUp() { return pickedUp; }
    bool isActivated(){ return activated;}

    void onPickUp(int);
    int getType(){ return type; }

    void updateList();
    virtual void update(double seconds);
    virtual bool isPickup(){ return true; }

    static int inventoryPos;

  private:
    bool eaten = false;
    bool powerup = false;
    bool pickedUp = false;
    bool activated = false;
    int type = keyNum;
};

#endif
