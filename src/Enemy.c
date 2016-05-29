#include "Enemy.h"
#include "Geometry.h"
#include "engine_desktop.h"
#include "globals.h"
#include <math.h>
#include <SDL2/SDL.h>


static float speed = 1.0;
static int blocksCount = 16;
static float enemySize = 20;


extern Config *config;


static void Enemy_move_from_angle(Enemy *enemies, int index, float angle) {
    double val = M_PI / 180.0;

    float s = sin(angle*val);
    float c = cos(angle*val);

    enemies[index].vel = (V2) {speed*c*-1, speed*s*-1};
}


static void Enemy_update_pos(Enemy *enemies, int index) {
    Enemy *enemy = &enemies[index]; // TODO: stop passing indices in like this...
    enemy->collision_box.box.tl = V2add(enemy->collision_box.box.tl,
                                       enemy->vel);
}

int Enemy_count()
{
return config->enemiesCount;
}


void Enemy_spawn(V2 topLeft, Enemy *enemies ) {
  Enemy enemy;
  int i;
  float blockSize = enemySize/4;
  
  for(i=0;i<blocksCount;i++)
  {
      V2 off = (V2) { i/4 * blockSize, i%4 * blockSize };

      enemy.blocks[i].topLeft = V2add(topLeft, off);
	enemy.blocks[i].size=blockSize;
	enemy.blocks[i].destroyed=0;
  }
    enemy.vel = (V2) {0.0, 0.0};

  enemy.blocksAlive=blocksCount;

  enemy.collision_box.box = Quad_from(topLeft,
  									   enemySize, enemySize);
  enemy.collision_box.owner = ENEMY_OWNER;
  enemy.collision_box.ownerIdx =  config->enemiesCount;

    if (config->enemiesCount > 200) {
        abort();
    }
  enemies[config->enemiesCount]=enemy;

  config->enemiesCount+=1;
}

void Enemy_remove(Enemy *enemies, int index)
{
   if(config->enemiesCount<=0 || index>=config->enemiesCount)
   {
	return; 
   }
   Enemy tmp = enemies[config->enemiesCount-1];
   enemies[index]=tmp;
    enemies[index].collision_box.ownerIdx=index;
   config->enemiesCount-=1;
}

void Enemy_update(Enemy *enemies, Player *players, int activePlayers)
{
	int i;
	for(i=0;i<config->enemiesCount;i++)
	{
		double distance =0;
		int nearestPlayer=0;
		int j;
		for(j=0;j<activePlayers;j++)
		{
			double tmpDistance = distanceBetween(CBox_center(&enemies[i].collision_box),
                                                 CBox_center(&players[j].box));
			if(distance==0 ||  tmpDistance< distance){
				nearestPlayer=j;						
				distance =tmpDistance;		
			}
		}

        int angle = angleBetween(CBox_center(&players[nearestPlayer].box),
                                 CBox_center(&enemies[i].collision_box));
        Enemy_move_from_angle(enemies,i, angle);
		Enemy_update_pos(enemies,i);  
	}
  
}

void Enemy_render(Enemy *enemies)
{
	Engine_glPointSize((enemySize/4));
	for(int i=0;i<config->enemiesCount;i++)
	{
        Engine_glColor3f(0.0, 0.0, 1.0);
        Quad_render(&enemies[i].collision_box.box);
	}
}



