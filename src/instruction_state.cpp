/*
 * instruction_state.cpp
 * creates intstruction scene
 */

#include "instruction_state.h"

InstructionState::InstructionState(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, TUTORIAL, TILES_TXT,
    &collisionDetector, &camera);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(TUTORIAL_ADD);

  isScene = true;

  setup();
  load();
}

/* setup images */
void InstructionState::setup() {
  //Camera Light
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT,
    errorHandler, 0, 0, false, true);
  // Lights
  num_lights = map->pushLights(images);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false);

  // king 0
  images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, 0, 0, &eventHandler, &audioHandler, this);
  images[ppl+"king"]->setPosition(map->width/2 - 16, map->height/2 - 25);
  camera.setCharacter(static_cast<Character*>(images[ppl+"king"]));
  SDL_SetTextureBlendMode(images[ppl+"king"]->getTexture(),SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(images[ppl+"king"]->getTexture(), 0);
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, 0, 0, static_cast<Sprite*>(images[ppl+"king"]), &eventHandler,
    &audioHandler, this);
  // instructions
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, skip, ROYAL_GOLD); 
  images[top+"i1"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i1, ROYAL_GOLD);
  images[top+"up"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, w);
  images[top+"left"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, a);
  images[top+"down"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, s);
  images[top+"right"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, d);
  images[top+"i2"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i2, ROYAL_GOLD);
  images[top+"shift"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sh);
  images[top+"space"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, sp);
  images[top+"i3"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 30, i3, ROYAL_GOLD);
  images[top+"open"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, e);
  images[top+"end"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, end, ROYAL_GOLD);
  // Score
  images[top+"zscore"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH - 114, 2, 16, "SCORE = " + std::to_string(engine->score));
  // Level
  images[top+"zlevel"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-25, 2, 16, "LEVEL 1");
  // Heart types
  images[top+"zheart_1"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 40, 34, false, true);
  images[top+"zheart_2"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 80, 34, false, true);
  images[top+"zheart_3"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 120, 34, false, true);
  // Stamina
  images[top+"zsta_box"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    0, 34, false, true);
  images[top+"zsta_bar"] = new Sprite(engine->renderer, STA_BAR, errorHandler,
    2, 38, false, true);
  // Experience
  images[top+"zexp_box"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    WIDTH/2 - 75, 34, false, true);
  images[top+"zexp_bar"] = new Sprite(engine->renderer, EXP_BAR, errorHandler,
    WIDTH/2 - 72, 38, false, true);
  // FPS Counter 
  images[top+"zfps"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    2, 2, 16, "FPS: ");
}

/* loads images */
void InstructionState::load() {
  PlayingState::load();
  // make all textures txt (!skip) transparent
  for (it = images.begin(); it != images.end(); it++) {
    if (it->first[0] == top[0] && it->first[1] != 'z') {
      if (it->first != top+"skip") {
        if (SDL_SetTextureAlphaMod(it->second->getTexture(), 0) < 0) {
          errorHandler->quit(__func__, SDL_GetError());
        }
      }
      auto center = getCenterForImage(it->second);
      it->second->setPosition(std::get<0>(center), std::get<1>(center));
      if (it->first == top+"skip") {
        it->second->pos_y = HEIGHT - 100;
      } else if (it->first==top+"up"
                 ||it->first==top+"open"
                 ||it->first==top+"shift") {
        it->second->pos_y -= 50;
      } else if (it->first == top+"left") {
        it->second->pos_x -= 110;
      } else if (it->first == top+"down" || it->first == top+"space") {
        it->second->pos_y += 50;
      } else if (it->first == top+"right") {
        it->second->pos_x += 114;
      } else {
        it->second->pos_y = 100;
      }
    }
  }
  // set size of stam & exp & hearts
  images[top+"zsta_box"]->getDestRect()->h = 32;
  images[top+"zsta_box"]->getDestRect()->w = 150;
  images[top+"zsta_bar"]->getDestRect()->h = 24;
  images[top+"zsta_bar"]->getDestRect()->w = 146;
  images[top+"zexp_box"]->getDestRect()->h = 32;
  images[top+"zexp_box"]->getDestRect()->w = 150;
  images[top+"zexp_bar"]->getDestRect()->h = 24;
  images[top+"zexp_bar"]->getDestRect()->w = 148;
  static_cast<Sprite*>(images[top+"zheart_1"])->setSrcRect(0, 0, 32, 32);
  static_cast<Sprite*>(images[top+"zheart_2"])->setSrcRect(0, 0, 32, 32);
  static_cast<Sprite*>(images[top+"zheart_3"])->setSrcRect(0, 0, 32, 32);
}

