//
//  Shield.h
//  SDLGame1
//
//  Created by Charlie Turner on 08/05/2016.
//  Copyright © 2016 Charlie Turner. All rights reserved.
//

#ifndef Shield_h
#define Shield_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Geometry.h"

typedef struct shield {
  V2 center;
  V2 vel;
  GLfloat radius;
  Collision_box cboxes[16];
  int used;
} Shield;

// Return an initialized shield object centered
// at center with given radius and the given number
// rotating fragments.
Shield *Shield_init(V2 center, V2 vel, GLfloat radius, int frags);
void Shield_point_at(Shield *s, GLfloat x, GLfloat y);
void Shield_update(Shield *s, float newCenterX, float newCenterY);
void Shield_render(const Shield *s);

#endif /* Shield_h */
