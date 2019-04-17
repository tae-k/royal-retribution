/*
 * squire.cpp
 */

#include "squire.h"
#include "character.h"

//constructor that takes in width and height for the rect
Squire::Squire(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, filename, error_handler, width, height, pos_x,
    pos_y, true) {

    rect.w *= 2;
    rect.h *= 2;

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

//constructor with no width and height arguments
Squire::Squire(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, double velocity_X,
  double velocity_Y)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y, true) {

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

void Squire::update(double seconds) {
  Sprite::update(seconds);

  if (velocityX > 0) {
    dir = "right";
    Sprite::animate(seconds, SQUIRE_R_MOVING_POS, SQUIRE_R_MOVING_POS
      + SQUIRE_MOVING_FRAMES - 1, SQUIRE_FPS*speedMultiplier);
  } else if (velocityX < 0) {
    dir = "left";
    Sprite::animate(seconds, SQUIRE_L_MOVING_POS, SQUIRE_L_MOVING_POS
      + SQUIRE_MOVING_FRAMES - 1, SQUIRE_FPS*speedMultiplier);
  } else if (velocityY > 0) {
    dir = "down";
    Sprite::animate(seconds, SQUIRE_DOWN_MOVING_POS, SQUIRE_DOWN_MOVING_POS
      + SQUIRE_MOVING_FRAMES - 1, SQUIRE_FPS*speedMultiplier);
  } else if (velocityY < 0) {
    dir = "up";
    Sprite::animate(seconds, SQUIRE_UP_MOVING_POS, SQUIRE_UP_MOVING_POS
      + SQUIRE_MOVING_FRAMES - 1, SQUIRE_FPS*speedMultiplier);
  } else {
    idleAnimation(seconds);
  }
}

void Squire::idleAnimation(double seconds) {
  int pos = -1;

  if (dir == "right") pos = SQUIRE_R_MOVING_POS;
  else if (dir == "left") pos = SQUIRE_L_MOVING_POS;
  else if (dir == "up") pos = SQUIRE_UP_MOVING_POS;
  else if (dir == "down") pos = SQUIRE_DOWN_MOVING_POS;
  else error_handler->quit(__func__, "direction not found");

  Sprite::animate(seconds, pos, pos + SQUIRE_IDLE_FRAMES - 1);
}
