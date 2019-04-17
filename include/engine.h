#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <map>
#include <vector>
#include <iostream>

#include "image.h"
#include "audio.h"
#include "state.h"
#include "enemy.h"
#include "sword.h"
#include "sprite.h"
#include "squire.h"
#include "constants.h"
#include "character.h"
#include "mini_enemy.h"
#include "event_handler.h"
#include "error_handler.h"
#include "collision_detector.h"

/*Header file for the game engine that is in charge of setup loading, 
and rendering textures along with any audio and text. Plus it destroys 
objects and frees memory when done*/
class State;
class Engine {

  public:
    static Engine* instance;

    void start();
    void cleanup();
    void newGame();
    void newHighscore();

    Engine();

    ErrorHandler error_handler;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    void setState(std::string);
    void setNextLevel(std::string, Image*&);

    void quit();
    void incrementScore(int increment);
    int score = 0;
    std::map<std::string, State*> states;
    static int volume;

    bool isRunning() { return running; } 

  private:
    void setup();
    void loop();
    void update(double seconds);
    void render();
    void createStates();
    bool running = true;
    double accumulator = 0;
    EventHandler eventHandler;
    State* currentState = nullptr;
    Mix_Music *intro = nullptr;
    unsigned int lastTime = 0;
};

#endif
