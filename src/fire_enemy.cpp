/*
 * fire_enemy.cpp
 */

#include "fire_enemy.h"

Fire_Enemy::Fire_Enemy(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y)
    : Enemy(renderer, filename, error_handler, width, height, pos_x, pos_y,
    0, 0) {

}

void Fire_Enemy::update(double seconds) {
  if (dead) return;
  if (this->exploding) die(seconds);

  SDL_SetTextureColorMod(getTexture(), 255,0,0);
  if(fireball != nullptr){
    SDL_SetTextureColorMod(fireball->getTexture(), 255,0,0);
  }

  velocityY = 0;
  velocityX = 0;

  if (shouldFollow != nullptr){
    if(abs(shouldFollow->getDoubleRect().x - getDoubleRect().x) > abs(shouldFollow
      ->getDoubleRect().y - getDoubleRect().y)){
      if((shouldFollow->getDoubleRect().x - getDoubleRect().x) > 0){dir = "right";}
      if((shouldFollow->getDoubleRect().x - getDoubleRect().x) < 0){dir = "left";}
    } else {
      if((shouldFollow->getDoubleRect().y - getDoubleRect().y) > 0){dir = "down";}
      if((shouldFollow->getDoubleRect().y - getDoubleRect().y) < 0){dir = "up";}

    }
  }

  Sprite::update(seconds);

  fireball->frozen = frozen;

  if (dir == "right"){
    spitFire(getDoubleRect().x + 500, getDoubleRect().y);
  } else if (dir == "left"){
    spitFire(getDoubleRect().x - 500, getDoubleRect().y);
  } else if (dir == "down"){
    spitFire(getDoubleRect().x, getDoubleRect().y + 500);
  } else if (dir == "up"){
    spitFire(getDoubleRect().x, getDoubleRect().y - 500);
  } 
  if (!exploding) idleAnimation(seconds);

}

void Fire_Enemy::spitFire(double x, double y){
  if(exploding){return;}
  if(fireball != nullptr){
    if(!fireball->wasThrown()){
      targetX = x;
      targetY = y;
    }
    fireball->goTo(targetX, targetY);
  }
}

