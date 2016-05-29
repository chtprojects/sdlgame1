//
//  Player.c
//  xxxxxxxx
//
//  Created by Charlie Turner on 28/05/2016.
//  Copyright © 2016 Charlie Turner. All rights reserved.
//

#include "Player.h"
#include "Weapons.h"

#include "engine_desktop.h"

#include <stdlib.h>
#include <math.h>

Player Player_init(V2 center, double width, double height)
{
    Player p;
    p.health = 100;
    p.isShooting = 0;
    p.vel = (V2) { 0, 0 };
    p.move = 0;
    p.speed = (V2) {5.0, 5.0};

    V2 tl = V2minus(center, (V2) {width/2, height/2});
    p.box.box = Quad_from(tl, width, height);
    p.box.owner = PLAYER_OWNER;
    p.box.ownerIdx = 0;

    Weapon_get(&p.weapon, HEAVY);
    return p;
}

void Player_move(Player *p, V2 vel) {
    p->vel = V2mul(vel, p->speed);
}

void Player_render(const Player *p) {
    Engine_glColor3f(1.0, 0.0, 0.0);
    Quad_render(&p->box.box);
}
void Player_update(Player *p, int screenW, int screenH) {
    if (!p->move) return;
    p->box.box.tl = V2add(p->box.box.tl, p->vel);
}
void Player_free(Player *p) {
    free(p);
}