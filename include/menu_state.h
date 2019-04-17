#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "state.h"
#include "pallete.h"

class MenuState : public State {
    public:
      MenuState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~MenuState();

      virtual void load();

    private:
      virtual void setup();

      void update(double);

//      unsigned int brightness = 100;

      std::tuple<int, int> getCenterForImage(Image*);

      int selectedIndex = 3;

      void updateBrightnessVolume();

      void transition();

 
      std::string translate(int i);
};

#endif
