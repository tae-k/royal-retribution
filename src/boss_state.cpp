/*
 * playing_state.cpp
 */

#include "boss_state.h"
#include "big_alien.h"
#include "main_boss.h"
#include "highscore_state.h"

BossState::BossState(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, FINAL_LEVEL, TILES_TXT,
    &collisionDetector, &camera);

  isScene = false;

  setup();
  load();
}

void BossState::setup() {
  // Skip Pan Test
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-170, HEIGHT-35, 30, "Press [m] to Skip Pan", ROYAL_GOLD);

  // Player 
  if (king != nullptr)
    images[ppl+"king"] = king;
  else
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, map->width/2-16, map->height/2-100,
      &eventHandler, &audioHandler, this);
    static_cast<Character*>(images[ppl+"king"])->dir = "up";
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, map->width/2, map->height/2, static_cast<Sprite*>
    (images[ppl+"king"]), &eventHandler, &audioHandler, this);

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
  //Laser beam
  images[ppl+"zBigBeam"] = new Sprite(engine->renderer, BIG_BEAM,
    errorHandler, 72, 150, map->width/2 - 100, map->height - 260, true);
  static_cast<Sprite*>(images[ppl+"zBigBeam"])->setLaser(true);
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setBeam(static_cast<Sprite*>(images[ppl+"zBigBeam"])); 
  // set body parts
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setHands(static_cast<Hand*>(images[ppl+"eBigLF"]),
      static_cast<Hand*>(images[ppl+"eBigRF"]));
  static_cast<BigAlien*>(images[ppl+"eBigAlien"])
    ->setBody(static_cast<Sprite*>(images[ppl+"BigBody"]));

  // Main Boss
  images[ppl+"eMainBoss"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 - 16, map->height/2 + 70, 0, 0, 9, map);
  images[ppl+"eclone1"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 - 36, map->height/2 + 70, 0, 0, 9, map);
  images[ppl+"eclone2"] = new MainBoss(engine->renderer, ANI_FILENAME,
    errorHandler, 16, 25, map->width/2 + 36, map->height/2 + 70, 0, 0, 9, map);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->clone1 = static_cast<MainBoss*>(images[ppl+"eclone1"]);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->clone2 = static_cast<MainBoss*>(images[ppl+"eclone2"]);
  static_cast<MainBoss*>(images[ppl+"eclone1"])
    ->setClone(true);
  static_cast<MainBoss*>(images[ppl+"eclone2"])
    ->setClone(true);
  static_cast<Enemy*>(images[ppl+"eMainBoss"])
    ->followWhenClose(images[ppl + "king"], FOLLOW_RADIUS);
  static_cast<Enemy*>(images[ppl+"eMainBoss"])
    ->followWhenClose(images[ppl + "king"],
      400);
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])
    ->target = static_cast<Sprite*>(images[ppl + "king"]);
  //projectile
  images[ppl+"fireball0"] = new Projectile(engine->renderer, TILES_IMG,
    errorHandler, 32, 32, map->width/2 - 16, map->height/2 + 70, 1000, 3, true,static_cast<Sprite*>(images[ppl + "eMainBoss"]));
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])->fireballs[0] = static_cast<Projectile*>(images[ppl + "fireball0"]);
  images[ppl+"fireball1"] = new Projectile(engine->renderer, TILES_IMG,
    errorHandler, 32, 32, map->width/2 - 16, map->height/2 + 70, 1000, 3, true,static_cast<Sprite*>(images[ppl + "eMainBoss"]));
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])->fireballs[1] = static_cast<Projectile*>(images[ppl + "fireball1"]);
  images[ppl+"fireball2"] = new Projectile(engine->renderer, TILES_IMG,
    errorHandler, 32, 32, map->width/2 - 16, map->height/2 + 70, 1000, 3, true,static_cast<Sprite*>(images[ppl + "eMainBoss"]));
  static_cast<MainBoss*>(images[ppl+"eMainBoss"])->fireballs[2] = static_cast<Projectile*>(images[ppl + "fireball2"]);
  // Score
  images[top+"score"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH - 114, 2, 16, "SCORE = " + std::to_string(engine->score));
  // Level
  images[top+"level"] = new Text(engine->renderer, FONT_FILENAME,
    errorHandler, WIDTH/2-48, 2, 16, "KING LEVEL "); 
  // Heart types
  images[top+"heart_1"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 40, 34, false, true);
  images[top+"heart_2"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 80, 34, false, true);
  images[top+"heart_3"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 120, 34, false, true);
  images[top+"heart_4"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 160, 34, false, true);
  // Stamina
  images[top+"sta_box"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    0, 34, false, true);
  images[top+"sta_bar"] = new Sprite(engine->renderer, STA_BAR, errorHandler,
    2, 38, false, true);
  // Experience
  images[top+"exp_box"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    WIDTH/2 - 75, 34, false, true);
  images[top+"exp_bar"] = new Sprite(engine->renderer, EXP_BAR, errorHandler,
    WIDTH/2 - 72, 38, false, true);
  //fps
  images[top+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");

  // automatically win w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    HighscoreState::saveScore(engine->score);
    engine->setState("win"); 
  }, SDLK_2);
}

