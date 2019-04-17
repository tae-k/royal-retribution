/*
 * error_handler.cpp
 */

#include "engine.h"
#include "error_handler.h"
//Definitions of the class that handles the errors

//Frees the memory and destroys the objects that were created, then quits
void ErrorHandler::quit(const char *function_name, const char *error_message) {
  printf("%s: %s\n", function_name, error_message);
  engine->cleanup();
  exit(0);
}

//Constructer that takes in the engine that runs the game.
ErrorHandler::ErrorHandler(Engine *engine_ref) {
  engine = engine_ref;
}
