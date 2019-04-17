#ifndef STATE_H
#define STATE_H

#include <stdlib.h>

#include <map>
#include <vector>
#include <fstream>
#include <iostream>

#include "map.h"
#include "text.h"
#include "audio.h"
#include "engine.h"
#include "event_handler.h"
#include "collision_detector.h"
#include "camera.h"

enum instruction {trapNum = 2, chestNum = 3, holeNum = 4, doorKeyNum = 5, foodTextNum = 6, healthTextNum = 7};

class State {
  public:
    State(Engine* engine, ErrorHandler* errorHandler);
    //State(Engine* engine, ErrorHandler* errorHandler, Image* sentKing);

    virtual void load();

    virtual void setup() = 0;

    virtual void run(double* seconds);

    virtual ~State();

    virtual void enter() { }
    virtual void exit() { }

    Engine* engine;

    bool isPaused() { return paused; }

    CollisionDetector collisionDetector;

    Map *map = nullptr;

    virtual void activateInstructionText(int);
    virtual void deactivateInstructionText(){}
    virtual void render(double interpol_alpha = 1);
  protected:
    virtual void cleanup();
    virtual void update(double seconds);

    double renderSeconds = 0;

    Image* king = nullptr;

    double newA = 0;
    double speed = 50;

    std::tuple<int, int> getCenterForImage(Image*);
    int fadeIn(std::string s, int a, double seconds, double mult);
    int fadeOut(std::string s, int a, double seconds, double mult);

    std::map<std::string,Image*> images;
    std::map<std::string,Image*>::iterator it;

    Audio audioHandler;
    EventHandler eventHandler;
    ErrorHandler* errorHandler;

    Camera camera;

    void resume(); 
    void pauseUpdate(double);
    void pause() { paused = true; }

    std::string ele = std::to_string(ELE);
    std::string ppl = std::to_string(PPL);
    std::string add = std::to_string(ADD);
    std::string top = std::to_string(TOP);

    static float brightness;

  private:
    unsigned int lastRender = 0;

    bool paused = false;

};

#endif
