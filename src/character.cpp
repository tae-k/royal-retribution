/*
 * character.cpp
 */

#include "character.h"
#include "hand.h"
#include "state.h"
#include "playing_state.h"
//Definitions for the character class that inherits from Sprite

//constructor that takes in arguments for the width and height for the rect
Character::Character(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int width, int height, int pos_x, int pos_y,
  EventHandler *eventHandler, Audio *audioHandler, State *state_p)
    : Sprite(renderer, filename, error_handler, width, height, pos_x, pos_y,
      true),
    audioHandler(audioHandler) {    

  rect.w *= 2;
  rect.h *= 2;

  startingX = pos_x;
  startingY = pos_y;

  state = state_p;
  createListeners(eventHandler);
  
  retrieveInteractables();
}

//constructor that does not take in width or height for the rect
Character::Character(SDL_Renderer *renderer, std::string filename,
  ErrorHandler *error_handler, int pos_x, int pos_y, EventHandler *eventHandler,
  Audio *audioHandler, State *state_p)
    : Sprite(renderer, filename, error_handler, pos_x, pos_y,
      true),
    audioHandler(audioHandler) {

  rect.w *= 2;
  rect.h *= 2;

  startingX = pos_x;
  startingY = pos_y;

  state = state_p;
  
  createListeners(eventHandler);
}

