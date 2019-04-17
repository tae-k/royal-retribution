/*
 * title_state.cpp
 * creates title screen
 */

#include "title_state.h"

TitleState::TitleState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

/* setup images */
void TitleState::setup() {
  // space BG 0
  images["0BG"] = new Sprite(engine->renderer, BG_FILENAME, errorHandler,
    0, 0, false);
  // ship img 1
  images["1ship"] = new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    -80, 180, false);
  // earth img 2
  images["2earth"] = new Sprite(engine->renderer, PLANET_FILENAME, errorHandler,
    0, 0, false);
  // logo text 3
  images["3logo"] = new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 100, logo);
  // skip text 4
  images["4skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, skip);
}

/* loads images */
void TitleState::load() {
  State::load();
  // make all textures transparent
  for (it = images.find("1ship"); it != images.end(); it++) {
    SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
  }
  // center some of the textures
  for (it = images.find("2earth"); it != images.end(); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
  }
  // position skip text
  images["4skip"]->pos_y += 300;
}

/* updates the screen */
void TitleState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;
  if(!audioHandler.isPlaying()){audioHandler.play("intro");}
  // wrap space and fade in ship
  if (images["0BG"]->getDestRect()->x
      <= WIDTH - images["0BG"]->getDestRect()->w)
  {
    x = 0;
    y = images["0BG"]->pos_y;
    images["0BG"]->setPosition(x, y);
  }
  a0 = fadeIn("4skip", a0, seconds, 2.5);
  images["0BG"]->velocityX = -1 * speed;
  // after 3.5 sec, fade in ship
  if (totalTime > 3.5 && totalTime < 7.5) {
    a1 = fadeIn("1ship", a1, seconds, 2.5);
    images["1ship"]->velocityX = 4 * speed;
  }
  // after 7.5 sec, fade in logo
  if (totalTime > 7.5 && totalTime < 11.5) {
    a3 = fadeIn("3logo", a3, seconds, 2.5);
  }
  // after 11.5 sec, fade in earth
  if (totalTime > 11.5) {
    a3 = fadeOut("3logo", a3, seconds, 2.5);
    a2 = fadeIn("2earth", a2, seconds, 2.5);
    x = images["2earth"]->pos_x - 4.0 * speed * seconds;
    y = images["2earth"]->pos_y - 3.0 * speed * seconds;
    images["2earth"]->setPosition(x, y);
    images["2earth"]->getDestRect()->w += 14.0 * speed * seconds;
    images["2earth"]->getDestRect()->h += 14.0 * speed * seconds;
  }
  // after 15.5 sec, transfer to menu
  if (totalTime > 15.5) {
    engine->setState("menu");
  }
}

TitleState::~TitleState() {}
