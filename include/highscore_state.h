#ifndef HIGHSCORE_STATE_H
#define HIGHSCORE_STATE_H

#define MAX_TOTAL     10

#include "state.h"
#include <algorithm>
#include <functional>

class HighscoreState : public State {
  public:
    HighscoreState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~HighscoreState();
    void update(double seconds);
    void load();
    void setup();

    static void saveScore(int);
  private:
    int total = 0;
    std::string line;
    std::ifstream file;
    std::vector<int> scores;

    int alphas[MAX_TOTAL + 1] = {};
};

#endif
