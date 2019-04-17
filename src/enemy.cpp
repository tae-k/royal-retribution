/*
 * enemy.cpp
 */

#include "enemy.h"
#include "character.h"
//Definitions for the enemy class that inherits from Sprite

//constructor that takes in width and height for the rect
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  double velocity_X, double velocity_Y)
    : Sprite(renderer, filename, error_handler, width, height, pos_x,
    pos_y, true) {

    rect.w *= 2;
    rect.h *= 2;

    velocityX = velocity_X;
    velocityY = velocity_Y;

    movement = rand() % 8;
    waitingTime = 0.5 * (double)(rand() % 4);
}

//constructor with no width and height arguments
Enemy::Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, double velocity_X,
  double velocity_Y)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y, true) {

    velocityX = velocity_X;
    velocityY = velocity_Y;
}

void Enemy::freeze() {
  if (!frozen) {
    velocityX = 0;
    velocityY = 0;
    frozen = true;
  }
}

void Enemy::thaw() {
  frozen = false;
}

void Enemy::update(double seconds) {
  if (dead) return;
  if (this->exploding) die(seconds); 

  if (flipXVelocity) velocityX *= -1;
  if (flipYVelocity) velocityY *= -1;
  flipXVelocity = false;
  flipYVelocity = false;

  if (shouldFollow != nullptr)
    attemptFollow();

  if (!frozen && !following && !isMini())
    moveRandomly(seconds);

  Sprite::update(seconds);

  if (velocityX > 0) {
    dir = "right";
    Sprite::animate(seconds, ENEMY_R_MOVING_POS, ENEMY_R_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityX < 0) {
    dir = "left";
    Sprite::animate(seconds, ENEMY_L_MOVING_POS, ENEMY_L_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY > 0) {
    dir = "down";
    Sprite::animate(seconds, ENEMY_DOWN_MOVING_POS, ENEMY_DOWN_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else if (velocityY < 0) {
    dir = "up";
    Sprite::animate(seconds, ENEMY_UP_MOVING_POS, ENEMY_UP_MOVING_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else {
    if (!exploding) idleAnimation(seconds);
  }
}

void Enemy::followWhenClose(Image* sprite, double radius) {
  shouldFollow = sprite;
  radiusFollow = radius;
}

void Enemy::attemptFollow() {
  if (checkDistance(shouldFollow, radiusFollow)) following = true;

  if (following)
    followSprite();
}

bool Enemy::checkDistance(Image* sprite, double radius) {
  doubleRect spriteRect = sprite->getDoubleRect();
  doubleRect thisRect = getDoubleRect();

  spriteRect.x -= radius - spriteRect.w / 2;
  spriteRect.y -= radius - spriteRect.h / 2;
  spriteRect.w += radius * 2;
  spriteRect.h += radius * 2;

  return Util::isIntersecting(&spriteRect, &thisRect);
}

void Enemy::followSprite() {
  if (exploding || frozen) return;

  if (getDoubleRect().x + 32 <= shouldFollow->getDoubleRect().x) velocityX = 100;
  else if (getDoubleRect().x >= shouldFollow->getDoubleRect().x + 32) velocityX = -100;
  else velocityX = 0;

  if (getDoubleRect().y + 25 <= shouldFollow->getDoubleRect().y) velocityY = 100;
  else if (getDoubleRect().y >= shouldFollow->getDoubleRect().y + 25) velocityY = -100;
  else velocityY = 0;
}

void Enemy::notifyCollision(Image* img, doubleRect* intersection, bool resolved)
{

  if(img->isEnemy() || img->isSword()
     || (img->isPickup() && static_cast<Pickup*>(img)->getType() == healthNum))
  {
    return;
  }

  if (!img->isSword() && !img->isEnemy())
    Sprite::notifyCollision(img, intersection, resolved);

  if (img->isCharacter()) return;

  if (intersection->w > intersection->h) {
    flipYVelocity = true;
  } else {
    flipXVelocity = true;
  }
}

doubleRect Enemy::getDoubleRect() {
  doubleRect x;
  x.x = pos_x;
  x.y = pos_y + (rect.h / 2);
  x.w = rect.w;
  x.h = rect.h / 2;
  return x;
}

void Enemy::die(double seconds) {
  velocityY = 0;
  velocityX = 0;

  Sprite::animate(seconds, ENEMY_DIE_POS, ENEMY_DIE_POS + ENEMY_MOVING_FRAMES
    - 1, ENEMY_FPS*1.0);
  explodingTimer += seconds;

  if (explodingTimer > (ENEMY_MOVING_FRAMES/(ENEMY_FPS*1.0)))
    dead = true;
}

void Enemy::idleAnimation(double seconds) {
  int pos = -1;

  if (dir == "right") pos = ENEMY_R_MOVING_POS;
  else if (dir == "left") pos = ENEMY_L_MOVING_POS;
  else if (dir == "up") pos = ENEMY_UP_MOVING_POS;
  else if (dir == "down") pos = ENEMY_DOWN_MOVING_POS;
  else error_handler->quit(__func__, "direction not found");

  Sprite::animate(seconds, pos, pos + ENEMY_IDLE_FRAMES - 1);
}

void Enemy::moveRandomly(double seconds) {
  if (totalTime == 0) choosingDirection = true;

  totalTime += seconds;
  
  if (totalTime > waitingTime) {
    if (choosingDirection) {
      switch (movement) {
      case 0:  velocityX = 50;
               break;
      case 1:  velocityY = 50;
               break;
      case 2:  velocityX = -50;
               break;
      case 3:  velocityY = -50;
               break;
      case 4:  velocityX = 50;
               velocityY = 50;
               break;
      case 5:  velocityX = 50;
               velocityY = -50;
               break;
      case 6:  velocityX = -50;
               velocityY = 50;
               break;
      case 7:  velocityX = -50;
               velocityY = -50;
               break;
      } // end switch
      choosingDirection = false;
    }
    if (totalTime > waitingTime+1) {
      velocityX = 0;
      velocityY = 0;
      totalTime = 0;
      movement = rand() % 8;
      waitingTime = 0.5 * (double)(rand() % 4);
    }
  }
}

/* fades out a texture */
int Enemy::fadeOut(int a, double seconds, double mult) {
  newA = (double)a - speed * seconds * mult;
  if (newA > 0) {
    a = (int)newA;
    SDL_SetTextureAlphaMod(this->getTexture(), a);
  } else {
    SDL_SetTextureAlphaMod(this->getTexture(), 0);
  }
  return a;
}
