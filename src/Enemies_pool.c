#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Enemies_pool.h"
#include "Enemy.h"
#include "globals.h"
#include <SDL2/SDL.h>

V2 pools[4];

Uint32 enemyLastSpawn;
Uint32 poolLastSpawn;
Uint32 enemySpawnDelay = 1000;
Uint32 poolSpawnDelay = 5000;

static void spawn_enemy(Enemy *enemies, V2 pool) {
  Enemy_spawn(pool, enemies);
  // Enemy_spawn(pool.x-20,pool.y-20,enemies);
  // Enemy_spawn(pool.x-40,pool.y-40,enemies);
  // Enemy_spawn(pool.x-60,pool.y-60,enemies);
  // Enemy_spawn(pool.x-80,pool.y-80,enemies);
};

void Enemies_pool_init_round(int round, Config *config) {
  switch (round) {
  case 0:
    pools[0].x = 200;
    pools[0].y = 200;
    pools[1].x = config->window_width - 200;
    pools[1].y = 200;
    pools[2].x = 200;
    pools[2].y = config->window_height - 200;
    pools[3].x = config->window_width - 200;
    pools[3].y = config->window_height - 200;
  }
}

void Enemies_pool_update(Enemy *enemies) {

  Uint32 current = SDL_GetTicks();
  if (current - enemyLastSpawn > enemySpawnDelay) {
    spawn_enemy(enemies, pools[rand() % 4]);
    enemyLastSpawn = current;
  }
}
