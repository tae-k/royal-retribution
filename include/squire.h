#ifndef SQUIRE_H
#define SQUIRE_H

#define SQUIRE_FPS              5
#define SQUIRE_MOVING_FRAMES    4
#define SQUIRE_IDLE_FRAMES      1
#define SQUIRE_R_MOVING_POS     12
#define SQUIRE_L_MOVING_POS     8
#define SQUIRE_UP_MOVING_POS    4
#define SQUIRE_DOWN_MOVING_POS  0

#include "sprite.h"
#include "constants.h"
#include "event_handler.h"

//Header file for enemy class that holds enemy position and its rect
class Squire : public Sprite {
  public:
    Squire(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int width, int height, int pos_x, int pos_y, double velocityX,
      double velocityY);

    Squire(SDL_Renderer *renderer, std::string, ErrorHandler *error_handler,
      int pos_x, int pos_y, double velocityX, double velocityY);

    virtual void update(double seconds);

    std::string dir="down";

  private:
    void idleAnimation(double);
};

#endif
