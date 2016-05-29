#include "Geometry.h"
#include "globals.h"

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void init_player(Config *c, ControlsType type, V2 center) {
  c->players[c->activePlayers] =
      Player_init(center, 50.0 /*width*/, 50.0 /*height*/);
  c->players[c->activePlayers].controlsType = type;
  c->players[c->activePlayers].box.ownerIdx = c->activePlayers;
  c->activePlayers += 1;
}

void init_game(Config *c) {
  c->activePlayers = 0;
  // init_player(c,GAMEPAD,c->window_width*3.0/5.0,c->window_height*1.0/5.0 );
  V2 playerCenter = (V2){c->window_width / 2.0, c->window_height / 2.0};
  init_player(c, KEYBOARD, playerCenter);
}
