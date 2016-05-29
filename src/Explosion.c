#include "Explosion.h"
#include "engine_desktop.h"
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265

static int TINY_COUNT = 3;
static int SMALL_COUNT = 10;
static int MEDIUM_COUNT = 30;
static int BIG_COUNT = 50;

extern Config *config;

static void set_spiral_movement(Particle *p, float angle, float speed) {
  p->moveX = speed;
  p->moveY = speed;
}

static void set_movement(Particle *p, float angle, float speed) {

  double val = PI / 180;

  float s = sin(angle * val);
  float c = cos(angle * val);

  p->moveX = speed * c * -1;
  p->moveY = speed * s * -1;
}

void Particle_move(Particle *p, float t) {

  p->position.x = p->position.x + sin(t) * p->moveX;
  p->position.y = p->position.y + cos(t) * p->moveY;
}

static void Explosion_remove(Explosion *explosions, int position) {
  if (config->explosionsCount <= 0 || position >= config->explosionsCount) {
    return;
  }

  Explosion tmp = explosions[config->explosionsCount - 1];
  explosions[position] = tmp;
  config->explosionsCount -= 1;
}

static void Particle_remove(Explosion *explosions, int explosionIndex,
                            int particleIndex) {
  if (config->explosionsCount <= 0 || explosionIndex >= config->explosionsCount)
    return;

  if (explosions[explosionIndex].particlesCount <= 0 ||
      particleIndex >= explosions[explosionIndex].particlesCount)
    return;

  Particle tmp = explosions[explosionIndex]
                     .particles[explosions[explosionIndex].particlesCount - 1];
  explosions[explosionIndex].particles[particleIndex] = tmp;
  explosions[explosionIndex].particlesCount -= 1;

  if (explosions[explosionIndex].particlesCount <= 0) {
    Explosion_remove(explosions, explosionIndex);
  }
}

void Explosion_spawn(V2 topLeft, Explosion *explosions, ExplosionType type) {

  Explosion e;

  e.type = type;
  int maxSpeed;
  long long maxLifeTime;
  switch (type) {
  case TINY:
    e.particlesCount = TINY_COUNT;
    maxSpeed = 2;
    maxLifeTime = 2 * 1E9;
    break;
  case SMALL:
    e.particlesCount = SMALL_COUNT;
    maxSpeed = 7;
    maxLifeTime = 4 * 1E9;
    break;
  case MEDIUM:
    e.particlesCount = MEDIUM_COUNT;
    maxSpeed = 10;
    maxLifeTime = 8 * 1E9;
    break;
  case BIG:
  case SPIRAL:
    e.particlesCount = BIG_COUNT;
    maxSpeed = 20;
    maxLifeTime = 10 * 1E9;
    break;
  }

  Uint32 creationTime = SDL_GetTicks();

  int i = 0;
  for (i = 0; i < e.particlesCount; i++) {
    Particle p;
    p.position = topLeft;

    p.creationTime = creationTime;
    p.lifeTime = rand() % maxLifeTime;
    if (e.type == SPIRAL)
      set_spiral_movement(&p, rand() % 360, rand() % maxSpeed);
    else
      set_movement(&p, rand() % 360, rand() % maxSpeed);
    e.particles[i] = p;
  }
  explosions[config->explosionsCount] = e;
  config->explosionsCount += 1;
}

void Explosion_update(Explosion *explosions, int width, int height) {
  int i = 0;

  Uint32 current = SDL_GetTicks();

  for (i = 0; i < config->explosionsCount; i++) {
    int j;
    for (j = 0; j < explosions[i].particlesCount; j++) {
      Particle *p = &explosions[i].particles[j];
      if (explosions[i].type == SPIRAL) {

        long elapsed = current - p->creationTime;
        float percent = (float)elapsed / p->lifeTime;
        Particle_move(p,
                      2 * PI * percent); // todo need better time base movement
      } else {
        if (p->position.y < 0 || p->position.y > height)
          p->moveY = -p->moveY;
        if (p->position.x < 0 || p->position.x > width)
          p->moveX = -p->moveX;
        p->position.x = p->position.x + p->moveX;
        p->position.y = p->position.y + p->moveY;
      }
      if (current - p->creationTime > p->lifeTime) {
        Particle_remove(explosions, i, j);
      }
    }
  }
}

void Explosion_render(Explosion *explosions) {

  Engine_glPointSize(5);
  Engine_glColor3f(0.8, 0, 0);
  Engine_glBegin(GL_POINTS);
  int i;
  for (i = 0; i < config->explosionsCount; i++) {
    for (int j = 0; j < explosions[i].particlesCount; j++) {
      Particle p = explosions[i].particles[j];
      Engine_glVertex3f(p.position.x, p.position.y, 0); // tl
    }
  }
  Engine_glEnd();
}
