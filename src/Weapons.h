#ifndef WEAPONS_H
#define WEAPONS_H

#include <SDL2/SDL_types.h>

// we need more ideas...
typedef enum { BASIC, HEAVY, SHOOTGUN, ROCKET } WeaponType;

typedef struct Weapon {

  float bulletSpeed;
  float bulletSize;
  float damage;
  WeaponType type;
  // 1ms = 1000000
  Uint32 delay;
  Uint32 lastShoot;

} Weapon;

void Weapon_get(Weapon *weapon, WeaponType type);

#endif
