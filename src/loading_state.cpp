#include "loading_state.h"

LoadingState::LoadingState(Engine* engine, ErrorHandler* errorHandler)
  : State(engine, errorHandler) {

  num_states = NUM_STATES - 1;

  ratios.reserve(NUM_STATES);

  setup();
  load();
}

void LoadingState::setup(){
  images["1start"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 100, "Loading...", ROYAL_GOLD);
  images["1progress"] = new Sprite(engine->renderer, EXP_BAR, errorHandler,
    0, 0, false, true);
  images["1progress_outline"] = new Sprite(engine->renderer, BAR_BOX, errorHandler,
    0, 0, false, true);
}

void LoadingState::load() {
  State::load();

  int orig_w = images["1progress_outline"]->getDestRect()->w;
  int w = images["1start"]->getDestRect()->w;

  images["1progress_outline"]->getDestRect()->w = w;
  images["1progress"]->getDestRect()->w = 0;

  auto center = getCenterForImage(images["1start"]);
  images["1start"]->setPosition(std::get<0>(center),
                                std::get<1>(center)-50);

  center = getCenterForImage(images["1progress_outline"]);
  images["1progress_outline"]->setPosition(std::get<0>(center),
                                           std::get<1>(center)+50);

  int offset_x = 5 * (double) w / (double) orig_w;
  images["1progress"]->setPosition(std::get<0>(center) + offset_x,
                                   std::get<1>(center) + 50);

  eventHandler.addListener(SDL_KEYUP, [&] (SDL_Event*) {
    engine->quit();}, SDLK_ESCAPE); 

  std::ifstream f(LOADTIME_FILENAME);
  if (f.good()) {
    is_saved = true;
    
    double ratio = 0;
    while (f >> ratio) {
      ratios.push_back(ratio);
    }

    if (ratios.size() != NUM_STATES) {
      is_saved = false;
      of.open(LOADTIME_FILENAME);
      if (!of.is_open())
        errorHandler->quit(__func__, "Could not open output file for loading!");
    }

    f.close();
  } else {
    of.open(LOADTIME_FILENAME);
    if (!of.is_open())
      errorHandler->quit(__func__, "Could not open output file for loading!");
  }
}

void LoadingState::update(double) {
  eventHandler.getEvents();
  eventHandler.check();
}

void LoadingState::saveRatios() {
  for (auto ratio : ratios)
    of << ratio / (double) totalTime << " ";
}

void LoadingState::start() {
  segmentTime = SDL_GetTicks();
}

void LoadingState::end() {
  if (!is_saved) {
    unsigned int currentTime = SDL_GetTicks();
    ratios.push_back(currentTime - segmentTime);
    totalTime += currentTime - segmentTime;
    saveRatios();
    of.close();
  }
}

void LoadingState::advance() {
  if (num_states < 0)
    errorHandler->quit(__func__, "Incorrect NUM_STATES field in constants.h");

  if (is_saved) width_ratio += ratios[NUM_STATES - num_states - 1];
  //if (is_saved) width_ratio = ratios[num_states];
  else width_ratio = (double) (NUM_STATES - num_states) / (double) NUM_STATES;

  double curr_w = images["1progress"]->getDestRect()->w;
  double total_w = 0.95*images["1progress_outline"]->getDestRect()->w * width_ratio;

  double vel_to_target = 100.0/1000.0;

  double accumulator = 0;
  int lastTime = SDL_GetTicks();
  unsigned int currentTime = SDL_GetTicks();
  
  if (!is_saved && num_states != 0) {
    ratios.push_back(currentTime - segmentTime);

    totalTime += currentTime - segmentTime;

    segmentTime = currentTime;
  }

  while (curr_w < total_w) {
    currentTime = SDL_GetTicks();
    accumulator += (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;
    while (accumulator >= DELTA_TIME && curr_w < total_w) {
      curr_w += (double) vel_to_target * DELTA_TIME * 1000.0;
      update(0);
      if (!engine->isRunning())
        return;

      accumulator -= DELTA_TIME;

      images["1progress"]->getDestRect()->w = curr_w;
    }

    render();

    if (accumulator < DELTA_TIME)
      SDL_Delay((DELTA_TIME - accumulator) * 1000);
  }

  num_states--;
}

LoadingState::~LoadingState(){}
