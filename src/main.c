//////////DESTOP IMPORTS////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "engine_desktop.h"

//////////MOBILE IMPORTS//////////
/*
#include <SDL.h>
#include "engine_mobile.h"
*/
//////////GLOBAL IMPORTS////////
#include "Geometry.h"
#include "game.h"
#include "Bullet.h"
#include "Shield.h"
#include "Controlls.h"
#include "Explosion.h"
#include "Enemy.h"
#include "Enemies_pool.h"
#include "FrameRate.h"
#include "Collision.h"

#define APPNAME "SDL"
#define PI 3.14159265

Config *config;

/*
Grab events using SDL
*/

void hack_swap_gl() { SDL_GL_SwapWindow(config->window); }

static void process_events(Config *c, int *shouldQuit) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    Controlls_handle_events(config, event);
    switch (event.type) {
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        *shouldQuit = 1;
        break;
      case SDLK_m:
        Enemy_spawn((V2){100.0, 100.0}, config->enemies);
        break;
      }
      break;

    case SDL_QUIT:
      *shouldQuit = 1;
      break;

    case SDL_WINDOWEVENT: {
      switch (event.window.event) {
      case SDL_WINDOWEVENT_SHOWN:
        SDL_Log("Window %d shown", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_HIDDEN:
        SDL_Log("Window %d hidden", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_EXPOSED:
        SDL_Log("Window %d exposed", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_MOVED:
        SDL_Log("Window %d moved to %d,%d", event.window.windowID,
                event.window.data1, event.window.data2);
        break;
      case SDL_WINDOWEVENT_RESIZED:
        SDL_Log("Window %d resized to %dx%d", event.window.windowID,
                event.window.data1, event.window.data2);
        config->window_width = event.window.data1;
        config->window_height = event.window.data2;
        on_surface_changed(config->window_width, config->window_height);
        break;
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        SDL_Log("Window %d size changed to %dx%d", event.window.windowID,
                event.window.data1, event.window.data2);
        break;
      case SDL_WINDOWEVENT_MINIMIZED:
        SDL_Log("Window %d minimized", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_MAXIMIZED:
        SDL_Log("Window %d maximized", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_RESTORED:
        SDL_Log("Window %d restored", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_ENTER:
        SDL_Log("Mouse entered window %d", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_LEAVE:
        SDL_Log("Mouse left window %d", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        SDL_Log("Window %d gained keyboard focus", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        SDL_Log("Window %d lost keyboard focus", event.window.windowID);
        break;
      case SDL_WINDOWEVENT_CLOSE:
        SDL_Log("Window %d closed", event.window.windowID);
        break;
      default:
        SDL_Log("Window %d got unknown event %d", event.window.windowID,
                event.window.event);
        break;
      }
      break;
    }
    default:
      break;
    }
  }
}

int initialize_systems() {
  config = init_config();

  SDL_Window *win = 0;
  SDL_Renderer *ren = 0;

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    return 0;
  }
  win = SDL_CreateWindow("PONG!",                 // window title
                         SDL_WINDOWPOS_UNDEFINED, // initial x position
                         SDL_WINDOWPOS_UNDEFINED, // initial y position
                         800,                     // width, in pixels
                         600,                     // height, in pixels
                         SDL_WINDOW_OPENGL);
  if (win == 0) {
    //__android_log_print(ANDROID_LOG_ERROR, APPNAME, "SDL_CreateWindow: %s\n",
    // SDL_GetError());
    return 0;
  }
  config->window = win;
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                                        SDL_RENDERER_PRESENTVSYNC);
  if (ren == 0) {
    return 0;
  }

  SDL_GL_CreateContext(win);
  config->window = win;

  int width, height;
  SDL_GL_GetDrawableSize(config->window, &width, &height);
  config->window_width = width;
  config->window_height = height;
  setup_opengl(width, height);

  return 1;
}

void update_objects() {
  Player_update(&config->players[0], config->window_width,
                config->window_height);
  Enemy_update(config->enemies, config->players, config->activePlayers);

  if (config->players[0].isShooting) {
    Bullet_spam(CBox_center(&config->players[0].box),
                config->players[0].box.box.angle, config->bullets,
                &config->players[0].weapon);
  }

  Bullet_update(config->bullets, config->window_width, config->window_height);

  Explosion_update(config->explosions, config->window_width,
                   config->window_height);
}

void Background_render(V2 topLeft, double w, double h, float r, float g,
                       float b) {
  Engine_glColor3f(r, g, b);
  glRectd(topLeft.x, topLeft.y, topLeft.x + w, topLeft.y + h);
}

void render_objects() {
  Background_render((V2){0.0, 0.0}, config->window_width, config->window_height,
                    0.2, 0.2, 0.2);
  Player_render(&config->players[0]);
  Enemy_render(config->enemies);
  Bullet_render(config->bullets);
  Explosion_render(config->explosions);
}

int main(int argc, char **argv) {

  if (!initialize_systems()) {
    SDL_Log("initization failed");
    abort();
  }

  ////////////////////////////////////game initialization

  init_game(config);

  int done = 0;
  while (!done) {

    frame_begin();

    process_events(config, &done);

    update_objects();

    collision_detection();

    prepare_draw();

    render_objects();

    SDL_GL_SwapWindow(config->window);

    frame_end();
  }
  SDL_Quit();
  return 0;
}
