#include "map.h"

Map::Map(SDL_Renderer* renderer_p, ErrorHandler* errorHandler_p,
        std::string map_filename, std::string textures_filename,
        CollisionDetector* collision_detector_p, Camera *cam) {

  collisionDetector = collision_detector_p;

  errorHandler = errorHandler_p;

  renderer = renderer_p;

  camera = cam;

  loadTextures(textures_filename);

  loadLayout(map_filename);
}


//creates textures based on legend symbols
void Map::loadTextures(std::string filename) {
  std::ifstream file(filename);
  
  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::string line;
  int id = 0;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    char sym = -1;
    int start_frame = -1;
    int frame_length = -1;
    int options = 0;

    if (!(iss >> sym) || sym == -1 || !(iss >> start_frame) || start_frame == -1
        || !(iss >> frame_length) || frame_length == -1)
      errorHandler->quit(__func__, "Invalid file contents");

    iss >> options;

    textureIDs[sym] = id;

    createTexture(id, TILES_IMG, start_frame, frame_length, options, -1);

    id++;
  }

  file.close();
}

// Creates tiles using textures created above
void Map::loadLayout(std::string filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::vector<letter> letters;
  std::string line;

  int row = 0;
  int col = 0;
  while(std::getline(file, line)) {
    std::istringstream iss(line);

    char sym = -1;
    while (iss >> sym) {
      if (sym == -1)
        errorHandler->quit(__func__, "Invalid file contents");
      else if (textureIDs.find(sym) == textureIDs.end()) {
        std::cerr << "sym not found: " << sym << "\n";
        errorHandler->quit(__func__, "Texture not found for symbol");
      }


      letter l;
      l.letter = sym;
      l.row = row;
      l.col = col;
      letters.push_back(l);

      col++;
    }

    int rowWidth = col * TILE_DIM;
    if (rowWidth > width) width = rowWidth;

    col = 0;
    row++;
  }

  col = width / TILE_DIM;

  height = row * TILE_DIM;

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (col * i + j < (int) letters.size()) {
        letter* l = &(letters[col * i + j]);

        if (!l->grouped) {
          tile t;
          t.tileTexture = &(textures[textureIDs[l->letter]]);
          t.collidable = ((t.tileTexture->options & 1) == 1);
          t.start_frame = t.tileTexture->start_frame;
          t.frame_length = t.tileTexture->frame_length;

          int textureWidth;
          int textureHeight;
          SDL_QueryTexture(t.tileTexture->texture, NULL, NULL, &textureWidth, &textureHeight);
          int padding_x = SPRITE_PADDING_AMOUNT_X;
          int padding_y = SPRITE_PADDING_AMOUNT_Y;
          int frame_width = TILE_DIM + padding_x;
          int frame_height = TILE_DIM + padding_y;

          int max_adj_w = 16384 / textureWidth;
          int max_adj_h = 16384 / (frame_height * (double) t.frame_length / NUM_COLS);
          int max_adj = max_adj_w < max_adj_h ? max_adj_w : max_adj_h;

          checkDirections(l, letters, col, row);

          if (l->adjRight > max_adj) l->adjRight = max_adj;
          if (l->adjDown > max_adj) l->adjDown = max_adj;

          if (l->adjRight > l->adjDown) {
            createSprite(t, l->col * TILE_DIM, l->row * TILE_DIM, l->adjRight * TILE_DIM, TILE_DIM, frame_width, frame_height);

            for (int a = l->col; a < l->col + l->adjRight; a++) {
              letters[a + l->row * col].grouped = true;
            }
          } else {
            for (int a = l->row; a < l->row + l->adjDown; a++) {
              letters[a * col + l->col].grouped = true;
            }
            createSprite(t, l->col * TILE_DIM, l->row * TILE_DIM, TILE_DIM, l->adjDown * TILE_DIM, frame_width, frame_height);
          }
        }
      }
    }
  }

  file.close();

  addCollidablesToBuckets(&tiles);
}

//max_cols and max_rows is the number of cols and rows in grid
void Map::checkDirections(letter* l, std::vector<letter> letters, int max_cols, int max_rows) {
  for (int i = l->col + 1; i < max_cols; i++) {
    if (l->letter != letters[max_cols * l->row + i].letter)
      break;

    l->adjRight++;
  }

  for (int i = l->row + 1; i < max_rows; i++) {
    if (l->letter != letters[l->col + i * max_cols].letter)
      break;

    l->adjDown++;
  }
}

