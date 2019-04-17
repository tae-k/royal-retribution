/*
 * playing_state.cpp
 */

#include "playing_state.h"

PlayingState::PlayingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {
}

void PlayingState::setup() {
  //Camera Light
  images[add+"cLight"] = new Sprite(engine->renderer, CAMERA_LIGHT,
    errorHandler, 0, 0, false, true);
  // Stairs 
  images[ele+"stairs"] = new Sprite(engine->renderer, STAIRS_FILENAME,
    errorHandler, map->width/2 - 16, map->height - 16, false);
  // Hole
  images[ele+"hole"] = new Sprite(engine->renderer, BLACK_PIXEL,
    errorHandler, 0, 0, false);
  SDL_SetTextureBlendMode(images[ele+"hole"]->getTexture(),SDL_BLENDMODE_BLEND);
  // Player 
  if (king != nullptr) {
    images[ppl+"king"] = king;
  } else {
    images[ppl+"king"] = new Character(engine->renderer, ANI_FILENAME,
      errorHandler, 16, 25, 125, 118, &eventHandler, &audioHandler, this);
  }
  // Sword
  images[ppl+"sword"] = new Sword(engine->renderer, SWORD, errorHandler,
    56, 56, 0, 0, static_cast<Sprite*>(images[ppl+"king"]), &eventHandler,
    &audioHandler, this);
  // Enemies
  std::ifstream file(LEVEL_1_E);
  int x = -1;
  int y = -1;
  while ((file >> y) && y != -1 && (file >> x) && x != -1) {
    std::string s = ppl+"enemy_"+std::to_string(num_enemies);
    images[s] = new Enemy(engine->renderer, ANI_FILENAME, errorHandler,
      16, 25, (x-1) * TILE_DIM, (y-1) * TILE_DIM, 0, 0);
    num_enemies++;
  }
  file.close();
  // Lights
  num_lights = map->pushLights(images);
  // Black
  images[add+"black"] = new Sprite(engine->renderer, BLACK_PIXEL, errorHandler,
    0, 0, false, true);
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
  double coinPosY = 0.0;
  Sprite *C1 = nullptr;
  Sprite *C2 = nullptr;
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
      }
    }
  }
  // add key
  images[add+"key"] = new Pickup(engine->renderer, KEY, errorHandler,
    32, 32, keyPosX, keyPosY, false, false, keyNum);
  static_cast<Sprite*>(images[add+"key"])->setPair(C1);
  // add coin
  images[add+"coin"] = new Pickup(engine->renderer, COIN, errorHandler,
    32, 32, coinPosX, coinPosY, false, false, coinNum);
  static_cast<Sprite*>(images[add+"coin"])->setPair(C2);
  // add food
  images[add+"food"] = new Pickup(engine->renderer, MILK, errorHandler,
    32, 32, 0, 0, false, true, foodNum);
  // add food in some enemies
  while (num_food < 3) {
    int r = rand() % num_enemies;
    std::string eStr = ppl+"enemy_"+std::to_string(r);
    std::string fStr = ppl+"food_"+std::to_string(num_food);
    if (images[fStr] == nullptr) {
      images[fStr] = new Pickup(engine->renderer, FOOD, errorHandler, 32, 32,
        images[eStr]->pos_x, images[eStr]->pos_y, false, false, healthNum);
      static_cast<Enemy*>(images[eStr])->addFood();
      static_cast<Sprite*>(images[fStr])->setPair((Sprite*)images[eStr]);
      num_food++;
    }
  }
  // instuctions
  setupInstruct();
  // FPS Counter 
  images[add+"fps"] = new Text(engine->renderer, FONT_FILENAME,  errorHandler,
    2, 2, 16, "FPS: ");
}

void PlayingState::enter() {
  if (images.find(top + "sta_bar") != images.end()) 
    static_cast<Character*>(images[ppl + "king"])->staminaBar = images[top + "sta_bar"];

  if (images.find(top + "exp_bar") != images.end())
    static_cast<Character*>(images[ppl + "king"])->setExpBar(images[top + "exp_bar"]);
}

void PlayingState::setupFood() {
  while (num_food < 3) {
    int r = rand() % num_enemies;
    std::string eStr = ppl+"enemy_"+std::to_string(r);
    std::string fStr = ppl+"food_"+std::to_string(num_food);
    if (!static_cast<Enemy*>(images[eStr])->hasFood()) {
      images[fStr] = new Pickup(engine->renderer, FOOD, errorHandler, 32, 32,
        images[eStr]->pos_x, images[eStr]->pos_y, false, false, healthNum);
      static_cast<Enemy*>(images[eStr])->addFood();
      static_cast<Sprite*>(images[fStr])->setPair((Sprite*)images[eStr]);
      num_food++;
    }
  }
}

