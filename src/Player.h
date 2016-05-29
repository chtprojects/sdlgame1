//
//  Player.h
//  xxxxxxxx
//
//  Created by Charlie Turner on 28/05/2016.
//  Copyright © 2016 Charlie Turner. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include "Geometry.h"
#include "Weapons.h"

typedef enum { KEYBOARD, TOUCH, GAMEPAD } ControlsType;

typedef struct s_player {
  // implementation data
  int health;
  ControlsType controlsType;
  int controllerId;
  int isShooting;
  V2 speed;
  int move;
  V2 vel;
  Weapon weapon;
  Collision_box box;
} Player;

Player Player_init(V2 center, double width, double height);
void Player_move(Player *p, V2 newCenter);
void Player_render(const Player *p);
void Player_update(Player *p, int screenW, int screenH);
void Player_free(Player *p);

#endif /* Player_h */
