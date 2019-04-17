/*
 * image.cpp
 */

#include "image.h"
//Definitions for the the parent class that any image object inherit from

void Image::load(SDL_Texture* texture_p) {
  if (texture_p != nullptr) {
    texture = texture_p;
  } else {
    //Makes a surface where the texture is created from
    surf = IMG_Load(image_file.c_str());
    if (surf == nullptr) {
      error_handler->quit(__func__, IMG_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr) {
      SDL_FreeSurface(surf);
      error_handler->quit(__func__, SDL_GetError());
    }

    SDL_FreeSurface(surf);
  }
}

void Image::update(double) {
  
}

void Image::render(Camera*, double) {
  if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

//Destroys the texture to free up memory
void Image::cleanup() {
  if (triggers != nullptr) {
    delete triggers;
    triggers = nullptr;
  }

  if (texture != nullptr) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}

SDL_Rect* Image::getDestRect() {
  return NULL;
}

SDL_Texture* Image::getTexture() {
  return texture;
}

bool Image::isCollidable() {
  return false;
}

void Image::setCollidable(bool) {
}


Image::Image(SDL_Renderer *renderer_p, std::string image_file,
  ErrorHandler *error_handler_p) :
    image_file(image_file) {

    buckets = std::vector<unsigned int>();
    
    renderer = renderer_p;
    error_handler = error_handler_p;

    triggers = new std::map<int, std::function<void()>>;
}

Image::~Image() {
  cleanup();
}

void Image::setPosition(double, double) {
  error_handler->quit(__func__,
  "Image position is always (0, 0). Did you mean to call this on a subclass?");
}

void Image::onHover(EventHandler *eventHandler, std::function<void()> trigger) {
  (*triggers)[currentTrigger_i] = trigger;

  int trigger_i = currentTrigger_i;

  eventHandler->addListener(SDL_MOUSEMOTION, [trigger_i, this] (SDL_Event* e) {
    int x = e->motion.x;
    int y = e->motion.y;
    SDL_Rect* rect = getDestRect();
    if (rect == NULL) *rect = {0, 0, WIDTH, HEIGHT};

    if (x > rect->x && x < rect->x + rect->w && y > rect->y
      && y < rect->y + rect->h) {
      
      triggers->at(trigger_i)();
    };
  });

  currentTrigger_i++;
}

void Image::onClick(EventHandler *eventHandler, std::function<void()> trigger) {
  (*triggers)[currentTrigger_i] = trigger;

  int trigger_i = currentTrigger_i;

  eventHandler->addListener(SDL_MOUSEBUTTONUP, [trigger_i, this] (SDL_Event* e) {
    if (e->button.button == SDL_BUTTON_LEFT) {
      int x = e->button.x;
      int y = e->button.y;
      SDL_Rect* rect = getDestRect();
      if (rect == NULL) *rect = {0, 0, WIDTH, HEIGHT};

      if (x > rect->x && x < rect->x + rect->w && y > rect->y
        && y < rect->y + rect->h) {
        
        triggers->at(trigger_i)();
      }
    }
  });

  currentTrigger_i++;
}
