/*
 * collision_detector.h
 */

#include <algorithm>
#include "collision_detector.h"
#include "types.h"
#include "character.h"
#include "util.h"

//Definitions of the collision detector class

CollisionDetector::CollisionDetector() {
  initializeBuckets();
}

void CollisionDetector::check(Camera* camera, Map* map) {
  checkInBuckets(camera, map);
}

/* For each bucket on screen, checks for collisions between all items */
void CollisionDetector::checkInBuckets(Camera* camera, Map* map) {
  for (auto bucket_it = buckets.begin(); bucket_it != buckets.end();
    bucket_it++) {

    if (map != nullptr) {
      int bucket_index = bucket_it - buckets.begin();
      const int GRID_WIDTH = map->width / BUCKET_COLS;
      const int GRID_HEIGHT = map->height / BUCKET_ROWS;
      SDL_Rect bucket_rect = {bucket_index % BUCKET_COLS * GRID_WIDTH,
        bucket_index / BUCKET_COLS * GRID_HEIGHT, GRID_WIDTH, GRID_HEIGHT};

      SDL_Rect camera_rect = camera->getRect();
      camera_rect.x -= 30;
      camera_rect.y -= 30;
      camera_rect.w += 60;
      camera_rect.h += 60;

      if (!SDL_HasIntersection(&bucket_rect, &camera_rect)) continue;
    }
    for (auto img1_it = bucket_it->begin(); img1_it != bucket_it->end();
      img1_it++) {
      for (auto img2_it = bucket_it->begin(); img2_it != bucket_it->end();
        img2_it++) {
        if (img1_it == img2_it) continue;

        checkCollision(*img1_it, *img2_it);
      }
    }
  }
}

/* when both object are collidable see if their rects touch if so
relay to that image that the collision happened */
void CollisionDetector::checkCollision(Image* img1, Image* img2) {
  if (img1->isCollidable() && img2->isCollidable() && img1->getDestRect() != nullptr &&
      img2->getDestRect() != nullptr) {

    doubleRect intersection;
    if (Util::getIntersection(img1, img2, &intersection)) {
      img1->notifyCollision(img2, &intersection);
    }
  }
}

// Updates the buckets for the specified image
//void CollisionDetector::updateBuckets(Image* image, int img_index) {
void CollisionDetector::updateBuckets(Image* image, Map* map) {
  unsigned int width;
  unsigned int height;
  if (map == nullptr) {
    width = WIDTH;
    height = HEIGHT;
  } else {
    width = map->width;
    height = map->height;
  }

  const int GRID_WIDTH = width / BUCKET_COLS;
  const int GRID_HEIGHT = height / BUCKET_ROWS;

  // Add image to all potential buckets
  doubleRect image_rect = image->getDoubleRect();
  unsigned int grid_x = image_rect.x / GRID_WIDTH;
  unsigned int grid_y = image_rect.y / GRID_HEIGHT;
  doubleRect grid_rect = {0, 0, (double) GRID_WIDTH, (double) GRID_HEIGHT};

  for (auto bucket = image->buckets.begin(); bucket != image->buckets.end();
    ) {
    grid_rect.x = floor(*bucket % BUCKET_COLS * GRID_WIDTH);
    grid_rect.y = floor(*bucket / BUCKET_ROWS * GRID_HEIGHT);
    if (!image->isCollidable() || !Util::isIntersecting(&grid_rect, &image_rect)) {
      auto img_bucket = &(buckets[*bucket]);
      img_bucket->erase(std::find((*img_bucket).begin(), (*img_bucket).end(), image));
      bucket = image->buckets.erase(bucket);
    } else {
      bucket++;
    }
  }

  if (!image->isCollidable()) return;

  grid_rect.x = grid_x * GRID_WIDTH;
  grid_rect.y = grid_y * GRID_HEIGHT;

  for (int i = grid_x; i < BUCKET_COLS &&
      Util::isIntersecting(&grid_rect, &image_rect); i++) {

    for (int j = grid_y; j < BUCKET_ROWS &&
        Util::isIntersecting(&grid_rect, &image_rect); j++) {

      unsigned int index = j * BUCKET_COLS + i;

      grid_rect.y = (j + 1) * GRID_HEIGHT;
      
      if (std::find(image->buckets.begin(), image->buckets.end(), index) !=
         image->buckets.end())
        continue;

      buckets[index].push_back(image);
      image->buckets.push_back(index);
    }

    grid_rect.x = (i + 1) * GRID_WIDTH;
    grid_rect.y = grid_y * GRID_HEIGHT;
  }
}

void CollisionDetector::initializeBuckets() {
  for (int i = 0; i < BUCKET_ROWS*BUCKET_COLS; i++) {
    buckets.push_back(collision_bucket());
  }
}
