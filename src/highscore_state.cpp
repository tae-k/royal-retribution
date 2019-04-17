/*
 * highscore_state.cpp
 */

#include "highscore_state.h"

HighscoreState::HighscoreState(Engine* eng, ErrorHandler* eHandler)
  :  State(eng, eHandler) {
    setup();
    load();
}

void HighscoreState::setup() {
  // title
  images["title"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, 0, 40, "HighScore");
  // read file
  file.open(SCORE_FILENAME);
  if (file.is_open() && std::getline(file, line)) {
    do {
      scores.push_back(stoi(line));

      if (total < MAX_TOTAL)
        total++;
    } while (std::getline(file, line));
    file.close();

    // order scores
    std::sort(scores.begin(), scores.end(), std::greater<int>());
    // create texts
    for (int i = 0; i < MAX_TOTAL; i++) {
      if (i >= total) break;

      std::string prev = "title";
      std::string s = "score_" + std::to_string(i);
      if (i > 0) prev = "score_" + std::to_string(i-1);
      images[s] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
        0, images[prev]->getDestRect()->y+60, 30, std::to_string(scores[i]));
    }
  } else {
    images["no_file"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
      0, HEIGHT / 2 - 15, 30, "You've never won the game!");
  }

  images["skip"] = new Text(engine->renderer, FONT_FILENAME, errorHandler,
    0, HEIGHT-35, 30, "Press [q] to return to menu");
}

void HighscoreState::load() {
  State::load();
  // center(x) of text and fade out
  for (it = images.begin(); it != images.end(); it++) {
    if (it->first != "skip")
      SDL_SetTextureAlphaMod(it->second->getTexture(), 0);
    int x = WIDTH / 2 - it->second->getDestRect()->w / 2;
    int y = it->second->getDestRect()->y;
    it->second->setPosition(x, y);
  }
}

void HighscoreState::update(double seconds) {
  State::update(seconds);

  // green
  SDL_SetTextureColorMod(images["title"]->getTexture(), 0, 180, 60);
  // blue
  alphas[0] = fadeIn("title", alphas[0], seconds, 3);
  for (int i = 0; i < total; i++) {
    std::string s = "score_" + std::to_string(i);
    SDL_SetTextureColorMod(images[s]->getTexture(), 0, 80, 190);

    // fade in
    alphas[i + 1] = fadeIn(s, alphas[i + 1], seconds, 3);
  }

  if (total == 0)
    alphas[1] = fadeIn("no_file", alphas[1], seconds, 3);
} 

HighscoreState::~HighscoreState(){}

void HighscoreState::saveScore(int score) {
  std::ofstream file;
  file.open(SCORE_FILENAME, std::ios_base::app);
  file << std::to_string(score) << std::endl;
  file.close();
}
