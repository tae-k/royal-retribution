#ifndef WIN_STATE_H
#define WIN_STATE_H

#include "state.h"

class WinState : public State {
  public:
    WinState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~WinState();

  private:
    virtual void setup();
    virtual void load();
    virtual void update(double seconds);

    int a0 = 0;
    double totalTime = 0;
    std::string logo = "Y O U   W I N";
};

#endif
