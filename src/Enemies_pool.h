#ifndef ENEMIES_POOL_H
#define ENEMIES_POOL_H

#include "Enemy.h"
#include "globals.h"

void Enemies_pool_update(Enemy *enemies);
void Enemies_pool_init_round(int round, Config* config);

#endif
