#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "engine_desktop.h"
#include "Geometry.h"
#include "globals.h"
#include <SDL2/SDL.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#define PI 3.14159265



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

extern Config *config;


static void set_movement(Bullet *bullets, int position, float angle, float speed)
{
    double val = PI / 180;

    float s = sin(angle*val);
    float c = cos(angle*val);


    if (position > 200)
        abort();

    bullets[position].vel = (V2) {speed*c*-1, speed*s*-1};

}

int Bullet_count()
{
	return config->bulletsCount;
}

void Bullet_remove(Bullet *bullets, int index)
{

   if (config->bulletsCount<=0 || index >=config->bulletsCount) {
       //printf("warning: %d\n", index); 
       return;
    }
   Bullet tmp = bullets[config->bulletsCount-1];
   bullets[index]=tmp;
    bullets[index].collision_box.ownerIdx=index;
   
   config->bulletsCount-=1;

}

void Bullet_spam(V2 center, int angle, Bullet *bullets, Weapon* weapon)
{
	if (config->bulletsCount>=200) {
		return;
	    }

	long long current = get_nanos();

	if(current- weapon->lastShoot>weapon->delay){

        Bullet b;
        b.center=center;
        b.size=weapon->bulletSize;
        b.weaponType=weapon->type;
        b.collision_box.box = Quad_from(V2minus(center,
                                                (V2){weapon->bulletSize/2,
                                                    weapon->bulletSize/2}),
                                        weapon->bulletSize,
                                        weapon->bulletSize);
        b.collision_box.owner=BULLET_OWNER;
        b.collision_box.ownerIdx=config->bulletsCount;
        bullets[config->bulletsCount]=b;

        set_movement(bullets,config->bulletsCount,angle,weapon->bulletSpeed);
        config->bulletsCount +=1;
        weapon->lastShoot=current;

    }

}

//FOR NOW WE PASS SCREEN SIZE, WE SHOULD HAVE SOME COLLISION DETECTION METHODS
void Bullet_update(Bullet *bullets, int width, int height)
{

	int i=0;
	for(i=0;i<config->bulletsCount;i++)	
	{
        bullets[i].center = V2add(bullets[i].center, bullets[i].vel);
		Quad_update_from_center(&bullets[i].collision_box.box,
                                bullets[i].center);
		if(bullets[i].center.x<0||bullets[i].center.x>width||bullets[i].center.y<0||bullets[i].center.y>height)
		{	
			Bullet_remove(bullets,i);
			return;		
		}
		//CQuad_update_position(&bullets[i].quad,newCenter);
		
		
	}
	
}

void Bullet_render(Bullet *bullets)
{
	
	//Engine_glPointSize(5);
	Engine_glColor3f(0.0,1.0,0.0);
	for(int i=0;i<config->bulletsCount;i++)
	{
        Quad_render(&bullets[i].collision_box.box);
	}

    //DEBUG CODE FOR COLLISION BOX
//	Engine_glColor3f(0.,1.,0.);
//	for(i=0;i<config->bulletsCount;i++)
//	{
//		CQuad_render(&bullets[i].collision_box.box);
//	}

}



