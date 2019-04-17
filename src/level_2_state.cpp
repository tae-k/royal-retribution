/*
 * level_2_state.cpp
 */

#include "level_2_state.h"

Level_2_State::Level_2_State(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_2, TILES_TXT,
    &collisionDetector, &camera);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_2_ADD);

  isScene = false;

  setup();
  load();
}

void Level_2_State::setup() {
  //Camera Light
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT,
    errorHandler, 0, 0, false, true);
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, map->width/2 - 25, map->height - 180, false);
  // Hole
  images[ele+"hole"] = new Sprite(engine->renderer, BLACK_PIXEL,
    errorHandler, 0, 0, false);
  // King
  if (king != nullptr) {
    images[ppl+"king"] = king;
  } else {
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, map->width/2 - 16, 220, &eventHandler,
      &audioHandler, this);
  }
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, map->width/2 - 16, 200, static_cast<Sprite*>(images[ppl+"king"]),
    &eventHandler, &audioHandler, this);
  // Enemies
  std::ifstream file(LEVEL_2_E);
  int x = -1;
  int y = -1;
  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    std::string s = ppl+"enemy_"+std::to_string(num_enemies);
    images[s] = new Enemy(engine->renderer, ANI_FILENAME, errorHandler,
      16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 0);
    static_cast<Enemy*>(images[s])->followWhenClose(images[ppl + "king"],
      FOLLOW_RADIUS);
    num_enemies++;
  }
  file.close();
  // Lights
  num_lights = map->pushLights(images);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false);
  // Skip Pan Test
  images[top+"skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-170, HEIGHT-35, 30, "Press [m] to Skip Pan", ROYAL_GOLD);
  // Score
  images[top+"score"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH - 114, 2, 16, "SCORE = " + std::to_string(engine->score));
  // Level
  images[top+"level"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    WIDTH/2-48, 2, 16, "KING LEVEL "); 
  // Heart types
  images[top+"heart_4"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 160, 34, false, true);
  images[top+"heart_3"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 120, 34, false, true);
  images[top+"heart_2"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 80, 34, false, true);
  images[top+"heart_1"] = new Sprite(engine->renderer, HEART, errorHandler,
    32, 32, WIDTH - 40, 34, false, true);
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
  // set coin pos
  double coinPos1X = 0.0;
  double coinPos1Y = 0.0;
  double coinPos2X = 0.0;
  double coinPos2Y = 0.0;
  double coinPos3X = 0.0;
  double coinPos3Y = 0.0;
  double coinPos4X = 0.0;
  double coinPos4Y = 0.0;
  Sprite *C1 = nullptr;
  Sprite *C2 = nullptr;
  Sprite *C3 = nullptr;
  Sprite *C4 = nullptr;
  for(auto tile : map->additions){
    if(tile.image->isChest()){
      if(coinPos1X == 0.0 && coinPos1Y == 0.0){
        coinPos1X = tile.image->pos_x;
        coinPos1Y = tile.image->pos_y;
        C1 = tile.image;
      } else if(coinPos2X == 0.0 && coinPos2Y == 0.0) {
        coinPos2X = tile.image->pos_x;
        coinPos2Y = tile.image->pos_y;
        C2 = tile.image;
      } else if(coinPos3X == 0.0 && coinPos3Y == 0.0) {
        coinPos3X = tile.image->pos_x;
        coinPos3Y = tile.image->pos_y;
        C3 = tile.image;
      } else {
        coinPos4X = tile.image->pos_x;
        coinPos4Y = tile.image->pos_y;
        C4 = tile.image;
      }
    }
  }
  // add coins
  images[add+"coin"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPos1X, coinPos1Y, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin"])->setPair(C1);
  images[add+"coin2"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPos2X, coinPos2Y, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin2"])->setPair(C2);
  images[add+"coin3"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPos3X, coinPos3Y, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin3"])->setPair(C3);
  images[add+"coin4"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPos4X, coinPos4Y, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin4"])->setPair(C4);
  // add food in some enemies
  setupFood();
  // instuctions
  setupInstruct();
  // FPS Counter 
  images[add+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");

  // go to floor 3 w/ '2'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->pos_x = static_cast<Character*>(images[ppl+"king"])->startingX;
    images[ppl+"king"]->pos_y = static_cast<Character*>(images[ppl+"king"])->startingY;
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    static_cast<Character*>(images[ppl+"king"])->dir = "down";
    king = images[ppl+"king"];
    Character::currFloor = 3;
    if (Character::highestFloor == 2) Character::highestFloor = 3;
    engine->setState("level_3");
   }, SDLK_2);
  // go to floor 1 w/ '1'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    Character::currFloor = 1;
    engine->setState("playing");
   }, SDLK_1);
}

void Level_2_State::load() {
  PlayingState::load(); 
  //sets pickup texture to see thru when in chests
  SDL_SetTextureAlphaMod(images[add+"coin"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin2"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin3"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin4"]->getTexture(), 0);
}
void Level_2_State::update(double seconds) {
  PlayingState::update(seconds);

  auto character = static_cast<Character*>(images[ppl+"king"]);
  auto coin2 = static_cast<Pickup*>(images[add+"coin2"]);
  if(static_cast<Sprite*>(images[add+"coin2"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin2"])->pair
    && !coin2->isPickedUp())
  {

    character->pickUp(coin2);

    static_cast<Sprite*>(images[add+"coin2"])->pair = character;
    engine->score += 1000;

    activateInstructionText(chestNum);
  }
  auto coin3 = static_cast<Pickup*>(images[add+"coin3"]);
  if(static_cast<Sprite*>(images[add+"coin3"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin3"])->pair
    && !coin3->isPickedUp())
  {

    character->pickUp(coin3);

    static_cast<Sprite*>(images[add+"coin3"])->pair = character;
    engine->score += 1000;

    activateInstructionText(chestNum);
  }
  auto coin4 = static_cast<Pickup*>(images[add+"coin4"]);
  if(static_cast<Sprite*>(images[add+"coin4"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin4"])->pair
    && !coin4->isPickedUp())
  {

    character->pickUp(coin4);

    static_cast<Sprite*>(images[add+"coin4"])->pair = character;
    engine->score += 1000;

    activateInstructionText(chestNum);
  }
  auto coin = static_cast<Pickup*>(images[add+"coin"]);
  if(static_cast<Sprite*>(images[add+"coin"])->pair->pair
    == static_cast<Sprite*>(images[add+"coin"])->pair
    && !coin->isPickedUp())
  {
    character->pickUp(coin);

    static_cast<Sprite*>(images[add+"coin"])->pair = character;
    engine->score += 1000;
    activateInstructionText(chestNum);
  }

  // changes state to level 3
  if (images[ppl+"king"]->pos_x < map->width/2 + 25
      && images[ppl+"king"]->pos_x + images[ppl+"king"]->getDestRect()->w
        > map->width/2 - 25
      && images[ppl+"king"]->pos_y + images[ppl+"king"]->getDestRect()->h
        > map->height - 180)
  {
    images[ppl+"king"]->pos_x = static_cast<Character*>(images[ppl+"king"])->startingX;
    images[ppl+"king"]->pos_y = static_cast<Character*>(images[ppl+"king"])->startingY;
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    Character::currFloor = 3;
    if (Character::highestFloor == 2) Character::highestFloor = 3;
    engine->setState("level_3");
  }
}

Level_2_State::~Level_2_State() {}
