#ifndef LEVEL_2_STATE_H
#define LEVEL_2_STATE_H

#include "playing_state.h"

class Level_2_State : public PlayingState {
  public:
    Level_2_State(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~Level_2_State();
  private:
    void update(double);
    virtual void setup();
};

#endif