void PlayingState::setupInstruct() {
  images[top+"dkInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU FOUND A KEY! It opens SPECIAL DOORS with [e]. Press [r] to clear the text.");
  images[top+"fInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU DRANK MILK! You now have an extra heart. Press [r] to clear the text.");
  images[top+"FInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "THE ENEMY DROPPED FOOD! Pick it up to restore a full heart. Press [r] to clear the text.");
  images[top+"hInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU FELL INTO A HOLE! You are now being sent to the previous floor. Press [r] to clear the text.");
  images[top+"tInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU ARE TRAPPED! Kill an enemy to escape! Press [r] to clear the text.");
  images[top+"cInstruct"] = new Text(engine->renderer, FONT_ROBOTO,
    errorHandler, 0, 0, 25, "YOU FOUND A COIN! Your score has increased. Press [r] to clear the text.");
}

void PlayingState::load() {
  State::load();

  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    skipPan = true; }, SDLK_m);

  for (int i = 0; i < num_lights; i++) {
    std::string s = add+"light_"+std::to_string(i);

    images[s]->getDestRect()->w = 3*TILE_DIM;
    images[s]->getDestRect()->h = 3*TILE_DIM;

    SDL_SetTextureBlendMode(images[s]->getTexture(),SDL_BLENDMODE_ADD);
    if (SDL_SetTextureAlphaMod(images[s]->getTexture(), 80) < 0)
      errorHandler->quit(__func__, SDL_GetError());
  }

  images[add+"black"]->getDestRect()->w = map->width;
  images[add+"black"]->getDestRect()->h = map->height;

  SDL_SetTextureBlendMode(images[add+"black"]
    ->getTexture(), SDL_BLENDMODE_ADD);
  if (SDL_SetTextureAlphaMod(images[add+"black"]->getTexture(), 150) < 0) {
    errorHandler->quit(__func__, SDL_GetError());
  }
  images[add+"cLight"]->getDestRect()->w = WIDTH;
  images[add+"cLight"]->getDestRect()->h = HEIGHT;

  SDL_SetTextureBlendMode(images[add+"cLight"]
    ->getTexture(),SDL_BLENDMODE_MOD);

  if (isScene) return;

  // set size of stairs & stam & exp
  images[ele+"stairs"]->getDestRect()->h = 50;
  images[ele+"stairs"]->getDestRect()->w = 50;
  images[top+"sta_box"]->getDestRect()->h = 32;
  images[top+"sta_box"]->getDestRect()->w = 150;
  images[top+"sta_bar"]->getDestRect()->h = 24;
  images[top+"sta_bar"]->getDestRect()->w = 146;
  images[top+"exp_box"]->getDestRect()->h = 32;
  images[top+"exp_box"]->getDestRect()->w = 150;
  images[top+"exp_bar"]->getDestRect()->h = 24;
  images[top+"exp_bar"]->getDestRect()->w = 148;

  // setup hole
  images[ele+"hole"]->getDestRect()->h = 22;
  images[ele+"hole"]->getDestRect()->w = 50;
  SDL_SetTextureBlendMode(images[ele+"hole"]->getTexture(),SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(images[ele+"hole"]->getTexture(), 0);

  // center some of the textures
  for (it = images.begin(); it != images.end(); it++) {
    if (it->first == "3skip") {
      auto center = getCenterForImage(it->second);
      it->second->setPosition(std::get<0>(center), HEIGHT-35);
    } else if (it->first == "3level") {
      auto center = getCenterForImage(it->second); 
      it->second->setPosition(std::get<0>(center), 0);
    } else if (it->first == "3dkInstruct"
               || it->first == "3cInstruct"
               || it->first == "3hInstruct"
               || it->first == "3tInstruct"
               || it->first == "3fInstruct"
               || it->first == "3FInstruct")
    {
      auto center = getCenterForImage(it->second);
      it->second->setPosition(std::get<0>(center), std::get<1>(center)-80);
    }
  }
  deactivateInstructionText();

  camera.setPosition(images[ele+"stairs"]);


  /****************
  CHEATS
  ****************/
  // automatically lose w/ '0'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   static_cast<Character*>(images[ppl+"king"])->hearts = 0;
   pause();
   if (static_cast<Character*>(images[ppl+"king"])->isDead())
     engine->setState("lose");
  }, SDLK_0);
  // pause w/ 'p'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
   if (skipPan) pause(); }, SDLK_p);
  //Delete instruction text / resume by pressing 'r'
  eventHandler.addListener(SDL_KEYUP, [&](SDL_Event*) {
    if (isPaused()) resume();
    deactivateInstructionText(); 
  }, SDLK_r);
}

