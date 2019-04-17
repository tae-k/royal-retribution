#ifndef LEVEL_3_STATE_H
#define LEVEL_3_STATE_H

#include "playing_state.h"


class Level_3_State : public PlayingState {
  public:
    Level_3_State(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~Level_3_State();
  private:
    void update(double);
    virtual void setup();
};

#endif
