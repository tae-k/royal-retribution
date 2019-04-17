#ifndef PLAYING_STATE_H
#define PLAYING_STATE_H

#include "state.h"
#include "enemy.h"
#include "mini_enemy.h"
#include "fire_enemy.h"
#include "projectile.h"

//#define NUM_ENEMIES     10
//#define ENEMY_FILENAME  "level_1_enemies.txt"

class PlayingState : public State {
  public:
    PlayingState(Engine* engine, ErrorHandler* errorHandler);

    virtual ~PlayingState();
    virtual void load();
    virtual void activateInstructionText(int);
    void deactivateInstructionText();
    void updateHearts();
    void updateHeartsPlus();
    void updateLights(double);

    static int instrGiven;
    static int fallen;
    double timer = 1;

    virtual void enter();
  protected:
    bool isScene = false;

    int currentLevel;
    int currentScore;

    void setupFood();
    void setupInstruct();
    void loadStaAndExp();

    void update(double);
    virtual void setup();

    int num_food = 0;
    int num_lights = 0;
    int num_enemies = 0;

    bool eat0 = false;
    bool eat1 = false;
    bool eat2 = false;

    bool skipPan = false;
    bool foodInstructionGiven = false;

    int a0 = 0;
    int a1 = 255;
    int musicSwitch = 1;

    double timeSinceStart = 0;

    virtual void cleanup();
};

#endif
