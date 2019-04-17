#include "util.h"

bool Util::isIntersecting(doubleRect* a, doubleRect* b) {
  doubleRect intersection = {0, 0, 0, 0};
  return Util::getIntersection(a, b, &intersection);
}
bool Util::isIntersecting(Image* a, Image* b) {
  doubleRect aRect = a->getDoubleRect();
  doubleRect bRect = b->getDoubleRect();
  return isIntersecting(&aRect, &bRect);
}

bool Util::getIntersection(Image* a, Image* b, doubleRect* intersection) {
  doubleRect aRect = a->getDoubleRect();
  doubleRect bRect = b->getDoubleRect();
  return Util::getIntersection(&aRect, &bRect, intersection);
}
bool Util::getIntersection(doubleRect* a, doubleRect* b,
  doubleRect* intersection) {

  double aMin, bMin, aMax, bMax;

  aMin = a->x;
  aMax = aMin + a->w;
  bMin = b->x;
  bMax = bMin + b->w;
  if (bMin > aMin)
    aMin = bMin;
  intersection->x = aMin;
  if (bMax < aMax)
    aMax = bMax;
  intersection->w = aMax - aMin;

  aMin = a->y;
  aMax = aMin + a->h;
  bMin = b->y;
  bMax = bMin + b->h;
  if (bMin > aMin)
    aMin = bMin;
  intersection->y = aMin;
  if (bMax < aMax)
    aMax = bMax;
  intersection->h = aMax - aMin;

  if (intersection->w > 0 && intersection->h > 0) return true;
  return false;
}

doubleRect Util::toDoubleRect(SDL_Rect a) {
  doubleRect b;
  b.x = (double) a.x;
  b.y = (double) a.y;
  b.w = (double) a.w;
  b.h = (double) a.h;

  return b;
}
