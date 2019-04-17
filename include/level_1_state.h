#ifndef LEVEL_1_STATE_H
#define LEVEL_1_STATE_H

#include "playing_state.h"

class Level_1_State : public PlayingState {
  public:
    Level_1_State(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~Level_1_State();
  private:
    void update(double);
    virtual void setup();
};

#endif
