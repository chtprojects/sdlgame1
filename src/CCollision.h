//
// Collision detection routines & data structures
//

// This collision detection system works with quads. It assumes that
// the user has defined the parts of their game objects they wish to
// be considered for collision detection as quads.
//
// For simple games, the game objects that need to take account of
// collisions can simply be quads. A more complicated game might have
// an enemy with several arms that themselves are defined to have
// collision points represented by quads.
//
//

#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>

#include "Geometry.h"
#include <assert.h>

#define INVALID_BOX_ID ((Uint32)-1)

#define MAX_BOX_ELEMENTS  10

typedef struct quad_list {
    CQuad *quads[MAX_BOX_ELEMENTS];
    Uint8 len;
} quad_list ;

typedef struct cbox2d {
    quad_list *grid;
    int width_in_screen_pixels;
    int height_in_screen_pixels;
    int box_grid_width;
    int box_grid_height;
    int num_boxes;
} CCollision_box_2d;

CCollision_box_2d *CCollision_init(int screenW, int screenH, int num_x_boxes, int num_y_boxes);
void Collision_update_all(CCollision_box_2d *b2);
void Collision_adjust_for_quad_movement(CCollision_box_2d *b2, CQuad *q,float newX, float newY);

// return true if q1 collides with q2
int Collision_quads();

#endif