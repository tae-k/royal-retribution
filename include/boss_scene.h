#ifndef BOSS_SCENE_H
#define BOSS_SCENE_H

#include "playing_state.h"

class BossScene : public PlayingState {
  public:
    BossScene(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~BossScene();

  private:
    virtual void setup();
    virtual void update(double seconds);

    int a0 = 0;
    int a1 = 0;
    int a2 = 0;
    int a3 = 0;
    int a4 = 255;
    int event = 0;
    int counter = 0;

    bool raise = false;
    bool finished = false;

    std::string skip = "Press [1] to SKIP";
    std::string e1 = "Hmmm... So you've made it this far. (next [n])";
    std::string k1 = "As king, it was only to be expected. (next [n])";
    std::string e2 = "hehehe... (next [n])";
    std::string e3 = "Well then, why don't we say hi to my little friend? (next [n])";
    std::string k2= "! (next [n])";
    std::string e4 = "MWAHAHAHAHAHA!!! NOW DIE, HUMAN!!! (next [n])";
};

#endif
