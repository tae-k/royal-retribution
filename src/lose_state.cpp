/*
 * lose_state.cpp
 * creates game over screen
 */

#include "lose_state.h"

LoseState::LoseState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

/* setup images */
void LoseState::setup() {
  // game over text
  images["lose"] = new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 200, logo, RED);
}

/* loads images */
void LoseState::load() {
  State::load();
  // make the text transparent
  SDL_SetTextureAlphaMod(images["lose"]->getTexture(), 0);
  // center the text
  auto center = getCenterForImage(images["lose"]);
  images["lose"]->setPosition(std::get<0>(center), std::get<1>(center));
}


/* updates the screen */
void LoseState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // fade in lose
  a0 = fadeIn("lose", a0, seconds, 3);
  // after 3 sec, transfer to menu
  if (totalTime > 3) {
    Mix_HaltMusic();
    engine->newHighscore();
    engine->setState("menu");
  }
}

LoseState::~LoseState() {}
