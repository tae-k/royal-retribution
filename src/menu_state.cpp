#include "menu_state.h"
#include "audio.h"

MenuState::MenuState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  setup();
  load();

  selectedIndex = 3;
}

void MenuState::setup() {
  // Create all the images needed for the text, spaceship pointer & background.
  images["0home"] = new Image(engine->renderer, CASTLE_FILENAME, errorHandler);
  images["1ship"] = new Sprite(engine->renderer, SHIP_FILENAME, errorHandler,
    0, 0, false);
  images["1Tit"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 90, "ROYAL RETRIBUTION", ROYAL_GOLD);
  images["1NG"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "New Game");
  images["1HS"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "High Scores");
  images["1CR"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "Credits");
  images["1QT"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 50,
    50, 40, "Quit");
  images["1BR"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 80,
    500, 40, "Brightness: ");
  images["1_12"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 80,
    540, 20, "(Use keys 1 and 2 to increase/decrease)");
  images["1Bnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    350, 500, 40, "100%");
  images["1VM"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 800,
    500, 40, "Volume: ");
  images["1_90"] = new Text(engine->renderer, FONT_FILENAME, errorHandler, 800,
    540, 20, "(Use keys 9 and 0 to increase/decrease)");
  images["1Vnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    1000, 500, 40, "100%");
}

void MenuState::load() {
  State::load();

  // change ship size
  images["1ship"]->getDestRect()->w = 50;
  images["1ship"]->getDestRect()->h = 38;

  // Center all the text (lines 35-47)
  auto center = getCenterForImage(images["1Tit"]);
  images["1Tit"]->setPosition(std::get<0>(center), std::get<1>(center) - 220);

  center = getCenterForImage(images["1NG"]);
  images["1NG"]->setPosition(std::get<0>(center), std::get<1>(center) - 40);
  images["1ship"]->setPosition(std::get<0>(center) - 55, std::get<1>(center) - 40);
  SDL_SetTextureColorMod(images["1NG"]->getTexture(), 255, 69, 0);

  center = getCenterForImage(images["1HS"]);
  images["1HS"]->setPosition(std::get<0>(center), std::get<1>(center) + 10);

  center = getCenterForImage(images["1CR"]);
  images["1CR"]->setPosition(std::get<0>(center), std::get<1>(center) + 60);

  center = getCenterForImage(images["1QT"]);
  images["1QT"]->setPosition(std::get<0>(center), std::get<1>(center) + 110);


  // Add mouse movement event listeners to reset link colors to white if they
  // are no longer selected.
  eventHandler.addListener(SDL_MOUSEMOTION, [&] (SDL_Event*) {
    if (selectedIndex != 3)
      SDL_SetTextureColorMod(images["1NG"]->getTexture(), 255, 255, 255);
    if (selectedIndex != 4)
      SDL_SetTextureColorMod(images["1HS"]->getTexture(), 255, 255, 255);
    if (selectedIndex != 5)
      SDL_SetTextureColorMod(images["1CR"]->getTexture(), 255, 255, 255);
    if (selectedIndex != 6)
      SDL_SetTextureColorMod(images["1QT"]->getTexture(), 255, 255, 255);
  });

  // If the down arrow key is pressed, this listener will set the link colors
  // correctly and then move the ship icon to the correct text entry
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex < 6)
      selectedIndex += 1;

    std::string s = translate(selectedIndex);

    SDL_SetTextureColorMod(images["1NG"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images["1HS"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images["1CR"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images["1QT"]->getTexture(), 255, 255, 255);
    SDL_SetTextureColorMod(images[s]->getTexture(), 255, 69, 0);

    int x = images[s]->getDestRect()->x - 55;
    int y = images[s]->getDestRect()->y;
    images["1ship"]->setPosition(x, y);
  }, SDLK_DOWN);

  // Up key event listener (similar to last listener)
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    if (selectedIndex > 3)
      selectedIndex -= 1;
      std::string s = translate(selectedIndex);

      SDL_SetTextureColorMod(images["1NG"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images["1HS"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images["1CR"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images["1QT"]->getTexture(), 255, 255, 255);
      SDL_SetTextureColorMod(images[s]->getTexture(), 255, 69, 0);

      int x = images[s]->getDestRect()->x - 55;
      int y = images[s]->getDestRect()->y;
      images["1ship"]->setPosition(x, y);
  }, SDLK_UP);

  // Enter/Space key listeners for currently selected entry
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    transition();
  }, SDLK_RETURN);
  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    transition();
  }, SDLK_SPACE);

  // Key listeners for brightness and volume
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (brightness > 0) {
      brightness--;
      updateBrightnessVolume();
    }
  }, SDLK_1);
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (brightness < 100) {
      brightness++;
      updateBrightnessVolume();
    }
  }, SDLK_2);
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (engine->volume > 0) {
      engine->volume--;
      updateBrightnessVolume();
      audioHandler.setVolume(engine->volume);
    }
  }, SDLK_9);
  eventHandler.addListener(SDL_KEYDOWN, [&] (SDL_Event*) {
    if (engine->volume < 100) {
      engine->volume++;
      updateBrightnessVolume();
      audioHandler.setVolume(engine->volume);
    }
  }, SDLK_0);

  // on hover and click events for the menu entries
  images["1NG"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["1NG"]->getTexture(), 255, 69, 0);
  });
  images["1NG"]->onClick(&eventHandler, [&] () {
    Mix_PauseMusic();
    engine->newGame();
    if (engine->isRunning())
      engine->setState("intro");
  });

  images["1HS"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["1HS"]->getTexture(), 255, 69, 0);
  });
  images["1HS"]->onClick(&eventHandler, [&] () {
    engine->newHighscore();
    engine->setState("Highscore");
  });

  images["1CR"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["1CR"]->getTexture(), 255, 69, 0);
  });
  images["1CR"]->onClick(&eventHandler, [&] () {
    engine->newHighscore();
    engine->setState("credits");
  });

  images["1QT"]->onHover(&eventHandler, [&] () {
    SDL_SetTextureColorMod(images["1QT"]->getTexture(), 255, 69, 0);
  });
  images["1QT"]->onClick(&eventHandler, [&] () {
    engine->quit();
  });
}

