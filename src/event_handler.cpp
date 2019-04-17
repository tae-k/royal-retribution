/*
 * event_handler.h
 */

#include "event_handler.h"

std::vector<SDL_Event> EventHandler::events;

void EventHandler::check() {
  for (SDL_Event event : EventHandler::events) {
    int sym = event.key.keysym.sym;

    for (auto map : listeners) {
      if ((int) event.type == map.first[0]
        && (map.first[1] == 0 || map.first[1] == sym)) {
        runTriggers((map.second), &event);
      }
    }
  }
}

// Add a listener for an event.
// `event` is the event to listen for
// `trigger` is what to run when event occurs
// `key_sym` is the key symbol to check for (defaults to 0 for none)
void EventHandler::addListener(int event, event_lambda trigger, int key_sym) {
  std::array<int, 2> actions = {{event, key_sym}};

  if (listeners.find(actions) == listeners.end()) {
    listeners[actions] = new event_lambda_vector;
  }
  listeners[actions]->push_back(trigger);
}

EventHandler::~EventHandler() {
  for (auto map : listeners) {
    delete map.second;
  }
}

void EventHandler::getEvents() {
  EventHandler::events.clear();

  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    EventHandler::events.push_back(event);
  }
}

void EventHandler::runTriggers(event_lambda_vector* triggers, SDL_Event* event) {
  for (auto trigger : *triggers) {
    trigger(event);
  }
}
