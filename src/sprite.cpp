/*
 * sprite.cpp
 */

#include "sprite.h"
#include "character.h"
#include "camera.h"
#include "util.h"
//Definitions for the the parent class that other sprite objects inherit from

/*Constructor that makes a sprite from an inputed image file along with a gi-
ven position to load in the sprite and size of its rect*/
Sprite::Sprite(SDL_Renderer *renderer, std::string image_filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  bool collidable_p, bool fixed_p)
  : Image(renderer, image_filename, error_handler) {

  fixed = fixed_p;

  setPosition(pos_x, pos_y);

  collidable = collidable_p;

  srcRect = {0, 0, width, height};
  rect = {(int) pos_x, (int) pos_y, width, height};
}

//Another constructor, but instead sets the width and height of the rect to 0
Sprite::Sprite(SDL_Renderer *renderer, std::string image_filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, bool collidable_p,
    bool fixed_p)
  : Image(renderer, image_filename, error_handler) {

  fixed = fixed_p;

  setPosition(pos_x, pos_y);

  collidable = collidable_p;

  srcRect = {0, 0, 0, 0};
  rect = {(int) pos_x, (int) pos_y, 0, 0};
}

void Sprite::load(SDL_Texture *texture_p) {
  Image::load(texture_p);
  /*When width and height were not specified, the rect is set to the the 
  width and height of the image texture that was loaded*/
  if (rect.w == 0 && rect.h == 0) {
    get_texture_size(texture, &(rect.w), &(rect.h));
  }
  
  if((door && !collidable) || laser){
    SDL_SetTextureAlphaMod(this->getTexture(), 0);  
  }
}

//moves the position of sprite based on current velocity and time passed
void Sprite::update(double seconds) {
  //prevRect = rect;
  prevRect = {(int) pos_x, (int) pos_y, rect.w, rect.h};
  if(isBlade()){
    if(higherVel == 0){ 
      higherVel = (velocityY == 0)? velocityX : velocityY;
    }
    blade_timer += seconds;
    if(stopped){
      velocityY = 0;
      velocityX = 0;
      blade_timer -= seconds;
    } else if(blade_timer > 8.0){
      velocityX = -1 * higherVel;
      velocityY = 0;
      blade_timer = 0;
    }
    else if(blade_timer > 6.0){
      velocityY = higherVel;
      velocityX = 0;
    }
    else if(blade_timer > 4.0){
      velocityX = higherVel;
      velocityY = 0;
    }
    else if(blade_timer > 2.0){
      velocityY = -1 * higherVel;
      velocityX = 0;
    }
  }
  double new_x = speedMultiplier * velocityX * seconds + pos_x;
  double new_y = speedMultiplier * velocityY * seconds + pos_y;

  setPosition(new_x, new_y);
  //Blades special movement pattern
}

void Sprite::animate(double seconds, int start_frame, int end_frame, int fps) {
  double spf = 1/((double) fps);
  if (currentFrame > end_frame || currentFrame < start_frame) {
    currentFrame = start_frame - 1;
    timer = spf;
  }

  if (timer >= spf)  {
    currentFrame = ((currentFrame - start_frame + 1)
      % (end_frame - start_frame + 1)) + start_frame;
  
    int row = currentFrame / NUM_COLS;
    int col = currentFrame % NUM_COLS;

    srcRect.x = col*(srcRect.w + SPRITE_PADDING_AMOUNT_X);
    srcRect.y = row*(srcRect.h + SPRITE_PADDING_AMOUNT_Y);

    if(chest && currentFrame == 63){
      pair = this;
      return;
    }
    
    timer = 0;
  }
  timer += seconds;
}

