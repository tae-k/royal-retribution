#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "sprite.h"
#include "error_handler.h"
#include "collision_detector.h"
#include "camera.h"

#define TILE_DIM      32

struct letter {
  char letter;
  int row;
  int col;
  int adjRight = 1;
  int adjDown = 1;
  bool grouped = false;
};

struct texture {
  SDL_Texture* texture;
  int start_frame = 0;
  int frame_length = 0;
  int options = 0;
  int pairing = -1;
};

struct tile {
  int start_frame = 0;
  int frame_length = 0;
  bool collidable = false;
  texture* tileTexture = nullptr;

  Sprite* image = nullptr;
};

class CollisionDetector;
class Map {
  public:
    Map(SDL_Renderer* renderer, ErrorHandler*, std::string, std::string,
      CollisionDetector*, Camera *);
    ~Map();

    std::vector<tile> tiles;
    std::vector<tile> additions;
    std::vector<tile> collidable_tiles;

    std::map<char, int> textureIDs;

    std::map<char, int> textureExtraIDs;

    std::vector<texture> textures;

    void loadTextures(std::string);
    void loadSecondTextures(std::string);
    void loadSecondLayout(std::string);
    void loadLayout(std::string);
    int pushLights(std::map<std::string,Image*>&);

    void cleanup();

    void update(double seconds);
    void render(Camera* camera, double interpol_alpha = 1);

    int width = 0;
    int height = 0;

    std::vector<Image*> lights;
 
    bool paused = false;

  private:
    void checkDirections(letter*, std::vector<letter> letters,
      int, int);

    void createTexture(int, std::string, int, int, int, char);

    void createSprite(tile, double, double, double, double, int, int);

    ErrorHandler* errorHandler;

    SDL_Renderer* renderer = nullptr;

    CollisionDetector* collisionDetector;

    void addCollidablesToBuckets(std::vector<tile>*);

    Camera *camera;
};

#endif