void MenuState::update(double seconds){
  State::update(seconds);  
  if(!audioHandler.isPlaying()){audioHandler.play("intro");}
}

MenuState::~MenuState() {}

void MenuState::transition() {
  if (selectedIndex == 3) {
    Mix_PauseMusic(); 
    engine->newGame();
    if (engine->isRunning())
      engine->setState("intro");
  }
  else if (selectedIndex == 4) {
    engine->newHighscore();
    engine->setState("Highscore");
  }
  else if (selectedIndex == 5) {
    engine->setState("credits");
  }
  else if (selectedIndex == 6) {
    engine->quit();
  }
}

// Gets the x and y positions for an image to center it in the window.
std::tuple<int, int> MenuState::getCenterForImage(Image* image) {
  int x = WIDTH / 2 - image->getDestRect()->w / 2;
  int y = HEIGHT / 2 - image->getDestRect()->h / 2;
  return std::tuple<int, int>(x, y);
}

void MenuState::updateBrightnessVolume() {
  int bright;
  delete images["1Bnum"];
  delete images["1Vnum"];
  
  bright = (int)brightness;
  images["1Bnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    350, 500, 40, std::to_string(bright) + "%");
  images["1Bnum"]->load();

  images["1Vnum"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    1000, 500, 40, std::to_string(engine->volume) + "%");
  images["1Vnum"]->load();
}

// Translates selected index
std::string MenuState::translate(int i) {
  switch(i) {
    case 3:
      return "1NG";
    case 4:
      return "1HS";
    case 5:
      return "1CR";
    case 6:
      return "1QT";
  }
  return "THIS WIL WORK!";
}

