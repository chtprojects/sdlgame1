#ifndef BULLET_H
#define BULLET_H

#include "Geometry.h"
#include "Weapons.h"

typedef struct Bullet{

 V2 center;
 float size;
 float speed;
 V2 vel;
 WeaponType weaponType;
 Collision_box collision_box;
} Bullet;

void Bullet_spam(V2 center,int angle,Bullet *bullets,Weapon* weapon);
void Bullet_update(Bullet *bullets, int screenW, int screenH);
void Bullet_render(Bullet *bullets);
void Bullet_remove(Bullet *bullets, int index);
int Bullet_count();

#endif