void BossState::load() {
  State::load();
  // sta and exp
  images[top+"sta_box"]->getDestRect()->h = 32;
  images[top+"sta_box"]->getDestRect()->w = 150;
  images[top+"sta_bar"]->getDestRect()->h = 24;
  images[top+"sta_bar"]->getDestRect()->w = 146;
  images[top+"exp_box"]->getDestRect()->h = 32;
  images[top+"exp_box"]->getDestRect()->w = 150;
  images[top+"exp_bar"]->getDestRect()->h = 24;
  images[top+"exp_bar"]->getDestRect()->w = 148;
  // bosses
  camera.setPosition(images[ppl+"BigBody"]);
  images[ppl+"eBigAlien"]->getDestRect()->h = 300;
  images[ppl+"eBigAlien"]->getDestRect()->w = 300;
  SDL_SetTextureAlphaMod(images[ppl+"eclone1"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"eclone2"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"eMainBoss"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[ppl+"zBigBeam"]->getTexture(), 0);
}

void BossState::update(double seconds) {
  // EPIC boss music
  if(musicSwitch == 1){
    audioHandler.play("boss");
    musicSwitch = 2;
  }
  if (!skipPan) {
    static_cast<Character*>(images[ppl+"king"])->frozen = true;
    static_cast<Enemy*>(images[ppl+"eBigAlien"])->freeze();
  } else {
    static_cast<Character*>(images[ppl+"king"])->frozen = false;
    static_cast<Enemy*>(images[ppl+"eBigAlien"])->thaw();
  }

  PlayingState::update(seconds);

  // fade out the head with the body
  if (static_cast<BigAlien*>(images[ppl+"eBigAlien"])->isDying())
    fade = fadeOut(ppl+"BigBody", fade, seconds, 1.0);
  // if Big Alien dies
  if(static_cast<Enemy*>(images[ppl+"eBigAlien"])->isDead() && thePhase < 1){
    thePhase = static_cast<MainBoss*>(images[ppl+"eMainBoss"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone1"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone2"])->changePhase(); 
    SDL_SetTextureAlphaMod(images[ppl+"eMainBoss"]->getTexture(), 255);
    SDL_SetTextureAlphaMod(images[ppl+"eclone1"]->getTexture(), 255);
    SDL_SetTextureAlphaMod(images[ppl+"eclone2"]->getTexture(), 255);
    delete images[ppl+"zBigBeam"];
    images[ppl+"zBigBeam"] = new Sprite(engine->renderer, BIG_BEAM,
    errorHandler, 1, 1, map->width/2 - 100, map->height - 260, true);
    //images[ppl+"zBigBeam"] = nullptr;
  }
  if(static_cast<Boss_Enemy*>(images[ppl+"eMainBoss"])->getHp() == 6 && thePhase < 2){
    thePhase = static_cast<MainBoss*>(images[ppl+"eMainBoss"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone1"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone2"])->changePhase(); 
  
  }
  if(static_cast<Boss_Enemy*>(images[ppl+"eMainBoss"])->getHp() == 2){
    thePhase++;
  }
  if(thePhase == 3){
    static_cast<MainBoss*>(images[ppl+"eMainBoss"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone1"])->changePhase(); 
    static_cast<MainBoss*>(images[ppl+"eclone2"])->changePhase();
  }   
  if(static_cast<Enemy*>(images[ppl+"eMainBoss"])->isDead()){
    HighscoreState::saveScore(engine->score);
    engine->setState("win"); 
  }
}

BossState::~BossState() {}
