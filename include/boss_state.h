#ifndef BOSS_STATE_H
#define BOSS_STATE_H

#include "playing_state.h"

class BossState : public PlayingState {
  public:
    BossState(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~BossState();
  private:
    void update(double);
    virtual void setup();
    int thePhase = 0;
    int fade = 255;
};

#endif
