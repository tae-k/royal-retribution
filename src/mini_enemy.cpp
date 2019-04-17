/*
 * mini_enemy.cpp
 */

#include "mini_enemy.h"

Mini_Enemy::Mini_Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Enemy(renderer, filename, error_handler, width, height, pos_x, pos_y,
    velocity_X, velocity_Y) {

  mini = true;
}

void Mini_Enemy::update(double seconds) {
  if (dead) return;
  if (this->exploding) die(seconds);

  if (flipXVelocity) velocityX *= -1;
  if (flipYVelocity) velocityY *= -1;
  flipXVelocity = false;
  flipYVelocity = false;

  if (shouldFollow != nullptr)
    attemptFollow(seconds);

  Sprite::update(seconds);

  if (velocityX > 0) {
    dir = "right";
    Sprite::animate(seconds, M_ENEMY_R_MOVING_POS, M_ENEMY_R_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityX < 0) {
    dir = "left";
    Sprite::animate(seconds, M_ENEMY_L_MOVING_POS, M_ENEMY_L_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY > 0) {
    dir = "down";
    Sprite::animate(seconds, M_ENEMY_D_MOVING_POS, M_ENEMY_D_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY < 0) {
    dir = "up";
    Sprite::animate(seconds, M_ENEMY_U_MOVING_POS, M_ENEMY_U_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  }

  if (transformed){
    pair->setPosition(9000, 0);
  }
}

void Mini_Enemy::attemptFollow(double seconds) {
  if (checkDistance(shouldFollow, radiusFollow)) following = true;
  if (!following) SDL_SetTextureAlphaMod(this->getTexture(), 0);

  if (transformed) followSprite();
  else if (following) transform(seconds);
}

void Mini_Enemy::transform(double seconds) {
  SDL_SetTextureAlphaMod(this->getTexture(), 255);
  pair->setCollidable(false);
  Sprite::animate(seconds, M_ENEMY_TRANSFORM_POS, M_ENEMY_TRANSFORM_POS
    + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*2.0);
  transformingTimer += seconds;

  if (transformingTimer > ENEMY_MOVING_FRAMES/(ENEMY_FPS*2.0))
    transformed = true;
}

void Mini_Enemy::notifyCollision(Image* img, doubleRect* intersection,
                                                        bool resolved)
{
  if (!img->isSword() && !img->isEnemy()) Sprite::notifyCollision(img,
                                              intersection, resolved);

  if(img->isEnemy() || img->isCharacter() || img->isSword()){
    return;
  }

  if (intersection->w > intersection->h) {
    flipYVelocity = true;
  } else {
    flipXVelocity = true;
  }
}

doubleRect Mini_Enemy::getDoubleRect() {
  doubleRect x;
  x.x = pos_x;
  x.y = pos_y + (rect.h / 2);
  x.w = rect.w;
  x.h = rect.h / 2;
  return x;
}
