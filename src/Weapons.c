#include "Weapons.h"

void Weapon_get(Weapon *weapon, WeaponType type) {
  switch (type) {
  case BASIC:
    weapon->bulletSpeed = 1;
    weapon->bulletSize = 2;
    weapon->delay = 1000; // 1sec
    weapon->lastShoot = 0;
    weapon->damage = 1;
    weapon->type = BASIC;
    break;
  case HEAVY:
    weapon->bulletSpeed = 4;
    weapon->bulletSize = 5;
    weapon->delay = 100;
    weapon->lastShoot = 0;
    weapon->damage = 2;
    weapon->type = HEAVY;
    break;
  case SHOOTGUN:
    weapon->bulletSpeed = 3;
    weapon->bulletSize = 5;
    weapon->delay = 1000; // 1sec
    weapon->lastShoot = 0;
    weapon->damage = 2;
    weapon->type = SHOOTGUN;
    break;
  case ROCKET:
    weapon->bulletSpeed = 4;
    weapon->bulletSize = 20;
    weapon->delay = 5000; // 5sec
    weapon->lastShoot = 0;
    weapon->damage = 5;
    weapon->type = ROCKET;
    break;
  }
}