void Map::createSprite(tile t, double pos_x, double pos_y, double width, double height, int frame_width, int frame_height) {
  int textureWidth;
  SDL_QueryTexture(t.tileTexture->texture, NULL, NULL, &textureWidth, NULL);

  //limit 16384x16384
  int num_frames_x = t.frame_length > NUM_COLS - 1 ? NUM_COLS : t.frame_length;
  int texture_width = width / TILE_DIM * textureWidth / NUM_COLS * (num_frames_x);
  int texture_height = ((t.frame_length - 1) / NUM_COLS + 1) * frame_height * height / TILE_DIM;


  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332,
    SDL_TEXTUREACCESS_TARGET, texture_width, texture_height);

  if (texture == nullptr) {
    errorHandler->quit(__func__, SDL_GetError());
  }

  if (SDL_SetRenderTarget(renderer, texture) < 0) {
    SDL_DestroyTexture(texture);
    errorHandler->quit(__func__, SDL_GetError());
  }

  SDL_RenderClear(renderer);
  
  for (int j = t.start_frame; j < t.start_frame + t.frame_length; j++) {
    SDL_Rect srcRect;
    srcRect = {j % NUM_COLS * frame_width, j / NUM_COLS * frame_height, TILE_DIM, TILE_DIM};

    // for each tile
    for (int i = 0; i < (width > height ? width : height) / TILE_DIM; i++){
      SDL_Rect dest;
      if (width > height)
        dest = {(int) ((width + SPRITE_PADDING_AMOUNT_X) * (j - t.start_frame) + TILE_DIM * i), 0, TILE_DIM, TILE_DIM};
      else
        dest = {0, (int) ((height + SPRITE_PADDING_AMOUNT_Y) * (j - t.start_frame) + TILE_DIM * i), TILE_DIM, TILE_DIM};

      if (SDL_RenderCopy(renderer, t.tileTexture->texture, &srcRect, &dest) < 0) {
        SDL_DestroyTexture(texture);
        errorHandler->quit(__func__, SDL_GetError());
      }
    }
  }

  t.start_frame = 0;
  
  SDL_SetRenderTarget(renderer, NULL);

  t.image = new Sprite(renderer, "", errorHandler, width, height,
    pos_x, pos_y, t.collidable, false);

  if (t.collidable) {
    tiles.insert(tiles.begin(), t);
    tiles[0].image->load(texture);
  } else {
    tiles.push_back(t);
    tiles.back().image->load(texture);
  }
}

void Map::loadSecondLayout(std::string filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::string line;
  int row = 0;
  while (std::getline(file, line)) {
    int col = 0;

    std::istringstream iss(line);
    char sym = -1;

    while(iss >> sym) {
      if (sym == -1)
        errorHandler->quit(__func__, "Invalid file contents");
      else if (textureExtraIDs.find(sym) == textureExtraIDs.end()) {
        std::cerr << "sym not found: " << sym << "\n";
        errorHandler->quit(__func__, "Texture not found for symbol");
      }else if (sym == '-'){
        col++;
        continue;
      }

      struct texture* texture = &(textures[textureExtraIDs[sym]]);

      bool collidable = ((texture->options & 1) == 1);
      bool trap = ((texture->options & 2) == 2);
      bool door = ((texture->options & 4) == 4);
      bool torch = ((texture->options & 8) == 8);
      bool chest = ((texture->options & 16) == 16);
      bool hole = ((texture->options & 32) == 32);
      bool blade = ((texture->options & 64) == 64);
      
      if (torch) {
        lights.push_back(new Sprite(renderer, LIGHTS_FILENAME, errorHandler,
            140, 140, TILE_DIM * (col-1), TILE_DIM * (row-1), false, false));
      }


      tile t;
      t.start_frame = texture->start_frame;
      t.frame_length = texture->frame_length;
      t.image = new Sprite(renderer, "", errorHandler, TILE_DIM, TILE_DIM,
        col * TILE_DIM, row * TILE_DIM, collidable, false);
      t.image->pairing = texture->pairing;
      t.image->setTrap(trap);
      t.image->setDoor(door);
      t.image->setChest(chest);
      t.image->setBlade(blade);
      t.image->setHole(hole);
      for(unsigned i = 0; i < additions.size(); i++){
        if(t.image->pairing != -1 && additions[i].image->pairing == t.image->pairing){
          t.image->setPair(additions[i].image);
          additions[i].image->setPair(t.image);
        }
      }

      if (collidable) {
        additions.insert(additions.begin(), t);
        additions[0].image->load(texture->texture);
      } else {
        additions.push_back(t);
        additions.back().image->load(texture->texture);
      }

      col++;
    }

    int rowWidth = col * TILE_DIM;
    if (rowWidth > width) width = rowWidth;

    col = 0;
    row++;
  }
  height = row * TILE_DIM;

  file.close();
  addCollidablesToBuckets(&additions);
}