void Character::update(double seconds) {
  if (dying) {
    die(seconds);
    Sprite::update(seconds);
    return;
  }

  invincibilitySeconds += seconds;
  attackingTimer += seconds;
  staSec += seconds;

  if (!frozen) updateSta();
  if (hearts == 0) dying = true;

  if (lastAttack && attackingTimer > ATTACK_FRAMES/(CHARACTER_FPS*2.0)) {
    attacking = false;
    lastAttack = false;
  }

  invincible = (invincibilitySeconds < INVINCIBLE_TIME) ? true : false;

  Sprite::update(seconds);

  // If attacking, don't move.
  if (attacking) {
    pos_x -= velocityX * seconds * speedMultiplier;
    pos_y -= velocityY * seconds * speedMultiplier;
  }

  //if the character would leave the window, stop movement in that direction.
  rect.x = (int) pos_x;
  rect.y = (int) pos_y;

  if (!attacking) {
    if (velocityX > 0) {
      dir = "right";
      Sprite::animate(seconds, R_RUNNING_POS, R_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityX < 0) {
      dir = "left";
      Sprite::animate(seconds, L_RUNNING_POS, L_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityY > 0) {
      dir = "down";
      Sprite::animate(seconds, D_RUNNING_POS, D_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else if (velocityY < 0) {
      dir = "up";
      Sprite::animate(seconds, U_RUNNING_POS, U_RUNNING_POS+RUNNING_FRAMES - 1,
        CHARACTER_FPS*speedMultiplier);
    } else {
      if (!dying) idleAnimation(seconds);
    }
  }

  if (attacking && !frozen) {
    if (dir == "up")
      Sprite::animate(seconds, U_ATTACK_POS, U_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
    else if (dir == "down")
      Sprite::animate(seconds, D_ATTACK_POS, D_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
    else if (dir == "right")
      Sprite::animate(seconds, R_ATTACK_POS, R_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
    else if (dir == "left")
      Sprite::animate(seconds, L_ATTACK_POS, L_ATTACK_POS + ATTACK_FRAMES - 1,
        CHARACTER_FPS*2.0);
  }

  checkInteractables();
}

void Character::render(Camera* camera, double interpol_alpha) {
  if (dying) {
    SDL_SetTextureAlphaMod(texture, 255);
  } else if (invincibilitySeconds < INVINCIBLE_TIME) {
    SDL_SetTextureAlphaMod(texture, 100);
  } else {
    if (!falling && !dead) SDL_SetTextureAlphaMod(texture, 255);
  }

  Sprite::render(camera, interpol_alpha);
 
  for(Pickup* item : inventory){
    if(item != nullptr && !item->isActivated())
      item->render(camera, interpol_alpha);
  }
}

void Character::idleAnimation(double seconds) {
  int pos = -1;

  if (dir == "right") pos = R_RUNNING_POS;
  else if (dir == "left") pos = L_RUNNING_POS;
  else if (dir == "up") pos = U_RUNNING_POS;
  else if (dir == "down") pos = D_RUNNING_POS;
  else error_handler->quit(__func__, "direction not found");

  Sprite::animate(seconds, pos, pos + IDLE_FRAMES - 1);
}

void Character::notifyCollision(Image* image, doubleRect* intersection,
  bool resolved) {
  std::string collisionDir = "";

  /*after stepping on a trap tile the associated door appears and is collidable
  The trap is no longer collidable, and the character is paired with the door
  */
  if(static_cast<Sprite*>(image)->isTrap()&& static_cast<Sprite*>(image)->pair != nullptr){
    static_cast<Sprite*>(image)->pair->setCollidable(true);
    state->collisionDetector.updateBuckets(static_cast<Sprite*>(image)->pair, state->map);

    SDL_SetTextureAlphaMod(static_cast<Sprite*>(image)->pair->getTexture(), 255);
    static_cast<Sprite*>(image)->setCollidable(false);
    this->setPair(static_cast<Sprite*>(image)->pair);

    state->activateInstructionText(trapNum);
    return;
  }

  // add a full heart if food is picked up
  else if (static_cast<Sprite*>(image)->isPickup()
           && static_cast<Pickup*>(image)->getType() == healthNum)
  {
    for (int i = 0; i < 2; i++) {
      if (hearts < maxHearts) hearts++;
    }
    static_cast<Pickup*>(image)->eat();
    static_cast<Pickup*>(image)->activate();
    SDL_SetTextureAlphaMod(image->getTexture(), 0);
    static_cast<Sprite*>(image)->setCollidable(false);
  }
  if(static_cast<Sprite*>(image)->isHole()){
    PlayingState::fallen = 1;
    falling = true;
    velocityX = 0;
    velocityY = 0;
  }
  if(static_cast<Sprite*>(image)->isHole()){
    PlayingState::fallen = 1;
    falling = true;
    velocityX = 0;
    velocityY = 0;
  }
  // either up/down
  if (intersection->w > intersection->h) {
    if (intersection->y <= image->pos_y)
      collisionDir = "down";
    else
      collisionDir = "up";
  } else {
    if (intersection->x <= image->pos_x)
      collisionDir = "right";
    else
      collisionDir = "left";
  }

  if (!image->isSword())
    Sprite::notifyCollision(image, intersection, resolved);

  //When collision detector detects a collision play the sound effect

  if ((image->isEnemy() && (!attacking || collisionDir != dir) && !invincible)
    || (static_cast<Sprite*>(image)->isBlade() && !invincible)) {

    if (static_cast<Sprite*>(image)->isHand()) {
      if (!static_cast<Hand*>(image)->onGround()) {
        static_cast<Sprite*>(image)->setCollidable(false);
        audioHandler->play("collision", 0);
        invincibilitySeconds = 0;
        invincible = true;
        hearts-=2;
      }
    } else {
      audioHandler->play("collision", 0);
      invincibilitySeconds = 0;
      invincible = true;
      hearts--;
    }
  }
  if(static_cast<Sprite*>(image)->isLaser() && !invincible){
        audioHandler->play("collision", 0);
        invincibilitySeconds = 0;
        invincible = true;
        hearts-=2;

  } 
}

void Character::updateSta() {
  if (sta > 1) sta = 1;
  if (sta < 0) sta = 0;
  // if running
  if (running) {
    if ((velocityX != 0 || velocityY != 0) && !staminaBypass) {
      staSec = 0;
      if (sta > 0) {
        sta -= 0.01;
      } else {
        speedMultiplier = 1;
        running = false;
      }
    }
  // if not running
  } else {
    if (staSec > STA_WAITING_TIME){
      sta += 0.005;
    }
  }

  if (staminaBar != nullptr) {
    int w = int(sta * 146);  
    staminaBar->getDestRect()->w = w;
  }
}

void Character::updateExpBar() {
  if (expBar != nullptr) {
    int w = int(exp * 146);
    expBar->getDestRect()->w = w;
  }
}

void Character::setExpBar(Image* img) {
  expBar = img;
  updateExpBar();
}

void Character::updateExp() {
  exp += expInc;
  if (exp >= 1) {
    exp--;
    level++;
    expInc -= 0.02;
    if (level == 9) expInc = 0.02;
    if (level == 10) expInc = 0;
  }
  state->engine->score += 1000;
  
  updateExpBar();
}

void Character::createListeners(EventHandler *eventHandler) {
  // w, a, s, d conrols
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = SPEED_CHAR; }, SDLK_d);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = -SPEED_CHAR; }, SDLK_a);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = SPEED_CHAR; }, SDLK_s);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = -SPEED_CHAR; }, SDLK_w);
  // up, down, left, right controls
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = SPEED_CHAR; }, SDLK_RIGHT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityX = -SPEED_CHAR; }, SDLK_LEFT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = SPEED_CHAR; }, SDLK_DOWN);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) velocityY = -SPEED_CHAR; }, SDLK_UP);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    attacking = true; }, SDLK_SPACE);

  //when key is released, velocity set back to 0
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX > 0) velocityX = 0;}, SDLK_d);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX < 0) velocityX = 0;}, SDLK_a);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY > 0) velocityY = 0;}, SDLK_s);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY < 0) velocityY = 0;}, SDLK_w);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX > 0) velocityX = 0;}, SDLK_RIGHT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityX < 0) velocityX = 0;}, SDLK_LEFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY > 0) velocityY = 0;}, SDLK_DOWN);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (velocityY < 0) velocityY = 0;}, SDLK_UP);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    lastAttack = true; attackingTimer = 0; }, SDLK_SPACE);
  
  // boost control
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) speedMultiplier = 3; running = true;}, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) speedMultiplier = 3; running = true; staminaBypass = true;}, SDLK_RSHIFT);

  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    speedMultiplier = 1; running = false; }, SDLK_LSHIFT);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    speedMultiplier = 1; running = false; staminaBypass = false;}, SDLK_RSHIFT);
  //object interaction 
  eventHandler->addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    if (!frozen) interacting = true;}, SDLK_e);
  eventHandler->addListener(SDL_KEYUP, [&](SDL_Event*) {
    interacting = false;}, SDLK_e);
}

