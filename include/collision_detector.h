#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include <iostream>
#include <vector>
#include "image.h"
#include "map.h"
#include "types.h"

#define BUCKET_ROWS 20
#define BUCKET_COLS 20

/*The header file for the collision class that holds a vector of all images
on screen and checks when they are intersecting in position.*/
class Map;
class CollisionDetector {
  public:
    CollisionDetector();

    void check(Camera*, Map* map = nullptr);

    void updateBuckets(Image* image, Map* map = nullptr);
  private:
    bool getIntersection(Image*, Image*, doubleRect*);
    void checkInBuckets(Camera*, Map* map = nullptr);
    void checkCollision(Image*, Image*);
    std::vector<collision_bucket> buckets;
    void initializeBuckets();
};
#endif
