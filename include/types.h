#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <map>
#include <SDL.h>

class Image;

typedef std::vector<Image*> collision_bucket;
typedef std::function<void(SDL_Event* event)> event_lambda;
typedef std::vector<event_lambda> event_lambda_vector;
typedef std::map<std::array<int, 2>, event_lambda_vector*> event_bind;

typedef struct {
  double x;
  double y;
  double w;
  double h;
} doubleRect;

#endif
