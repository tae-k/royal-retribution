#ifndef AUDIO_H 
#define AUDIO_H

#include <iostream>
#include <map>
#include <SDL.h>
#include <SDL_mixer.h>
#include "constants.h"
#include "error_handler.h"

/*Header file for the class that handles audio, Responsible for loading 
and holding music as well as playing it*/
class Audio {
  public:
    Audio(ErrorHandler*);

    void load();
    void cleanup();
    void play(std::string, int = -1);
    void setVolume(int vol);
    bool isPlaying();
    Mix_Chunk* getChunk(std::string sfx){ return sound_effects[sfx];}
  private:
    ErrorHandler* error_handler;
    std::map<std::string, Mix_Music*> music;
    std::map<std::string, Mix_Chunk*> sound_effects;
};

#endif
