#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct s_v2 { double x, y; } V2;

V2 V2add(const V2 a, const V2 b);
V2 V2minus(const V2 a, const V2 b);
V2 V2mul(const V2 a, const V2 b);

typedef enum {
  PLAYER_OWNER,
  BULLET_OWNER,
  ENEMY_OWNER,
  SHIELD_OWNER
} Owner_type;

typedef struct s_quad {
  V2 tl; // top-left
  int w, h;
  double angle;
} Quad;

typedef struct collision_box_s {
  Quad box;
  Owner_type owner;
  int ownerIdx;
} Collision_box;

V2 CBox_center(Collision_box *cbox);

Quad Quad_from(V2 topLeft, double w, double h);
void Quad_update_position(Quad *q, V2 newTopLeft);
// If for convenience you want to update the quad to a new center
// position, call this function and it will calculate the top-left
// for you.
void Quad_update_from_center(Quad *q, V2 newCenter);
void Quad_render(const Quad *q);
void Quad_collides_with(const Quad *q1, const Quad *q2);

double angleBetween(V2 x, V2 y);
double distanceBetween(V2 x, V2 y);

#endif // GEOMETRY_H
