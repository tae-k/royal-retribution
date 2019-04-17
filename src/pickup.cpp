#include "pickup.h"
#include "enemy.h"
#include "character.h"

Pickup::Pickup(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
  int width, int height, int pos_x, int pos_y, bool collidable, bool powerup, int theType):
  Sprite(renderer, filename, error_handler, width, height, pos_x, pos_y, collidable),
  powerup(powerup), type(theType){
  

}

Pickup::Pickup(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
  int pos_x, int pos_y, bool collidable, bool powerup, int theType):
  Sprite(renderer, filename, error_handler, pos_x, pos_y, collidable),
  powerup(powerup), type(theType){

}

void Pickup::update(double seconds) {
  if (type == healthNum) {
    if (!static_cast<Enemy*>(this->pair)->isDead()) {
      this->pos_x = this->pair->pos_x;
      this->pos_y = this->pair->pos_y+25;
      SDL_SetTextureAlphaMod(this->getTexture(), 0);
    } else {
      if (!eaten) {
        this->setCollidable(true);
        SDL_SetTextureAlphaMod(this->getTexture(), 255);
      }
    }
  }

  Sprite::update(seconds);
}

void Pickup::onPickUp(int) {
  pickedUp = true;
  setFixed(true);
  setPosition(inventoryPos * 40, 66);
  SDL_SetTextureAlphaMod(texture, 255);
  if(!activated){
    inventoryPos++;
  }
}

void Pickup::updateList(){
  inventoryPos = 0;
  for(Pickup* item : Character::inventory){
    if(item != nullptr && !item->isActivated()){
      item->setPosition(inventoryPos * 40, 66);
      inventoryPos++;
    }
  }
  
}

void Pickup::activate() {
  activated = true;

  SDL_SetTextureAlphaMod(texture, 0);
}

int Pickup::inventoryPos = 0;