/* updates the screen */
void InstructionState::update(double seconds) {
  PlayingState::update(seconds);
  timer += seconds;

  // update FPS
  if (timer > 1) {
    delete images[top+"zfps"];
    images[top+"zfps"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      2, 2, 16, "FPS: " + std::to_string((int)(1/seconds)));
    images[top+"zfps"]->load();
    timer = 0;
  }
  // update sta
  bar_w = int(static_cast<Character*>(images[ppl+"king"])->sta * 146);
  images[top+"zsta_bar"]->getDestRect()->w = bar_w;
  // update exp
  bar_w = int(static_cast<Character*>(images[ppl+"king"])->exp * 146);
  images[top+"zexp_bar"]->getDestRect()->w = bar_w;

  // skip intro
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("playing");}, SDLK_1);

  // fade in move text
  a0 = fadeIn(top+"i1", a0, seconds, 4);
  a1 = fadeIn(top+"up", a1, seconds, 4);
  a2 = fadeIn(top+"left", a2, seconds, 4);
  a3 = fadeIn(top+"down", a3, seconds, 4);
  a4 = fadeIn(top+"right", a4, seconds, 4);

  // determine of player pressed move buttons
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"up"]->getTexture(), 0, 255, 0);
    up = true;
  }, SDLK_w);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"up"]->getTexture(), 0, 255, 0);
    up = true;
  }, SDLK_UP);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"left"]->getTexture(), 0, 255, 0);
    left = true;
  }, SDLK_a);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"left"]->getTexture(), 0, 255, 0);
    left = true;
  }, SDLK_LEFT);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"down"]->getTexture(), 0, 255, 0);
    down = true;
  }, SDLK_s);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"down"]->getTexture(), 0, 255, 0);
    down = true;
  }, SDLK_DOWN);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"right"]->getTexture(), 0, 255, 0);
    right = true;
  }, SDLK_d);
  eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
    SDL_SetTextureColorMod(images[top+"right"]->getTexture(), 0, 255, 0);
    right = true;
  }, SDLK_RIGHT);

  // if all move buttons pressed
  if (up && down && left && right) {
    SDL_SetTextureAlphaMod(images[top+"i1"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"up"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"left"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"down"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"right"]->getTexture(), 0);
    a5 = fadeIn(top+"i2", a5, seconds, 4);
    a6 = fadeIn(top+"shift", a6, seconds, 4);
    a7 = fadeIn(top+"space", a7, seconds, 4);

    // determine if all action buttons were pressed
    if (static_cast<Character*>(images[ppl+"king"])->running
        && (static_cast<Sprite*>(images[ppl+"king"])->velocityX != 0
            || static_cast<Sprite*>(images[ppl+"king"])->velocityY != 0))
    {
      SDL_SetTextureColorMod(images[top+"shift"]->getTexture(), 0, 255, 0);
      run = true;
    }
    eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
      SDL_SetTextureColorMod(images[top+"space"]->getTexture(), 0, 255, 0);
      attack = true;
    }, SDLK_SPACE);
  }

  // if ran and attacked
  if (run && attack) {
    SDL_SetTextureAlphaMod(images[top+"i2"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"shift"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"space"]->getTexture(), 0);
    a9 = fadeIn(top+"i3", a9, seconds, 4);
    a10 = fadeIn(top+"open", a10, seconds, 4);

    if (images[ppl+"king"]->pos_x >= 510 && images[ppl+"king"]->pos_y <= 74) {
      eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
        SDL_SetTextureColorMod(images[top+"open"]->getTexture(), 0, 255, 0);
        open = true;
      }, SDLK_e);
    }
  }

  if (open) {
    SDL_SetTextureAlphaMod(images[top+"i3"]->getTexture(), 0);
    SDL_SetTextureAlphaMod(images[top+"open"]->getTexture(), 0);
    a8 = fadeIn(top+"end", a8, seconds, 4);
    eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
      engine->setState("playing");}, SDLK_n);
  }
}

InstructionState::~InstructionState() {}
