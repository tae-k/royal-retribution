/*
 * playing_state.cpp
 */

#include "level_1_state.h"

Level_1_State::Level_1_State(Engine* engine, ErrorHandler* errorHandler)
  : PlayingState(engine, errorHandler) {

  map = new Map(engine->renderer, errorHandler, LEVEL_1, TILES_TXT,
    &collisionDetector, &camera);
  map->loadSecondTextures(TILES_ADD);
  map->loadSecondLayout(LEVEL_1_ADD);

  isScene = false;

  setup();
  load();
}

void Level_1_State::setup() {
  //Camera Light
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT,
    errorHandler, 0, 0, false, true);
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, map->width/2 - 25, map->height - 115, false);
  // Hole
  images[ele+"hole"] = new Sprite(engine->renderer, BLACK_PIXEL,
    errorHandler, 0, 0, false);
  // Player 
  if (king != nullptr) {
    images[ppl+"king"] = king;
  } else {
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, map->width/2 - 16, 440, &eventHandler,
      &audioHandler, this);
  }
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, map->width/2, 400, static_cast<Sprite*>(images[ppl+"king"]),
    &eventHandler, &audioHandler, this);
  // Enemies
  std::ifstream file(LEVEL_1_E);
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
  // set key and coin pos
  double keyPosX = 0.0;
  double keyPosY = 0.0;
  double coinPosX = 0.0;
  double coin2PosY = 0.0;
  double coin2PosX = 0.0;
  double coin3PosY = 0.0;
  double coin3PosX = 0.0;
  double coinPosY = 0.0;
  double foodPosX = 0.0;
  double foodPosY = 0.0;
  Sprite *C1 = nullptr;
  Sprite *C2 = nullptr;
  Sprite *C3 = nullptr;
  Sprite *C4 = nullptr;
  Sprite *C5 = nullptr;
  for(auto tile : map->additions){
    if(tile.image->isChest()){
      if(keyPosX == 0.0 && keyPosY == 0.0){
        keyPosX = tile.image->pos_x;
        keyPosY = tile.image->pos_y;
        C1 = tile.image;
      } else if(coinPosX == 0.0 && coinPosY == 0.0) {
        coinPosX = tile.image->pos_x;
        coinPosY = tile.image->pos_y;
        C2 = tile.image;
      } else if(coin2PosX == 0.0 && coin2PosY == 0.0) {
        coin2PosX = tile.image->pos_x;
        coin2PosY = tile.image->pos_y;
        C3 = tile.image;
      } else if(coin3PosX == 0.0 && coin3PosY == 0.0) {
        coin3PosX = tile.image->pos_x;
        coin3PosY = tile.image->pos_y;
        C4 = tile.image;
      } else {
        foodPosX = tile.image->pos_x;
        foodPosY = tile.image->pos_y;
        C5 = tile.image;
      }
    }
  }
  // add key
  images[add+"key"] = new Pickup(engine->renderer, KEY, errorHandler,
    32, 32, keyPosX, keyPosY, false, false, keyNum);
  static_cast<Sprite*>(images[add+"key"])->setPair(C1);
  // add coins
  images[add+"coin"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPosX, coinPosY, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin"])->setPair(C2);
  images[add+"coin2"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coin2PosX, coin2PosY, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin2"])->setPair(C3);
  images[add+"coin3"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coin3PosX, coin3PosY, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin3"])->setPair(C4);
  // add food
  images[add+"food"] = new Pickup(engine->renderer, MILK, errorHandler,
    32, 32, foodPosX, foodPosY, false, true, foodNum);
  static_cast<Sprite*>(images[add+"food"])->setPair(C5);
  // add food in some enemies
  setupFood();
  // instuctions
  setupInstruct();
  // go to floor 2
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    images[ppl+"king"]->pos_x = static_cast<Character*>(images[ppl+"king"])->startingX;
    images[ppl+"king"]->pos_y = static_cast<Character*>(images[ppl+"king"])->startingY;
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    static_cast<Character*>(images[ppl+"king"])->dir = "down";
    king = images[ppl+"king"];
    Character::currFloor = 2;
    if (Character::highestFloor == 1) Character::highestFloor = 2;
    engine->setState("level_2");
   }, SDLK_2);
}
void Level_1_State::load() {
  PlayingState::load(); 
  //sets pickup texture to see thru when in chests
  SDL_SetTextureAlphaMod(images[add+"coin"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin2"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"coin3"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"key"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[add+"food"]->getTexture(), 0);
}
void Level_1_State::update(double seconds) {

  PlayingState::update(seconds);

  //food chest
  if(static_cast<Sprite*>(images[add+"food"])->pair->pair
    == static_cast<Sprite*>(images[add+"food"])->pair
    && !static_cast<Pickup*>(images[add+"food"])->isPickedUp())
  {

    static_cast<Character*>(images[ppl+"king"])->pickUp(
      static_cast<Pickup*>(images[add+"food"]));

    static_cast<Sprite*>(images[add+"food"])->pair
      = static_cast<Character*>(images[ppl+"king"]);

    engine->score += 1000;
    Character::hearts = 8;
    activateInstructionText(foodTextNum);

  }
  //key chest
  auto character = static_cast<Character*>(images[ppl+"king"]);
  auto key = static_cast<Pickup*>(images[add+"key"]);
  if(static_cast<Sprite*>(images[add+"key"])->pair->pair
    == static_cast<Sprite*>(images[add+"key"])->pair
    && !key->isPickedUp())
  {

    character->pickUp(key);

    static_cast<Sprite*>(images[add+"key"])->pair = character;

    activateInstructionText(doorKeyNum);
  }
  //coin chest
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

  // changes state to Level_2
  if (images[ppl+"king"]->pos_x < map->width/2 + 25
      && images[ppl+"king"]->pos_x + images[ppl+"king"]->getDestRect()->w
        > map->width/2 - 25
      && images[ppl+"king"]->pos_y + images[ppl+"king"]->getDestRect()->h
        > map->height - 115)
  {
    images[ppl+"king"]->pos_x = static_cast<Character*>(images[ppl+"king"])->startingX;
    images[ppl+"king"]->pos_y = static_cast<Character*>(images[ppl+"king"])->startingY;
    images[ppl+"king"]->velocityX = 0;
    images[ppl+"king"]->velocityY = 0;
    static_cast<Character*>(images[ppl+"king"])->dir = "down";
    king = images[ppl+"king"]; 
    Character::currFloor = 2;
    if (Character::highestFloor == 1) Character::highestFloor = 2;
    engine->setState("level_2");
  }
}

Level_1_State::~Level_1_State() {}