void Sprite::render(Camera* camera, double interpol_alpha) {
  if (camera == nullptr) {
    error_handler->quit(__func__, "must specify a non-null camera to render for a sprite!"); 
  }

  int renderResult = 0;
  if (srcRect.w == 0 && srcRect.h == 0) {
    renderResult = camera->render(renderer, texture, NULL, &rect, &prevRect, interpol_alpha, fixed);
  } else {
    renderResult = camera->render(renderer, texture, &srcRect, &rect, &prevRect, interpol_alpha, fixed);
  }

  if (renderResult < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

SDL_Rect* Sprite::getDestRect() {
  return &rect;
}

bool Sprite::isCollidable() {
  return collidable;
}

void Sprite::setCollidable(bool collide){
  collidable = collide;
}

void Sprite::get_texture_size(SDL_Texture *texture, int *width, int *height) {
  if (SDL_QueryTexture(texture, NULL, NULL, width, height) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

/*if the intersection of 2 collidable sprites have more height overlap, y 
velocity changes direction if more width, x direction changes*/
void Sprite::notifyCollision(Image* img, doubleRect* intersection, bool resolved) {
  if (!resolved) {
    if(isBlade()){
      if(img->isSword()){
        return;
      }
    } 
  if(isLaser()){
    if(img->isCharacter()){
      collidable = false;
    }
    return;
  }
  /*if (abs(velocityY) > abs(velocityX)) {
    resolveYCollision(img, intersection->h);
  } else if (abs(velocityY) == abs(velocityX)) {*/
    if (velocityX == 0 && velocityY == 0 && (img->velocityX != 0 ||
      img->velocityY != 0)) {

      img->notifyCollision(this, intersection, false);
    } else {
      if (intersection->w > intersection->h) {
        resolveYCollision(img, intersection->h);
      } else {
        resolveXCollision(img, intersection->w);
      }

      img->notifyCollision(this, intersection, true);

      setPosition(pos_x, pos_y);
      img->setPosition(img->pos_x, img->pos_y);
    }
  /*} else {
    resolveXCollision(img, intersection->w);
  }*/
  if(blade){
    if (intersection->w > intersection->h) {
      velocityY *= -1;
      setPosition(pos_x, pos_y);
    } else {
      velocityX *= -1;
      setPosition(pos_x, pos_y);
    }
  }
}

}

void Sprite::resolveXCollision(Image* img, double intersection) {
  double velocity = velocityX * (double) speedMultiplier;
  double img_velocity = img->velocityX * (double) img->speedMultiplier;
  if (velocity != 0) {
    double velRatio = abs(velocity) / (abs(velocity) + abs(img_velocity));

    double adjustmentMagnitude = intersection * velRatio;
    pos_x -= velocity / abs(velocity) * (adjustmentMagnitude);

    if (img_velocity != 0) {
      adjustmentMagnitude = intersection - adjustmentMagnitude;
      img->pos_x -= img_velocity / abs(img_velocity) * (adjustmentMagnitude);
    }
  }
}

void Sprite::resolveYCollision(Image* img, double intersection) {
  double velocity = velocityY * (double) speedMultiplier;
  double img_velocity = img->velocityY * (double) img->speedMultiplier;
  if (velocity != 0) {
    double velRatio = abs(velocity) / (abs(velocity) + abs(img_velocity));

    double adjustmentMagnitude = intersection * velRatio;

    pos_y -= velocity / abs(velocity) * (adjustmentMagnitude);

    if (img_velocity != 0) {
      adjustmentMagnitude = intersection - adjustmentMagnitude;
      img->pos_y -= img_velocity / abs(img_velocity) * (adjustmentMagnitude);
    }
  }
}

Sprite::~Sprite() {

}

void Sprite::setSrcRect(int posX, int posY, int rectWidth, int rectHeight){
  srcRect.x = posX;
  srcRect.y = posY;
  srcRect.w = rectWidth;
  srcRect.h = rectHeight;
}
void Sprite::setPosition(double x, double y) {
  pos_x = x;
  pos_y = y;
  
  rect.x = (int) x;
  rect.y = (int) y;
}
void Sprite::setVelocity(double x, double y) {
  velocityX = x;
  velocityY = y;
}

doubleRect Sprite::getDoubleRect() {
  doubleRect x;
  x.x = pos_x;
  x.y = pos_y;
  x.w = rect.w;
  x.h = rect.h;
  return x;
}