void Map::loadSecondTextures(std::string filename) {
  std::ifstream file(filename);
  
  if (!file.is_open()) {
    std::string error = filename + " could not be opened";
    errorHandler->quit(__func__, error.c_str());
  }

  std::string line;
  int id = textures.size();
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    char sym = -1;
    int start_frame = -1;
    int frame_length = -1;
    int options = 0;
    char pairing = -1;

    if (!(iss >> sym) || sym == -1 || !(iss >> start_frame) || start_frame == -1
        || !(iss >> frame_length) || frame_length == -1)
      errorHandler->quit(__func__, "Invalid file contents");

    iss >> options;
    if(!(iss >> pairing) || pairing == -1){
      pairing = -1;
    }

    textureExtraIDs[sym] = id;

    createTexture(id, TILES_IMG, start_frame, frame_length, options, pairing);

    id++;
  }
}
void Map::createTexture(int id, std::string filename, int start_frame,
    int frame_length, int options, char pairing) {

  if ((int) textures.size() != id)
    errorHandler->quit(__func__, "Invalid Texture ID. Vector wrong size");

  SDL_Surface* surf = IMG_Load((filename).c_str());
  if (surf == nullptr) errorHandler->quit(__func__, IMG_GetError());

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (texture == nullptr) {
    SDL_FreeSurface(surf);
    errorHandler->quit(__func__, SDL_GetError());
  }
  SDL_FreeSurface(surf);

  struct texture t;
  t.texture = texture;
  t.start_frame = start_frame;
  t.frame_length = frame_length;
  t.options = options;
  t.pairing = pairing;

  textures.push_back(t);
}

void Map::update(double seconds) {
  for (auto tile : tiles) {
    tile.image->update(seconds);
    tile.image->animate(seconds, tile.start_frame,
      tile.frame_length + tile.start_frame - 1);
  }
  for (auto tile : additions) {
    tile.image->update(seconds);
    collisionDetector->updateBuckets(tile.image, this);
    if(tile.image->isChest()){
      if(tile.image->pair == nullptr){
        tile.image->animate(seconds, tile.start_frame, tile.start_frame);
        continue;
      }
      if(tile.image->pair == tile.image->pair){
        tile.image->animate(seconds, tile.frame_length + tile.start_frame - 1, 
          tile.frame_length + tile.start_frame - 1);
        continue;
      }
    }
    if(tile.image->isBlade()){
      SDL_Rect cameraRect = camera->getRect();
      SDL_Rect imageRect = {(int) tile.image->pos_x, (int) tile.image->pos_y,
      tile.image->getDestRect()->w, tile.image->getDestRect()->h};

      if (!SDL_HasIntersection(&imageRect, &cameraRect)){
        continue;
      }
      tile.image->animate(seconds, tile.start_frame, tile.frame_length + tile.start_frame - 1, 9);
      tile.image->stopped = paused;
      continue;
    }
    tile.image->animate(seconds, tile.start_frame, tile.frame_length + tile.start_frame - 1, 5.0);
  }
}

void Map::render(Camera* camera_, double interpol_alpha) {
  for (auto tile : tiles) {
    tile.image->render(camera_, interpol_alpha);
  }
  for (auto tile : additions) {
    tile.image->render(camera_, interpol_alpha);
  }
}

int Map::pushLights(std::map<std::string, Image*>& images) {
  int counter = 0;
  for (auto light : lights) {
    std::string s = std::to_string(ADD)+"light_"+std::to_string(counter);
    images[s] = light;
    counter++;
  }
  lights.clear();
  return counter;
}

void Map::cleanup() {
  for (auto light : lights) {
    if (light != nullptr) {
      delete light;
      light = nullptr;
    }
  }

  for (auto tile : tiles) {
    if (tile.image != nullptr) {
      delete tile.image;
      tile.image = nullptr;
    }
  }
  for (auto tile : additions) {
    if (tile.image != nullptr) {
      delete tile.image;
      tile.image = nullptr;
    }
  }
  for (auto texture : textures) {
    if (texture.texture != nullptr) {
      SDL_DestroyTexture(texture.texture);
      texture.texture = nullptr;
    }
  }
}

void Map::addCollidablesToBuckets(std::vector<tile>* tiles) {
  for (auto tile = tiles->begin(); tile != tiles->end(); tile++) {
    if (!tile->image->isCollidable()) break;

    collisionDetector->updateBuckets(tile->image, this);
  }
}


Map::~Map() {
  cleanup();
}
