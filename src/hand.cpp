#include "hand.h"

Hand::Hand(SDL_Renderer *renderer, std::string filename, ErrorHandler
  *error_handler, int width, int height, int pos_x, int pos_y,
  double velocityX, double velocityY, int health)
    : Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocityX, velocityY) {

  rect.h /= 2;
  rect.w /= 2;

  hp = health;
  origPosX = pos_x+80;
  origPosY = pos_y+80;
}

doubleRect Hand::getDoubleRect(){
    doubleRect x;
    x.x = pos_x;
    x.y = pos_y;
    x.w = rect.w;
    x.h = rect.h;
    return x;
}

void Hand::update(double seconds){
  if (!fade || frozen){ 
    Sprite::update(seconds);
    return;
  }
  if (isDead()) {
    goBack();
    fade = fadeOut(fade, seconds, 1.0);

    Sprite::update(seconds);
    return;
  }

  if (onGround()) {
    waitingTime += seconds;
  }

  if (returned()) {
    SDL_SetTextureAlphaMod(getTexture(), 255);
    waitingTime = 0;
    collidable = true;
    reachOrigX = false;
    reachOrigY = false;
    reachGroundX = false;
    reachGroundY = false;
  }
  if (waitingTime > STATIONARY_TIME) goBack();

  Sprite::update(seconds);
}

void Hand::kill() {
  hp--;
  SDL_SetTextureAlphaMod(getTexture(), 100);
  collidable = false;
  // just greater than STAIONARY_TIME
  waitingTime = 14.0;
}

void Hand::notifyCollision(Image* img, doubleRect*, bool) {
  if (img->isEnemy() || img->isCharacter() || img->isSword()) return;  
}

void Hand::goBack() {
  collidable = false;
  goTo(origPosX, origPosY);
}

void Hand::goTo(int x, int y) {
  int cen_x = pos_x + 80;
  int cen_y = pos_y + 80;
  double dis = pow(cen_x-x,2)+pow(cen_y-y,2);
  double xProp = pow(cen_x-x,2)/dis;
  double yProp = pow(cen_y-y,2)/dis;

  if (cen_x < x-5) velocityX = xProp*600;
  else if (cen_x > x+5) velocityX = -1*xProp*600;
  else {
    velocityX = 0;
    if (x == origPosX) reachOrigX = true;
    else reachGroundX = true;
  }
  if (cen_y < y-5) velocityY = yProp*600;
  else if (cen_y > y+5) velocityY = -1*yProp*600;
  else {
    velocityY = 0;
    if (y == origPosY) reachOrigY = true;
    else reachGroundY = true;
  }
}
