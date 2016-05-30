#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Bullet.h"
#include "engine_desktop.h"
#include "globals.h"

extern Config *config;

static void set_movement(Bullet *bullets, int position, float angle,
                         float speed) {
  float s = sin(angle * DEG_TO_RAD);
  float c = cos(angle * DEG_TO_RAD);

  if (position > 200)
    abort();

  bullets[position].vel = (V2){speed * c * -1, speed * s * -1};
}

int Bullet_count() { return config->bulletsCount; }

void Bullet_remove(Bullet *bullets, int index) {

  if (config->bulletsCount <= 0 || index >= config->bulletsCount) {
    // printf("warning: %d\n", index);
    return;
  }
  Bullet tmp = bullets[config->bulletsCount - 1];
  bullets[index] = tmp;
  bullets[index].collision_box.ownerIdx = index;

  config->bulletsCount -= 1;
}

void Bullet_spam(V2 center, int angle, Bullet *bullets, Weapon *weapon) {
  if (config->bulletsCount >= 200) {
    return;
  }

  Uint32 current = SDL_GetTicks();

  if (current - weapon->lastShoot > weapon->delay) {

    Bullet b;
    b.center = center;
    b.size = weapon->bulletSize;
    b.weaponType = weapon->type;
    b.collision_box.box = Quad_from(
        V2minus(center, (V2){weapon->bulletSize / 2, weapon->bulletSize / 2}),
        weapon->bulletSize, weapon->bulletSize);
    b.collision_box.owner = BULLET_OWNER;
    b.collision_box.ownerIdx = config->bulletsCount;
    bullets[config->bulletsCount] = b;

    set_movement(bullets, config->bulletsCount, angle, weapon->bulletSpeed);
    config->bulletsCount += 1;
    weapon->lastShoot = current;
  }
}

// FOR NOW WE PASS SCREEN SIZE, WE SHOULD HAVE SOME COLLISION DETECTION METHODS
void Bullet_update(Bullet *bullets, int width, int height) {

  int i = 0;
  for (i = 0; i < config->bulletsCount; i++) {
    bullets[i].center = V2add(bullets[i].center, bullets[i].vel);
    Quad_update_from_center(&bullets[i].collision_box.box, bullets[i].center);
    if (bullets[i].center.x < 0 || bullets[i].center.x > width ||
        bullets[i].center.y < 0 || bullets[i].center.y > height) {
      Bullet_remove(bullets, i);
      return;
    }
    // CQuad_update_position(&bullets[i].quad,newCenter);
  }
}

void Bullet_render(Bullet *bullets) {

  // Engine_glPointSize(5);
  Engine_glColor3f(0.0, 1.0, 0.0);
  for (int i = 0; i < config->bulletsCount; i++) {
    Quad_render(&bullets[i].collision_box.box);
  }

  // DEBUG CODE FOR COLLISION BOX
  //	Engine_glColor3f(0.,1.,0.);
  //	for(i=0;i<config->bulletsCount;i++)
  //	{
  //		CQuad_render(&bullets[i].collision_box.box);
  //	}
}
