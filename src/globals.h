#ifndef GLOBALS_H
#define GLOBALS_H

#include "Geometry.h"
#include "Bullet.h"
#include "Shield.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Player.h"

typedef struct Config {
  Bullet bullets[200];
  Uint32 bulletsCount;
  Explosion explosions[200];
  Uint32 explosionsCount;
  Enemy enemies[200];
  Uint32 enemiesCount;
  Quad analogRight;
  Quad analogLeft;
  SDL_Window *window;
  double window_width, window_height;
  Player players[4];
  int activePlayers;
} Config;

Config *init_config();

#endif