void PlayingState::update(double seconds) {
  if (isScene) {
    updateLights(seconds);
    State::update(seconds);
    return;
  }

  timer += seconds;

  int kingLevel = static_cast<Character*>(images[ppl+"king"])->level;

  if(Mix_PausedMusic() == 1){audioHandler.play("theme");}

  /***************
     UPDATE TEXT
  ***************/

  // update FPS Display
  if (timer > 1) {
    delete images[top+"fps"];
    images[top+"fps"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      2, 2, 16, "FPS: " + std::to_string((int)(1/renderSeconds)));
    images[top+"fps"]->load();
    timer = 0;
  }
  // update Level
  if (currentLevel != kingLevel) {
    delete images[top+"level"];
    images[top+"level"] = new Text(engine->renderer, FONT_FILENAME,
      errorHandler, WIDTH/2-48, 2, 16, "KING LEVEL "+std::to_string(kingLevel));
    images[top+"level"]->load();
    currentLevel = kingLevel;
  }
  // update Score
  if (currentScore != engine->score) {
    delete images[top+"score"];
    images[top+"score"] = new Text(engine->renderer, FONT_FILENAME,
      errorHandler, WIDTH-114, 2, 16, "SCORE = "+std::to_string(engine->score));  
    images[top+"score"]->load();
    currentScore = engine->score;
  }


  /********************
  FALLING
  ********************/
  if(PlayingState::fallen == 1){
    a0 = fadeIn(ele+"hole", a0, seconds, 6);
    a1 = fadeOut(ppl+"king", a1, seconds, 1);
    activateInstructionText(holeNum);
  }
  if(PlayingState::fallen == 2){
    SDL_SetTextureAlphaMod(images[ppl+"king"]->getTexture(), 255);
    fallen = 0;
    if(Character::currFloor == 2){
      Character::currFloor = 1;
      engine->setState("playing");
    }
    else if(Character::currFloor == 3){
      Character::currFloor = 2;
      engine->setState("level_2");
    }
    else if(Character::currFloor == 4){
      Character::currFloor = 3;
      engine->setState("level_3");
    }
  }


  /********************
  HEARTS & LIGHTS
  *********************/
  // updates Health & Lights
  updateHearts();
  updateLights(seconds);
  State::update(seconds);


  /********************
  FOOD INSTRUCTION
  *********************/
  // prints food instruction
  if (!foodInstructionGiven) {
    for (int i = 0; i < num_enemies; i++) {
      std::string s = ppl+"enemy_"+std::to_string(i);
      if (static_cast<Enemy*>(images[s])->isDead()
          && static_cast<Enemy*>(images[s])->hasFood())
      {
        activateInstructionText(healthTextNum);
        foodInstructionGiven = true;
      }
    }
  }
  
 
  /********************
  DIE
  *********************/
  // changes state to Lose
  if (static_cast<Character*>(images[ppl+"king"])->hearts <= 0) {
    pause();
    if (static_cast<Character*>(images[ppl+"king"])->isDead())
     engine->setState("lose");
  }

  /********************
  CAMERA PAN
  *********************/
  timeSinceStart += seconds;
  if (timeSinceStart > PAN_DELAY_SECONDS || skipPan) {
    if (!skipPan && !camera.pan(images[ppl+"king"], seconds)) {
      static_cast<Character*>(images[ppl+"king"])->frozen = true;
    } else {
      SDL_SetTextureAlphaMod(images[top+"skip"]->getTexture(), 0);
      static_cast<Character*>(images[ppl+"king"])->frozen = false;
      camera.setCharacter(static_cast<Character*>(images[ppl+"king"]));
      skipPan = true;
    }
  }
}

