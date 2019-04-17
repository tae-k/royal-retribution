#include "big_alien.h"
#include "character.h"

BigAlien::BigAlien(SDL_Renderer *renderer, std::string filename, ErrorHandler
  *error_handler, int width, int height, int pos_x, int pos_y, int hp,
  Sprite *king_p)
    : Boss_Enemy(renderer, filename, error_handler, width, height,
      pos_x, pos_y, 0, 0, hp) {

  rect.h /= 2;
  rect.w /= 2;

  king = king_p;
}

bool BigAlien::setHands(Hand *rh, Hand *lh){
  leftHand = lh;
  if(leftHand == nullptr) return false;
  rightHand = rh;
  if(rightHand == nullptr) return false;

  return true;
}

bool BigAlien::setBody(Sprite* b){
  body = b;
  if(body == nullptr) return false;

  return true;
}

doubleRect BigAlien::getDoubleRect(){
    doubleRect x;
    x.x = pos_x;
    x.y = pos_y;
    x.w = rect.w;
    x.h = rect.h;
    return x;
}

void BigAlien::update(double seconds){
  if (!fade) {
    dead = true;
    //beam->setCollidable(false);
    SDL_SetTextureAlphaMod(getTexture(), 0);
    return;
  }
  if (frozen) {
    Sprite::update(seconds);
    if(phase == 0){
      Sprite::animate(seconds, BIG_ENEMY_ANIM_POS, BIG_ENEMY_ANIM_POS
        + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
    } else {
      Sprite::animate(seconds, BIG_ENEMY_BEAM_POS, BIG_ENEMY_BEAM_POS
        + BEAM_FRAMES - 1, ENEMY_FPS*speedMultiplier);
    }
    return;
  }

  if(hp == 1){
 
    
    dying = true;
    fade = fadeOut(fade, seconds, 1.0);
    return;

  }

  if(phase == 0){
    //beam->setCollidable(false);
    //beam->setPosition(pos_x + getDoubleRect().w/2 - 36, pos_y + getDoubleRect().h);
    SDL_SetTextureAlphaMod(beam->getTexture(), 0);
    // if any/both hand(s) are dead
    if (left && static_cast<Hand*>(leftHand)->isDead()) {
      timePassed = 0.0;
      left = false;
    }
    if (!left && static_cast<Hand*>(rightHand)->isDead()) {
      timePassed = 0.0;
      left = true;
    }
    if (static_cast<Hand*>(leftHand)->isDead()
        && static_cast<Hand*>(rightHand)->isDead()) {
      dying = true;
      fade = fadeOut(fade, seconds, 1.0);
    
      return;
      timePassed = 0.0;
      changePhase(); 
    }

    // determine which hand to attack with
    if (left) {
      attackWith(leftHand, seconds);
    } else {
      attackWith(rightHand, seconds);
    }
    Sprite::animate(seconds, BIG_ENEMY_ANIM_POS, BIG_ENEMY_ANIM_POS
      + ENEMY_MOVING_FRAMES - 1, ENEMY_FPS*speedMultiplier);
  } else{
    beamFiring(seconds);
  }

  Sprite::update(seconds);

}

void BigAlien::attackWith(Hand* hand, double seconds) {
  timePassed += seconds;

  // setting up
  if (timePassed < 1.0) {
    settingUp = true;
    hand->velocityY = -20;
  } else {
    // prepare attack
    if (settingUp) {
      attacking = true;
      settingUp = false;
      targetX = king->pos_x + 16;
      targetY = king->pos_y + 25;
    }
  }

  // start attack
  if (attacking) {
    static_cast<Hand*>(hand)->goTo(targetX, targetY);
    if (static_cast<Hand*>(hand)->onGround()) attacking = false;
  }

  // hand is returned
  if (static_cast<Hand*>(hand)->returned()) {
    if (left) left = false;
    else left = true;
    timePassed = 0;
  }
}

void BigAlien::notifyCollision(Image* img, doubleRect*, bool) {
  if (img->isEnemy() || img->isCharacter() || img->isSword()) return;
}

void BigAlien::render(Camera* camera, double interpol_alpha) {
  if (dead) return;
  Sprite::render(camera, interpol_alpha);
}

void BigAlien::beamFiring(double seconds){
  if(beam != nullptr){
    speedMultiplier = .75;
    Sprite::animate(seconds, BIG_ENEMY_BEAM_POS, BIG_ENEMY_BEAM_POS
      + BEAM_FRAMES - 1, ENEMY_FPS*speedMultiplier);
    timePassed += seconds;
    //6 is the numper of frames before the beam appears
    if (timePassed > (6/(ENEMY_FPS*speedMultiplier))){
      beam->setPosition(pos_x + getDoubleRect().w/2 - 36, pos_y + getDoubleRect().h);
      beam->setCollidable(true);
      SDL_SetTextureAlphaMod(beam->getTexture(), 255);
    }
    if (timePassed > (BEAM_FRAMES/(ENEMY_FPS*speedMultiplier))){
      beam->setCollidable(false);
      SDL_SetTextureAlphaMod(beam->getTexture(), 0);
      timePassed = 0.0;
      hp=1;
    }
  }
}
BigAlien::~BigAlien(){}
