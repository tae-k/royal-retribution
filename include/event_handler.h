#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <iostream>
#include <map>
#include <vector>
#include <array>
#include <functional>
#include <SDL.h>
#include "types.h"

//This class handles the events that are needed in the game
class EventHandler {
  public:
    void getEvents();

    void check();
    void addListener(int event, event_lambda, int key_sym = 0);
    ~EventHandler();
  private:
    static std::vector<SDL_Event> events;
    
    event_bind listeners;

    void runTriggers(event_lambda_vector* triggers, SDL_Event* event);
};

#endif
