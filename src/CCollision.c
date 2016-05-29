#include "CCollision.h"
#include "Geometry.h"

#include <math.h>
#include <stdio.h>

// const int MAX_BOX_ELEMENTS  = 6;
#define MAX_BOX_ELEMENTS 10
#define MAX_BOX_COLLISIONS 100
const int BOX_GRID_WIDTH = 20;
const int BOX_GRID_HEIGHT = 20;
// const int NUM_BOXES   = BOX_GRID_WIDTH * BOX_GRID_HEIGHT;
#define NUM_BOXES 400

CCollision_box_2d *CCollision_init(int screenW, int screenH, int num_x_boxes,
                                   int num_y_boxes) {
  CCollision_box_2d *b2 = malloc(sizeof(CCollision_box_2d));
  b2->width_in_screen_pixels = screenW / num_x_boxes;
  b2->height_in_screen_pixels = screenH / num_y_boxes;
  b2->box_grid_width = num_x_boxes;
  b2->box_grid_height = num_y_boxes;
  b2->num_boxes = num_x_boxes * num_y_boxes;
  b2->grid = calloc(b2->num_boxes, sizeof(quad_list));
  return b2;
}

extern void hack_swap_gl();
// helper for visu.
void dump_collision_data(const char *where) {
  FILE *fp = fopen(where, "a");
  if (fp == NULL) {
    printf("bug");
    abort();
  }
  for (int i = 0; i < NUM_BOXES; i++) {

    fprintf(fp, "%d, %d\n", i / BOX_GRID_WIDTH, i / BOX_GRID_HEIGHT);
    // fprintf(fp, "There are %d quads in box %d,%d\n", uniform_grid[i].len,
    //       i/BOX_GRID_WIDTH, i/BOX_GRID_HEIGHT);
  }
  hack_swap_gl();
  fclose(fp);
}

void grid_cell_add(quad_list *c, CQuad *quad) {
  assert(c->len < MAX_BOX_ELEMENTS && "increase resources");
  c->quads[c->len++] = quad;
}

void grid_cell_rm(quad_list *c, CQuad *quad) {
  if (c->len == 0)
    return;
  for (int i = 0; i < c->len; i++) {
    if (c->quads[i] == quad) {
      if (i == c->len - 1) {
        c->quads[i] = NULL;
        c->len--;
      } else {
        c->quads[i] = c->quads[c->len - 1];
        c->len--;
        c->quads[c->len] = NULL;
      }
    }
  }
}

// Boxes have associated unqiue ids. Each quad in the game
// has a unique id associated with it.

void each_collision_in(quad_list *b, CQuad *collider_pairs[],
                       int *num_colliders) {
  int num_found = 0;
  for (int i = 0; i < b->len - 1; i++) {
    for (int j = i + 1; j < b->len; j++) {
      if (Collision_quads(b->quads[i], b->quads[j])) {
        collider_pairs[num_found++] = b->quads[i];
        collider_pairs[num_found++] = b->quads[j];
      }
    }
  }
  assert((num_found & 1) == 0 || "non-even number of collisions - BUG");
  *num_colliders = num_found;
}

// void Collision_show_quads() {
//    for (int i=0; i < NUM_BOXES; i++) {
//        for (int b=0; b < uniform_grid[i].len; b++) {
//            glColor3f(0., 0.5, 0);
//            CQuad_render(uniform_grid[i].quads[b]);
//        }
//    }
//}

void Collision_update_all(CCollision_box_2d *b2) {

  //    Collision_show_quads();
  //    hack_swap_gl();
  //   return;

  CQuad *colliders_pairs[10];
  int num_colliders = 0;
  for (int i = 0; i < b2->num_boxes; i++) {
    each_collision_in(&b2->grid[i], colliders_pairs, &num_colliders);
    for (int i = 0; i < num_colliders; i += 2) {
      CQuad_collides_with(colliders_pairs[i], colliders_pairs[i + 1]);
      // note: if we assume symetrric collision properties, then adding
      // this next line will cause a "doublibg effect". in general,
      // object will react in different way when they interact, so we
      // want to let both objects know they collided
      // Another way: CQuad_collides with only updates its first
      // argument using information about its second.
      CQuad_collides_with(colliders_pairs[i + 1], colliders_pairs[i]);
    }
  }
}

Uint32 screen_coords_to_box_id(CCollision_box_2d *b2, float x, float y) {
  int sx = x / b2->width_in_screen_pixels;
  int sy = y / b2->height_in_screen_pixels;
  int p = sy * b2->box_grid_width + sx;
  assert(p >= 0 && p < b2->num_boxes && "bad screen coordinates");
  return p;
}

void Collision_adjust_for_quad_movement(CCollision_box_2d *b2, CQuad *q,
                                        float newX, float newY) {

  Uint32 next_grid_cell = screen_coords_to_box_id(b2, newX - 1, newY - 1);
  Uint32 curr_grid_cell = q->bid;
  if (next_grid_cell != curr_grid_cell) {
    // dump_collision_data("/tmp/quad_collisions.txt");
    // Remove q from the old grid cell, and add it to the new one.
    if (curr_grid_cell != INVALID_BOX_ID)
      grid_cell_rm(&b2->grid[curr_grid_cell], q);
    grid_cell_add(&b2->grid[next_grid_cell], q);
    q->bid = next_grid_cell;
  }
}

int Collision_quads(CQuad *q1, CQuad *q2) {
  float q1_left = q1->tl[0];
  float q1_top = q1->tl[1];
  float q1_bottom = q1_top + q1->h;
  float q1_right = q1_left + q1->w;

  float q2_left = q2->tl[0];
  float q2_top = q2->tl[1];
  float q2_bottom = q2_top + q2->h;
  float q2_right = q2_left + q2->w;

  int res = (q1_left < q2_right && q1_right > q2_left && q1_top < q2_bottom &&
             q1_bottom > q2_top);

  if (res) {
    glColor3f(0., 1., 0.);
    CQuad_render(q1);
    hack_swap_gl();
    SDL_Delay(100);
    glColor3f(1., 0.0, 0.0);
    CQuad_render(q2);
    hack_swap_gl();
    SDL_Delay(100);
  }

  return res;
}
