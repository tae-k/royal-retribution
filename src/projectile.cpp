#include "projectile.h"


Projectile::Projectile(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
  int width, int height, int pos_x, int pos_y, double radius,double wt, bool enem, Sprite *owner)
    :Sprite(renderer, filename, error_handler, width, height, pos_x, pos_y, true) {

  dissipateRadius = radius;
  thrower = owner;
  enemy = enem;
  waitTime = wt;
}

void Projectile::goTo(Sprite* dest){
  destX = dest->getDoubleRect().x;
  destY = dest->getDoubleRect().y;

  goTo(destX, destY);

}

void Projectile::goTo(double x, double y){
  
  double dis = pow(getDoubleRect().x-x,2)+pow(getDoubleRect().y-y,2);
  double xProp = pow(getDoubleRect().x-x,2)/dis;
  double yProp = pow(getDoubleRect().y-y,2)/dis;

  if(frozen){

    velocityX = 0;
    velocityY = 0;
    return;
  }  

  if (getDoubleRect().x < x-5){
    velocityX = xProp*300;
    throwPosX = rect.w;
  } else if (getDoubleRect().x > x+5){
    velocityX = -1*xProp*300;
    throwPosX = -1*rect.w;
  } else {
    velocityX = 0.0;
    throwPosX = 0.0;
  }
  if (getDoubleRect().y < y-5){
    velocityY = yProp*300;
    throwPosY = rect.h;
  } else if (getDoubleRect().y > y+5){
    velocityY = -1*yProp*300;
    throwPosY = -1*rect.h;
  } else {
    velocityY = 0.0;
    throwPosY = 0.0;
  }
  if(velocityY != 0.0 || velocityX != 0.0){
    throwThis();
  } else{
    waiting = true;
    if(timeStill > waitTime){
      waiting = false;
    }
    thrown = false;
  }

}

void Projectile::throwThis(){
  if(thrown == false && !waiting){
    thrown = true;
    setPosition(thrower->getDoubleRect().x, thrower->getDoubleRect().y);
    timeStill = 0.0;
  }
}

void Projectile::update(double seconds){

  double dis;
  if(!frozen){

    //velocityY = tempY;
    //velocityX = tempX;
    //std::cout<<velocityY <<"\t"<<velocityX<<"\n";

  } else {
    //tempX = velocityX;
    //tempY = velocityY;
    //velocityY = 0;
    //velocityX = 0;
  }
  Sprite::update(seconds);
  if(thrown == false){
    if(waiting == true){
      if(timeStill > waitTime){
        waiting = false;
      }
      
      if(!frozen){timeStill += seconds;}
      collidable = true;
    }else {
      setPosition(thrower->getDoubleRect().x, thrower->getDoubleRect().y);
      collidable = false;
      SDL_SetTextureAlphaMod(getTexture(), 0);
    }
    velocityY = 0;
    velocityX = 0;
  } else {
    timeStill = 0.0;
    collidable = true;
    SDL_SetTextureAlphaMod(getTexture(), 255);
    
  }
  if(overridden){
    overridden = false;
    thrown = false;
    waiting = false;
    //timeStill = 0.0;
  }

  dis = sqrt(pow(getDoubleRect().x-thrower->getDoubleRect().x,2) +
    pow(getDoubleRect().y - thrower->getDoubleRect().y,2));
  if(dis > dissipateRadius){
    thrown = false;
    waiting = true;
  }
  Sprite::animate(seconds, FIREBALL_START, FIREBALL_START + 3, 4);   

}

void Projectile::notifyCollision(Image* img, doubleRect* intersection, bool resolved){
  if(img == thrower || img->isSword() || img->isEnemy() == enemy) return;
    thrown = false;
    waiting = false;
    Sprite::notifyCollision(img, intersection, resolved);

}
