#include "boss_enemy.h"



Boss_Enemy::Boss_Enemy(SDL_Renderer *renderer, std::string filename, ErrorHandler *error_handler,
    int width, int height, int pos_x, int pos_y, double velocity_x,
    double velocity_y, int health)
    : Enemy(renderer, filename, error_handler, width, height, pos_x,
    pos_y, velocity_x, velocity_y) {

    hp = health;
}

int Boss_Enemy::changePhase(){
    phase++;
    return phase;
}
doubleRect Boss_Enemy::getDoubleRect(){
    return Enemy::getDoubleRect();
}
void Boss_Enemy::update(double){
  if (dead) return;
  if (hp <= 0) dead = true; 
}
void Boss_Enemy::notifyCollision(Image*, doubleRect*, bool){}
