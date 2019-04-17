/*
 * text.cpp
 */

#include "text.h"

Text::Text(SDL_Renderer *renderer, std::string font_filename,
    ErrorHandler *error_handler, int pos_x, int pos_y, int font_size_p,
    std::string text_p, const SDL_Color &color_p)
    : Sprite(renderer, font_filename, error_handler, pos_x, pos_y, false),
    color(color_p) {
  
  font_size = font_size_p;
  text = text_p;
}

void Text::load(SDL_Texture *texture_p) {
  if (texture_p != nullptr) {
    texture = texture_p;
  } else {
    font = TTF_OpenFont(image_file.c_str(), font_size);
    if (font == nullptr) {
      error_handler->quit(__func__, TTF_GetError());
    }

    surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (surface == nullptr) {
      error_handler->quit(__func__, TTF_GetError());
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
      error_handler->quit(__func__, SDL_GetError());
    }

    SDL_FreeSurface(surface);
  }

  if (rect.w == 0 && rect.h == 0) {
    get_texture_size(texture, &(rect.w), &(rect.h));
  }
}

SDL_Rect* Text::getDestRect() {
  return &rect;
}

void Text::render(Camera*, double) {
  if (SDL_RenderCopy(renderer, texture, NULL, &rect)) {
    error_handler->quit(__func__, SDL_GetError());
  }
}

void Text::cleanup() {
  Image::cleanup();

  if (font != nullptr) {
    TTF_CloseFont(font);
  }
}

Text::~Text() {
  cleanup();
}
