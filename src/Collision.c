#include "Enemy.h"
#include "Geometry.h"
#include "Bullet.h"
#include "globals.h"
#include "Explosion.h"
#include <SDL2/SDL.h>

extern Config *config;

Collision_box *collisions_arr[8096];
Uint32 ca_len=0;

void gather_collision_candidates() {
    int i,ai=0;

    for (i=0; i<config->activePlayers; i++){

        collisions_arr[ai++]=&config->players[i].box;
    }

    for (i=0; i<config->enemiesCount;i++)
    {
        collisions_arr[ai++]=&config->enemies[i].collision_box;
    }
    for (i=0; i<config->bulletsCount;i++) {
        collisions_arr[ai++]=&config->bullets[i].collision_box;
    }


    ca_len=ai;
    //SDL_Log("%d potential colliders\n",ai);
}


int Collision_quads(Quad *q1, Quad *q2) {
    float q1_left = q1->tl.x;
    float q1_top = q1->tl.y;
    float q1_bottom = q1_top + q1->h;
    float q1_right = q1_left + q1->w;


    float q2_left = q2->tl.x;
    float q2_top = q2->tl.y;
    float q2_bottom = q2_top + q2->h;
    float q2_right = q2_left + q2->w;

    int res= (q1_left < q2_right && q1_right > q2_left &&
              q1_top < q2_bottom && q1_bottom > q2_top );

    if(res)
        ;

    return res;
}

static void on_check(Bullet *bullets, Enemy *enemies, int bulletPos, int enemyPos, Explosion* explosions)
{
    Explosion_spawn(enemies[enemyPos].collision_box.box.tl, explosions,SPIRAL);
    Enemy_remove(enemies,enemyPos);
    Bullet_remove(bullets, bulletPos);
}

static void shield_hit(Enemy *enemies, int enemyPos, int shieldPos, Explosion *explosions)
{
    Explosion_spawn(enemies[enemyPos].collision_box.box.tl, explosions,SPIRAL);
    Enemy_remove(enemies,enemyPos);
}

typedef enum {OK,REV} col_order;

static col_order BULLET_ENEMY_COL(int a, int b) {
    if (collisions_arr[a]->owner == BULLET_OWNER && collisions_arr[b]->owner == ENEMY_OWNER)
        return OK;
    if (collisions_arr[a]->owner == ENEMY_OWNER && collisions_arr[b]->owner == BULLET_OWNER)
        return REV;

    return -1;

}

static col_order PLAYER_ENEMY_COL(int a, int b) {
    if (collisions_arr[a]->owner == PLAYER_OWNER && collisions_arr[b]->owner == ENEMY_OWNER)
        return OK;
    if (collisions_arr[a]->owner == ENEMY_OWNER && collisions_arr[b]->owner == PLAYER_OWNER)
        return REV;

    return -1;
}

static col_order ENEMY_SHIELD_COL(int a, int b) {
    if (collisions_arr[a]->owner == ENEMY_OWNER && collisions_arr[b]->owner == SHIELD_OWNER)
        return OK;
    if (collisions_arr[a]->owner == SHIELD_OWNER && collisions_arr[b]->owner == ENEMY_OWNER)
        return REV;

    return -1;
}

void player_hit(int p) {
    config->players[p].health -= 10;  //cautionary tale about mod 2^32
    if(config->players[p].health == 0)
    {
        config->players[p]=config->players[config->activePlayers-1];
        config->activePlayers--;
        SDL_Log("player %d died\n",p);
    }
}

void collision_detection() {
    int i,j,ncol=0,order;
    gather_collision_candidates();
    int num_be_cols=0;

    for(i=0; i<ca_len-1; i++) {
        for(j=i+1; j<ca_len; j++) {
            if(Collision_quads(&collisions_arr[i]->box,&collisions_arr[j]->box))
            {

                //Very hand debug code, don't delete
                //                    glColor3f(1., 1., 1.);
                //                    CQuad_render(&collisions_arr[i]->box);
                //                    hack_swap_gl();
                //                    SDL_Delay(100);
                //                    glColor3f(0.5, 0.5, 0.5);
                //                    CQuad_render(&collisions_arr[j]->box);
                //                    hack_swap_gl();
                //                    SDL_Delay(100);

                if ((order=BULLET_ENEMY_COL(i,j))!=-1)
                {   num_be_cols++;
                    order==OK ? on_check(config->bullets,config->enemies, collisions_arr[i]->ownerIdx, collisions_arr[j]->ownerIdx, config->explosions)
                    : on_check(config->bullets,config->enemies, collisions_arr[j]->ownerIdx, collisions_arr[i]->ownerIdx, config->explosions);
                }
                else if ((order=PLAYER_ENEMY_COL(i,j))!=-1)
                {
                    order==OK ? player_hit(collisions_arr[i]->ownerIdx)
                    : player_hit(collisions_arr[j]->ownerIdx);

                }
                else if ((order=ENEMY_SHIELD_COL(i, j))!=-1)
                {
                    order==OK ? shield_hit(config->enemies, collisions_arr[i]->ownerIdx, collisions_arr[j]->ownerIdx, config->explosions)
                    : shield_hit(config->enemies, collisions_arr[j]->ownerIdx, collisions_arr[i]->ownerIdx, config->explosions);
                }
                ncol++;
            }
        }
    }
    //if (num_be_cols!=0)SDL_Log("%d\n",num_be_cols);
    //SDL_Log("%d collisions\n",ncol);
}