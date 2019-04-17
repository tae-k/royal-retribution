#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"
#include "constants.h"
#include "types.h"

/*header file for the sprite class that inherits from image and is the parent 
class of the 2 moveable object  classes character and enemy. Holds a velocity
a speed multiplier and a way to detect collisions when they occur*/
class Sprite : public Image {
  public:
    double timer = 0;

    Sprite(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, bool collidable, bool fixed = false);

    Sprite(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
    int pos_x, int pos_y, bool collidable, bool fixed = false);

    virtual void load(SDL_Texture* texture = nullptr);
    virtual void update(double seconds);
    virtual void render(Camera*, double interpol_alpha = 1);

    virtual void notifyCollision(Image*, doubleRect*, bool resolved = false);

    virtual bool isCollidable();
    virtual void setCollidable(bool);
    virtual SDL_Rect* getDestRect();

    virtual ~Sprite();

    void setSrcRect(int posX, int posY, int rectWidth, int rectHeight);
    void setPosition(double x, double y);
    void setVelocity(double x, double y);

    void animate(double, int, int, int fps = 3);
    void setFixed(bool fixed_p){ fixed = fixed_p;}
    bool isTrap(){ return trap;}
    void setTrap(bool trap_p){ trap = trap_p;}
    bool isDoor(){ return door;}
    void setDoor(bool door_p){ door = door_p;}
    bool isChest(){ return chest;}
    void setChest(bool chest_p){ chest = chest_p;}
    bool isHole(){ return hole;}
    void setHole(bool hole_p){ hole = hole_p;}
    bool isBlade(){ return blade;}
    void setBlade(bool blade_p){ blade = blade_p;}
    bool isLaser(){ return laser;}
    void setLaser(bool laser_p){ laser = laser_p;}
    char pairing = -1;
    void setPair(Sprite *pair_p){ pair = pair_p;}
    Sprite* pair = nullptr;
    bool stopped = false;
    
    virtual doubleRect getDoubleRect();
  protected:
    bool collidable = true;
    
    int currentFrame = -1;

    ErrorHandler *error_handler;
    SDL_Rect rect;
    SDL_Rect srcRect;
    SDL_Rect prevRect;
    void get_texture_size(SDL_Texture*, int*, int*);
  private:
    bool animating = false;
    bool fixed = false;
    void updateVelocities(double seconds);
    bool trap = false;
    bool door = false;
    bool chest = false;
    bool hole = false;
    bool blade = false;
    bool laser = false;
    double blade_timer = 0.0;
    double higherVel = 0.0;

    void resolveXCollision(Image*, double);
    void resolveYCollision(Image*, double);
};

#endif
