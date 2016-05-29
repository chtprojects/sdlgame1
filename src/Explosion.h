#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Geometry.h"

typedef struct Particle {
  V2 position;
  float size;
  float speed;
  float moveX;
  float moveY;

  // index in particles array
  long creationTime;
  long lifeTime;

} Particle;

typedef enum { TINY, SMALL, MEDIUM, BIG, SPIRAL } ExplosionType;

typedef struct Explosion {

  Particle particles[50];
  ExplosionType type;
  // count of particles in this explosion
  int particlesCount;

} Explosion;

void Explosion_render(Explosion *explosions);
void Explosion_update(Explosion *explosions, int width, int height);
void Explosion_spawn(V2 topLeft, Explosion *explosions, ExplosionType type);

#endif
