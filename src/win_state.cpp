/*
 * win_state.cpp
 * creates you won screen
 */

#include "win_state.h"

WinState::WinState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();
}

/* setup images */
void WinState::setup() {
  // game over text
  images["win"] = new Text(engine->renderer, FONT_ARCADE, errorHandler,
    0, 0, 200, logo, ROYAL_GOLD);
}

/* loads images */
void WinState::load() {
  State::load();
  // make the text transparent
  SDL_SetTextureAlphaMod(images["win"]->getTexture(), 0);
  // center the text
  auto center = getCenterForImage(images["win"]);
  images["win"]->setPosition(std::get<0>(center), std::get<1>(center));
}

/* updates the screen */
void WinState::update(double seconds) {
  State::update(seconds);

  totalTime += seconds;

  // fade in win
  a0 = fadeIn("win", a0, seconds, 3);
  // after 3 sec, transfer to highscore
  if (totalTime > 3) {
    Mix_HaltMusic();
    engine->newHighscore();
    engine->setState("Highscore");
  }
}

WinState::~WinState() {}