void PlayingState::activateInstructionText(int instruct){
  if(instruct == doorKeyNum && instrGiven % doorKeyNum != 0){ 
    pause();

    SDL_SetTextureAlphaMod(images[top+"dkInstruct"]->getTexture(), 255);
    PlayingState::instrGiven *= doorKeyNum;
  }
  if(instruct == holeNum){ 
    pause();

    images[ele+"hole"]->pos_x = images[ppl+"king"]->pos_x-9;
    images[ele+"hole"]->pos_y = images[ppl+"king"]->pos_y+39;
    SDL_SetTextureAlphaMod(images[top+"hInstruct"]->getTexture(), 255);
  }
  if(instruct == trapNum && instrGiven % trapNum != 0){ 
    pause();

    SDL_SetTextureAlphaMod(images[top+"tInstruct"]->getTexture(), 255); 
    PlayingState::instrGiven *= trapNum;
  }
  if(instruct == healthTextNum && instrGiven % healthTextNum != 0){
    pause();

    SDL_SetTextureAlphaMod(images[top+"FInstruct"]->getTexture(), 255);
    PlayingState::instrGiven *= healthTextNum;
  }
  if(instruct == foodTextNum){
    pause();

    SDL_SetTextureAlphaMod(images[top+"fInstruct"]->getTexture(), 255);
    PlayingState::instrGiven *= foodTextNum;
  }
  if(instruct == chestNum && instrGiven % chestNum != 0){ 
    pause();

    SDL_SetTextureAlphaMod(images[top+"cInstruct"]->getTexture(), 255); 
    PlayingState::instrGiven *= chestNum;
  }
}

void PlayingState::deactivateInstructionText() {
  a0 = 0;
  a1 = 255;
  SDL_SetTextureAlphaMod(images[ele+"hole"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[top+"tInstruct"]->getTexture(), 0); 
  SDL_SetTextureAlphaMod(images[top+"hInstruct"]->getTexture(), 0); 
  SDL_SetTextureAlphaMod(images[top+"cInstruct"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[top+"dkInstruct"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[top+"fInstruct"]->getTexture(), 0);
  SDL_SetTextureAlphaMod(images[top+"FInstruct"]->getTexture(), 0);
  if(PlayingState::fallen == 1){
    PlayingState::fallen = 2;
    static_cast<Character*>(images[ppl+"king"])->
      setPosition(static_cast<Character*>(images[ppl+"king"])->startingX,
                  static_cast<Character*>(images[ppl+"king"])->startingY);
    static_cast<Character*>(images[ppl+"king"])->falling = false;
  }
}

//Health update with extra heart
void PlayingState::updateHeartsPlus(){
  static_cast<Character*>(images[ppl+"king"])->maxHearts = 8; 
  SDL_SetTextureAlphaMod(images[top+"heart_4"]->getTexture(), 255);
  switch(static_cast<Character*>(images[ppl+"king"])->hearts) {
    case 8:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;

    case 7:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;

    case 6:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_4"])->setSrcRect(40, 0, 32, 32);
      break;
  }
}
//health update with normal hearts
void PlayingState::updateHearts(){
  SDL_SetTextureAlphaMod(images[top+"heart_4"]->getTexture(), 0);
  for(unsigned n =0; n < Character::activePowerups.size(); n++){
    if(Character::activePowerups[n] == foodNum){
      updateHeartsPlus();
      return;
    }
  }

  switch(static_cast<Character*>(images[ppl+"king"])->hearts) {
    case 6:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 5:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 4:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(0, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 3:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(40, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 2:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(0, 0, 32, 32);
      break;
    case 1:
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(40, 0, 32, 32);
      break;
    default: 
      static_cast<Sprite*>(images[top+"heart_1"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_2"])->setSrcRect(80, 0, 32, 32);
      static_cast<Sprite*>(images[top+"heart_3"])->setSrcRect(80, 0, 32, 32);
     break; 
  }
}

void PlayingState::updateLights(double seconds) {
  for (int i = 0; i < num_lights; i++) {
    std::string s = add+"light_"+std::to_string(i);
    Sprite* light = static_cast<Sprite*>(images[s]);
    doubleRect lightRect = light->getDoubleRect();
    doubleRect cameraRect = Util::toDoubleRect(camera.getRect());

    if (Util::isIntersecting(&lightRect, &cameraRect))
      light->animate(seconds, 0, 3, 5.0);
  }
}

void PlayingState::cleanup() {
  if (images[ppl + "king"] != nullptr && static_cast<Character*>(images[ppl + "king"])->staminaBar != nullptr) {
    delete static_cast<Character*>(images[ppl + "king"])->staminaBar;
    static_cast<Character*>(images[ppl + "king"])->staminaBar = nullptr;
  }

  if (images[ppl + "king"] != nullptr && static_cast<Character*>(images[ppl + "king"])->expBar != nullptr) {
    delete static_cast<Character*>(images[ppl + "king"])->expBar;
    static_cast<Character*>(images[ppl + "king"])->expBar = nullptr;
  }
    
  State::cleanup();
}

PlayingState::~PlayingState() {}

int PlayingState::instrGiven = 1;

int PlayingState::fallen = 0;
