#ifndef INTRO_STATE_H
#define INTRO_STATE_H

#include "playing_state.h"

class IntroState : public PlayingState {
  public:
    IntroState(Engine* engine, ErrorHandler* errorHandler);

    virtual void load();
    virtual ~IntroState();
  private:
    virtual void setup();
    virtual void update(double seconds);

    int a = 0;
    int event = 0;
    int counter = 0;

    bool end = false;
    bool raise = false;
    std::string skip = "Press [1] to SKIP";
    std::string s1 = "MY KING! I bear bad news! (next [n])";
    std::string k1 = "Please tell, my trusted aide. (next [n])";
    std::string s2 = "ALIENS! They have invaded! (next [n])";
    std::string k2 = "I see... How many of my men remain? (next [n])";
    std::string s3 = "None, my Majesty... (next [n])";
    std::string k3 = "Worry Not! Stand Tall! (next [n])";
    std::string k4 = "Royal retribution shall be dealt! (next [n])";
    std::string s4 = "Our lives are in your hands, my king... (next [n])";
};

#endif
