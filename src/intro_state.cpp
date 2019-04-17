/*
 * intro_state.cpp
 * creates intro scene
 */

#include "intro_state.h"

IntroState::IntroState(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, INTRO, TILES_TXT,
    &collisionDetector, &camera);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(INTRO_ADD);

  isScene = true;

  setup();
  load();

}

/* setup images */
void IntroState::setup() {
  //Camera Light
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT,
    errorHandler, 0, 0, false, true);
  // Lights
  num_lights = map->pushLights(images);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false);

  // throne
  images[ele+"throne"] = new Sprite(engine->renderer, THRONE_FILENAME,
     errorHandler, 1854, 2160, false);
  // king 
  images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, 1902, 2209, &eventHandler, &audioHandler, this);
  // squire
  images[ppl+"squire"] = new Squire(engine->renderer, SQUIRE_FILENAME,
    errorHandler, 16, 25, 1902, 2677, 0, 0);
  camera.setCharacter(static_cast<Character*>(images[ppl+"squire"]));
  // text
  images[top+"c0"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s1);
  images[top+"c1"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k1);
  images[top+"c2"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s2);
  images[top+"c3"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k2);
  images[top+"c4"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, s3);
  images[top+"c5"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k3);
  images[top+"c6"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k4);
  images[top+"c7"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 40, s4, ROYAL_GOLD);
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, skip);

  // skip intro
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("instruction");}, SDLK_1);
}

/* loads images */
void IntroState::load() {
  PlayingState::load();
  // set up throne
  images[ele+"throne"]->getDestRect()->w *= 2;
  images[ele+"throne"]->getDestRect()->h *= 2;
  // squre faces left and king is frozen
  static_cast<Squire*>(images[ppl+"squire"])->dir = "left";
  static_cast<Character*>(images[ppl+"king"])->frozen = true;
  // make convo textures transparent
  for (it = images.find(top+"c0"); it != images.end(); it++) {
    if (it->first != top+"skip")
      SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
  }
  // position all the text
  for (it = images.find(top+"c0"); it != images.end(); it++) {
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
    if (it->first == top+"skip") {
      it->second->pos_y = 0;
    } else if (it->first == top+"c7") {
      it->second->pos_y += 280;
    } else if (it->first == top+"c0"
               || it->first == top+"c2"
               || it->first == top+"c4") {
      it->second->pos_y -= 50;
    } else {
      it->second->pos_y -= 285;
    }
  }
}

/* updates the screen */
void IntroState::update(double seconds) {
  PlayingState::update(seconds);

  std::string s = top + "c" + std::to_string(counter);
  std::string prev = top + "c" + std::to_string(counter-1);

  switch(event) {
    // squire walks in
    case 0:
      if (images[ppl+"squire"]->pos_x > 1265) {
        images[ppl+"squire"]->velocityX = -250;
      } else {
        images[ppl+"squire"]->velocityX = 0;
        event++;
      }
      break;
    case 1:
      if (images[ppl+"squire"]->pos_y > 2405) {
        images[ppl+"squire"]->velocityY = -250;
      } else {
        images[ppl+"squire"]->velocityY = 0;
        event++;
      }
      break;
    case 2:
      if (images[ppl+"squire"]->pos_x < 1902) {
        images[ppl+"squire"]->velocityX = 250;
      } else {
        event++;
        images[ppl+"squire"]->velocityX = 0;
        static_cast<Squire*>(images[ppl+"squire"])->dir = "up";
      }
      break;
  }
  // text displayed with [n]
  if (event == 3) {
    if (counter < 7) {
      SDL_SetTextureAlphaMod(images[s]->getTexture(), 255);
      if (counter > 0) {
        SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
      }
      if (counter >= 5) {
        if (images[ppl+"king"]->pos_y < 2287) {
          images[ppl+"king"]->velocityY = 100;
        } else  images[ppl+"king"]->velocityY = 0;
      }
      // increments to the next text
      eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
        raise = true;}, SDLK_n);
      if (raise) { raise = false; counter++; }
    } else {
      SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
      // edit king movement
      images[ppl+"king"]->velocityX = 150;
      if (images[ppl+"king"]->pos_y > 2287) images[ppl+"king"]->velocityY = 0;
      // edit squire movement
      if (images[ppl+"king"]->pos_x > 1910 && !end) {
        static_cast<Squire*>(images[ppl+"squire"])->dir = "right";
        end = true;
      }
      if (images[ppl+"king"]->pos_x > 2550) {
        a = fadeIn(top+"c7", a, seconds, 2);
        static_cast<Squire*>(images[ppl+"squire"])->dir = "down";
        eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
        static_cast<Character*>(images[ppl+"king"])->frozen = false;
          engine->setState("instruction");}, SDLK_n);
      }
    }
  }
}

IntroState::~IntroState() {}
