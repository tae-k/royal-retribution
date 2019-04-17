#ifndef LEVEL_4_STATE_H
#define LEVEL_4_STATE_H

#include "playing_state.h"


class Level_4_State : public PlayingState {
  public:
    Level_4_State(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~Level_4_State();
  private:
    void update(double);
    virtual void setup();
};

#endif
