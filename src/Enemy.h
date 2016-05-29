#ifndef ENEMY_h
#define ENEMY_h

#include "Geometry.h"
#include "Player.h"

typedef struct block {
  V2 topLeft;
  float size;
  int destroyed;
} Block;

typedef struct enemy {
  V2 vel;
  // 16 elements on each enemy
  Block blocks[16];
  // count of alive blocks
  int blocksAlive;
  // current room
  int roomIndex;
  Collision_box collision_box;
} Enemy;

void Enemy_spawn(V2 center, Enemy *enemies);
void Enemy_render(Enemy *enemies);
void Enemy_update(Enemy *enemies, Player *players, int activePlayers);
void Enemy_remove(Enemy *enemies, int index);
int Enemy_count();
#endif
