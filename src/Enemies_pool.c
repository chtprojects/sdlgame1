#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Enemies_pool.h"
#include "Enemy.h"
#include "globals.h"
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

V2 pools[4];

long long enemyLastSpawn;
long long poolLastSpawn;
long long enemySpawnDelay=1000*1000000;
long long poolSpawnDelay=5000*1000000;


///////////////////////////////////SHOULD BE GLOBAL
static long long get_nanos() {
struct timespec ts;

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
clock_serv_t cclock;
mach_timespec_t mts;
host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
clock_get_time(cclock, &mts);
mach_port_deallocate(mach_task_self(), cclock);
ts.tv_sec = mts.tv_sec;
ts.tv_nsec = mts.tv_nsec;
#else
    timespec_get(&ts, TIME_UTC);
#endif
    return (long long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}


static void spawn_enemy(Enemy *enemies, V2 pool)
{
	Enemy_spawn(pool, enemies);
	//Enemy_spawn(pool.x-20,pool.y-20,enemies);
	//Enemy_spawn(pool.x-40,pool.y-40,enemies);
	//Enemy_spawn(pool.x-60,pool.y-60,enemies);
	//Enemy_spawn(pool.x-80,pool.y-80,enemies);
};

void Enemies_pool_init_round(int round, Config* config)
{
	switch(round)	
	{	
		case 0:
		pools[0].x=200;
		pools[0].y=200;
		pools[1].x=config->window_width-200;
		pools[1].y=200;
		pools[2].x=200;
		pools[2].y=config->window_height-200;
		pools[3].x=config->window_width-200;
		pools[3].y=config->window_height-200;
	}
}

void Enemies_pool_update(Enemy *enemies)
{
	        
	
	long long current= get_nanos();
	if(current-enemyLastSpawn>enemySpawnDelay)
	{
		spawn_enemy(enemies,pools[rand()%4]);
		enemyLastSpawn=current;	
	}
}