doubleRect Character::getDoubleRect() {
  doubleRect x;
  x.x = pos_x;
  x.y = pos_y + (rect.h / 2);
  x.w = rect.w;
  x.h = rect.h / 2;
  return x;
}

void Character::pickUp(Pickup* pickup) {
  inventory.push_back(pickup);
  pickup->onPickUp(inventory.size() - 1);

  if (pickup->isPowerup()) {
    activePowerups.push_back(pickup->getType());
  }
}

void Character::retrieveInteractables(){
  for(auto tile : state->map->additions){ 
    if(!tile.image->isCollidable()){
      break;
    }
    if(tile.image->isChest() || tile.image->isDoor()){
      interactables.push_back(tile.image);
    } 
  }
}

void Character::checkInteractables(){
  doubleRect spriteRect;
  doubleRect thisRect;
  if(!interacting){
    return;
  }
  for(auto sprite : interactables){
    thisRect = getDoubleRect();
    spriteRect = sprite->getDoubleRect();

    spriteRect.x -= spriteRect.w / 4;
    spriteRect.y -= spriteRect.h / 4;
    spriteRect.w += spriteRect.w / 2;
    spriteRect.h += spriteRect.h / 2;

    if(Util::isIntersecting(&spriteRect, &thisRect)){
      if (sprite->isDoor()){
        for(Pickup* item : inventory){
          if( item->getType() == keyNum && !item->isActivated()){
            SDL_SetTextureAlphaMod(sprite->getTexture(), 0);
            sprite->setCollidable(false);
            item->activate();
            item->updateList();
          }
        }
      } else if(sprite->isChest()){
        sprite->pair = this;
      }

    }

  } 

}

void Character::die(double seconds) {
  if (!done) Sprite::animate(seconds, DIE_POS, DIE_POS + RUNNING_FRAMES - 1,
    CHARACTER_FPS*0.5);
  dyingTimer += seconds;

  if (dyingTimer > (RUNNING_FRAMES/(CHARACTER_FPS*0.5)))
    done = true;
  if (dyingTimer > (RUNNING_FRAMES/(CHARACTER_FPS*0.5)) + 1)
    dead = true;
}

Character::~Character(){}

std::vector<Pickup*> Character::inventory;

std::vector<int>Character::activePowerups;

int Character::hearts = 6;

int Character::maxHearts = 6;

int Character::level = 1;

double Character::exp = 0;

int Character::currFloor = 1;

int Character::highestFloor = 1;
