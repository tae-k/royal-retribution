/*
 * credit_state.cpp
 */

#include "credit_state.h"

CreditState::CreditState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {

    setup();
    load();
}
void CreditState::setup(){
  images["t0"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, HEIGHT / 3, 40, "Credits" );
  images["t1"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t0"]->getDestRect()->y + 55, 40, "Programmers");
  images["t2"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t1"]->getDestRect()->y + 55, 30, "Idean LaBiB");
  images["t3"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t2"]->getDestRect()->y + 55, 30, "Tae Jin Kim");
  images["t4"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t3"]->getDestRect()->y + 55, 30, "Billy Carrington");
  images["t5"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t4"]->getDestRect()->y + 55, 40, " Graphics");
  images["t6"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t5"]->getDestRect()->y + 55, 30, "Derek Chang");
  images["t7"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t6"]->getDestRect()->y + 55, 40, "Music");
  images["t8"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH / 2, images["t7"]->getDestRect()->y + 55, 30, "Billy Carrington");
  images["skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, HEIGHT-35, 30, "Press [q] to return to main");
}

void CreditState::load() {
  State::load();
  int x;
  int y;
  for (it = images.begin(); it != images.end(); it++) {
    x = WIDTH / 2 - it->second->getDestRect()->w / 2;
    y = it->second->getDestRect()->y;
    if (it->first == "skip") it->second->setPosition(x, HEIGHT-35);
    else {
      it->second->setPosition(x, y);
      it->second->velocityY = -150;
    }
  }
}

void CreditState::update(double seconds){
  State::update(seconds);

  if(images["t0"]->getDestRect()->y <= 0) {
    for(it = images.begin(); it != images.end(); it++){
      it->second->velocityY = 0;
    }
    SDL_SetTextureColorMod(images["t1"]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images["t2"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t3"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t4"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t5"]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images["t6"]->getTexture(), 0, 80, 190);
    SDL_SetTextureColorMod(images["t7"]->getTexture(), 0, 180, 60);
    SDL_SetTextureColorMod(images["t8"]->getTexture(), 0, 80, 190);
  }
} 

CreditState::~CreditState(){}
