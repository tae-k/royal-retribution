/*
 * boss_scene.cpp
 */

#include "boss_scene.h"
#include "big_alien.h"
#include "main_boss.h"

BossScene::BossScene(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, FINAL_LEVEL, TILES_TXT,
    &collisionDetector, &camera);

  isScene = true;

  setup();
  load();
}

void BossScene::setup() {
  // Player 
  if (king != nullptr)
    images[ppl+"king"] = king;
  else
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, 898, 1460, &eventHandler, &audioHandler, this);
  camera.setCharacter(static_cast<Character*>(images[ppl+"king"]));
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, 900, 1400, static_cast<Sprite*>(images[ppl+"king"]),
    &eventHandler, &audioHandler, this);

  // Big Alien
  images[ppl+"eBigAlien"] = new BigAlien(engine->renderer, BIG_HEAD,
    errorHandler, 160, 164, map->width/2 - 150, map->height/2 - 520, 3,
    static_cast<Sprite*>(images[ppl+"king"]));
  // body
  images[ppl+"BigBody"] = new Sprite(engine->renderer, BIG_BODY,
    errorHandler, 200, 80, map->width/2 - 100, map->height/2 - 305, false);
  // left hand
  images[ppl+"eBigLF"] = new Hand(engine->renderer, BIG_LF,
    errorHandler, 200, 200, map->width/2 + 100, map->height/2 - 340, 0, 0, 4);
  // right hand
  images[ppl+"eBigRF"] = new Hand(engine->renderer, BIG_RF,
    errorHandler, 200, 200, map->width/2 - 300, map->height/2 - 340, 0, 0, 4);
  // set body parts
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setHands(static_cast<Hand*>(images[ppl+"eBigLF"]),
      static_cast<Hand*>(images[ppl+"eBigRF"]));
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setBody(static_cast<Sprite*>(images[ppl+"BigBody"]));

  // Main Boss
  images[ppl+"eMainBoss"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 - 16, map->height/2-200, 0, 0, 9, map);
  images[ppl+"eclone1"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, 0, 0, 0, 0, 9, map);
  images[ppl+"eclone2"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, 0, 0, 0, 0, 9, map);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->clone1 = static_cast<MainBoss*>(images[ppl+"eclone1"]);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->clone2 = static_cast<MainBoss*>(images[ppl+"eclone2"]);
  static_cast<MainBoss*>(images[ppl+"eclone1"])
    ->setClone(true);
  static_cast<MainBoss*>(images[ppl+"eclone2"])
    ->setClone(true);

  // text
  images[top+"c0"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, e1);
  images[top+"c1"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k1);
  images[top+"c2"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, e2);
  images[top+"c3"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, e3);
  images[top+"c4"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 25, k2);
  images[top+"c5"] = new Text(engine->renderer, FONT_ROBOTO, errorHandler,
    0, 0, 50, e4, RED);
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 25, skip); 

  // automatically win w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    engine->setState("boss"); 
  }, SDLK_2);
}

void BossScene::load() {
  State::load();
  // freeze
  static_cast<Enemy*>(images[ppl+"eBigAlien"])->freeze();
  static_cast<Enemy*>(images[ppl+"eMainBoss"])->freeze();
  static_cast<Character*>(images[ppl+"king"])->frozen = true;

  // bosses
  images[ppl+"eBigAlien"]->getDestRect()->h = 300;
  images[ppl+"eBigAlien"]->getDestRect()->w = 300;
  SDL_SetTextureAlphaMod(images[ppl+"eBigLF"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"eBigRF"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"BigBody"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"eBigAlien"]->getTexture(), 0);
  
  // position all the text
  for (it = images.find(top+"c0"); it != images.end(); it++) {
    if (it->first != top+"skip")
      SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
    auto center = getCenterForImage(it->second);
    it->second->setPosition(std::get<0>(center), std::get<1>(center));
    if (it->first == top+"skip") {
      it->second->pos_y = 0;
    } else if (it->first == top+"c1"
               || it->first == top+"c4") {
      it->second->pos_y -= 50;
    } else if (it->first == top+"c5") {
      it->second->pos_y -= 180;
    } else {
      it->second->pos_y -= 220;
    }
  }
}

void BossScene::update(double seconds) {
  // EPIC boss music
  if(musicSwitch == 1){
    audioHandler.play("boss");
    musicSwitch = 2;
  }
  PlayingState::update(seconds);

  std::string s = top + "c" + std::to_string(counter);
  std::string prev = top + "c" + std::to_string(counter-1);

  switch(event) {
    // king walks in
    case 0:
      if (images[ppl+"king"]->pos_x < map->width/2-16) {
        images[ppl+"king"]->velocityX = 100;
      } else {
        images[ppl+"king"]->velocityX = 0;
        event++;
      }
      break;
    case 1:
      if (images[ppl+"king"]->pos_y > map->height/2-25) {
        images[ppl+"king"]->velocityY = -100;
      } else {
        images[ppl+"king"]->velocityY = 0;
        event++;
      }
      break;
  }

  // text displayed with [n]
  if (event == 2) {
    SDL_SetTextureAlphaMod(images[s]->getTexture(), 255);
    if (counter > 0) {
      SDL_SetTextureAlphaMod(images[prev]->getTexture(), 0);
    }
    if (counter >= 4) {
      a0 = fadeIn(ppl+"eBigLF", a0, seconds, 2);
      a1 = fadeIn(ppl+"eBigRF", a1, seconds, 2);
      a2 = fadeIn(ppl+"BigBody", a2, seconds, 2);
      a3 = fadeIn(ppl+"eBigAlien", a3, seconds, 2);
    }
    if (counter >= 5) {
      a4 = fadeOut(ppl+"eMainBoss", a4, seconds, 2);
      if (images[ppl+"king"]->pos_y > map->height/2-100) {
        images[ppl+"king"]->velocityY = -100;
      } else {
        images[ppl+"king"]->velocityY = 0;
        finished = true;
      }
    }
    // increments to the next text
    eventHandler.addListener(SDL_KEYDOWN, [&](SDL_Event*) {
      raise = true;}, SDLK_n);
    if (raise) { raise = false; counter++; }
  }
  if (counter == 6) {
    engine->setState("boss");
  }
}

BossScene::~BossScene() {}
