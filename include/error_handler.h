#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

//This class handles the errors and safely quits the program
class Engine;
class ErrorHandler {
  public:
    void quit(const char *, const char*);
    ErrorHandler(Engine *engine_ref);
    Engine *engine = nullptr;
  private:
};
#endif
