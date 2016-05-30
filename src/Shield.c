//
//  Shield.c
//  SDLGame1
//
//  Created by Charlie Turner on 08/05/2016.
//  Copyright Â© 2016 Charlie Turner. All rights reserved.
//

#include "Shield.h"

#include "engine_desktop.h"

Shield *Shield_init(V2 center, V2 vel, GLfloat radius, int frags) {
  Shield *s = malloc(sizeof(Shield));
  s->center = center;
  s->vel = vel;
  s->radius = radius;
  s->used = 16;
  float steps = 2. * M_PI / (float)s->used;
  SDL_Log("Shield_init using %d steps", s->used);
  int i;
  float p;
  for (i = 0, p = 0.0; p < 2 * M_PI; p += steps, i++) {
    s->cboxes[i].box = Quad_from(V2minus(center, (V2){5, 5}), 10, 10);
    s->cboxes[i].owner = SHIELD_OWNER;
    s->cboxes[i].ownerIdx = i;
  }
  return s;
}

// Used by Main_sprite to change the center of the circle about
// which the shield rotates.
void Shield_update(Shield *s, float newCenterX, float newCenterY) {
#if 0
    //SDL_Log("shield_update before: center=%f,%f",s->center.x,s->center.y);
    s->center.x  = newCenterX;
    s->center.y = newCenterY;

    int i;
    for (i = 0; i < s->used; i++) {
        Quad *q =&s->cboxes[i].box;
        //Quad_update_about(q, s->center, s->radius);
    }
    //SDL_Log("shield_update after: center=%f,%f",s->center.x,s->center.y);
#endif
}

void Shield_point_at(Shield *s, GLfloat x, GLfloat y) {
  s->vel.x = x - s->center.x;
  s->vel.y = y - s->center.y;
  float hp = sqrt(s->vel.x * s->vel.x + s->vel.y * s->vel.y);
  s->vel.x /= hp;
  s->vel.y /= hp;
  return;
}

void Shield_render(const Shield *s) {
  int i;
  for (i = 0; i < s->used; i++) {
    Engine_glColor3f(0.1 * i, 0.0, 0.3 * i);
    Quad_render(&s->cboxes[i].box);
  }
}
