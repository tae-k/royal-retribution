#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include <SDL_image.h>

#include "constants.h"
#include "error_handler.h"

class Image;
class Character;
class Camera {
  public:
    Camera(ErrorHandler*);
    void setCharacter(Character*);
    void updatePosition();

    void setPosition(Image *img);
    bool pan(Image *to, double seconds);

    int render(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*,
      SDL_Rect*, double, bool fixed = false);
    int render(SDL_Renderer*, SDL_Texture*, SDL_Rect*, SDL_Rect*,
      bool fixed = false);
    Character* character;

    SDL_Rect getRect();
  private:
    int counter  = 1;

    bool left = false;
    bool start = true;

    double pos_x = 0.0;
    double pos_y = 0.0;
    double dest_x = 0.0;
    double dest_y = 0.0;

    ErrorHandler* errorHandler;

    SDL_Rect prevRect;

    void interpolate(SDL_Rect*, SDL_Rect*, double);
};

#endif
