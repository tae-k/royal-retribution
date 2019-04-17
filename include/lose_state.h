#ifndef LOSE_STATE_H
#define LOSE_STATE_H

#include "state.h"

class LoseState : public State {
  public:
    LoseState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~LoseState();

  private:
    virtual void setup();
    virtual void load();
    virtual void update(double seconds);

    int a0 = 0;
    double totalTime = 0;
    std::string logo = "G A M E   O V E R";
};

#endif
