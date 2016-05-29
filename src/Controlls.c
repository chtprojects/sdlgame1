
#include "globals.h"
#include "Geometry.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265

int leftKeyPressed = 0;
int upKeyPressed = 0;
int downKeyPressed = 0;
int rightKeyPressed = 0;

double xRotationDir;
double yRotationDir;

double xMoveDir;
double yMoveDir;

void handle_mouse_motion(V2 mousePos, Config *c) {
  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == KEYBOARD) {
      double a = angleBetween(mousePos, CBox_center(&c->players[i].box));
      c->players[i].box.box.angle = a;
      return;
    }
  }
}

void handle_mouse_press(int press, Config *c) {
  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == KEYBOARD) {
      c->players[i].isShooting = press;
      return;
    }
  }
}

void set_movement_from_keys(Config *c) {

  static V2 leftUp = {-1, -1};
  static V2 leftDown = {-1, 1};
  static V2 rightUp = {1, -1};
  static V2 rightDown = {1, 1};
  static V2 left = {-1, 0};
  static V2 right = {1, 0};
  static V2 up = {0, -1};
  static V2 down = {0, 1};
  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == KEYBOARD) {
      if (leftKeyPressed && upKeyPressed)
        Player_move(&c->players[i], leftUp);
      else if (leftKeyPressed && downKeyPressed)
        Player_move(&c->players[i], leftDown);
      else if (rightKeyPressed && upKeyPressed)
        Player_move(&c->players[i], rightUp);
      else if (rightKeyPressed && downKeyPressed)
        Player_move(&c->players[i], rightDown);
      else if (leftKeyPressed)
        Player_move(&c->players[i], left);
      else if (rightKeyPressed)
        Player_move(&c->players[i], right);
      else if (upKeyPressed)
        Player_move(&c->players[i], up);
      else if (downKeyPressed)
        Player_move(&c->players[i], down);

      c->players[i].move = 1;
      return;
    }
  }
}

void set_movement_from_controller(Config *c, SDL_Event event) {

  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == GAMEPAD) {

      float value = (float)event.caxis.value / (float)32767;
      if (event.caxis.axis == 0)
        xMoveDir = -value;
      else if (event.caxis.axis == 1)
        yMoveDir = -value;
      else
        return;

      if (xMoveDir == 0 && yMoveDir == 0) {
        c->players[i].move = 0;
      } else {
        V2 dv = (V2){fabs(xMoveDir), fabs(yMoveDir)};
        c->players[i].vel = V2mul(c->players[i].vel, dv);
        c->players[i].move = 1;
      }
      return;
    }
  }
}

void set_angle_from_controller(Config *c, SDL_Event event) {

  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == GAMEPAD) {
      float value = (float)event.caxis.value / (float)32767;
      if (event.caxis.axis == 2)
        xRotationDir = -value;
      else if (event.caxis.axis == 3)
        yRotationDir = -value;
      else
        return;
      double angle = atan2(yRotationDir, xRotationDir) * (180.0 / PI);
      if (xRotationDir == 0 && yRotationDir == 0) {
        c->players[i].isShooting = 0;
      } else {
        c->players[i].isShooting = 1;
      }
      return;
    }
  }
}

void AddController(int id) {
  if (SDL_IsGameController(id)) {
    SDL_GameController *pad = SDL_GameControllerOpen(id);

    if (pad) {
      SDL_Joystick *joy = SDL_GameControllerGetJoystick(pad);
      int instanceID = SDL_JoystickInstanceID(joy);
      SDL_Log("pad connected %d", instanceID);
      // You can add to your own map of joystick IDs to controllers here.
      // YOUR_FUNCTION_THAT_CREATES_A_MAPPING( id, pad );
    }
  }
}

void RemoveController(int id) {
  // SDL_GameController *pad = YOUR_FUNCTION_THAT_RETRIEVES_A_MAPPING( id );
  // SDL_GameControllerClose( pad );
}

void handle_finger_press(int press, Config *c) {
  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == TOUCH) {
      c->players[i].move = press;
      return;
    }
  }
}

void handle_finger_motion(float x, float y, Config *c) {
  int i;
  for (i = 0; i < c->activePlayers; i++) {
    if (c->players[i].controlsType == TOUCH) {
      int width, height;
      SDL_GL_GetDrawableSize(c->window, &width, &height);
      float inWorldX = x * width;
      float inWorldY = y * height;
      float analogLeftX = c->analogLeft.tl.x + c->analogLeft.w / 2;
      float analogLeftY = c->analogLeft.tl.x + c->analogLeft.h / 2;
      float analogRightX = c->analogRight.tl.x + c->analogRight.w / 2;
      float analogRightY = c->analogRight.tl.x + c->analogRight.h / 2;

      double dlx = (inWorldX - analogLeftX);
      double dly = (inWorldY - analogLeftY);
      float distanceToLeftStick = sqrt(dlx * dlx + dly * dly);

      double drx = (inWorldX - analogRightX);
      double dry = (inWorldY - analogRightY);
      float distanceToRightStick = sqrt(drx * drx + dry * dry);
      float treshold = width / 4;

      if (distanceToLeftStick < treshold) {
        //                  float angle =
        //                  get_angle_between_points(inWorldX,inWorldY,analogLeftX,analogLeftY);
        //                  c->players[i].mainSprite->quad.angle = angle;

      }

      else if (distanceToRightStick < treshold) {
        //                  c->players[i].mainSprite->move=1;
        //                   float angle =
        //                   get_angle_between_points(inWorldX,inWorldY,
        //                   analogRightX,analogRightY);
        //                   Main_sprite_move_angle(c->players[i].mainSprite,angle,20);
      }
    }
  }
}

void Controlls_handle_events(Config *c, SDL_Event event) {

  switch (event.type) {
  case SDL_CONTROLLERDEVICEADDED:
    // SDL_Log("controler device added %d",event.cdevice.which);
    AddController(event.cdevice.which);

    break;

  case SDL_CONTROLLERDEVICEREMOVED:
    // SDL_Log("controler device removed");
    RemoveController(event.cdevice.which);
    break;

  case SDL_CONTROLLERBUTTONDOWN:
  case SDL_CONTROLLERBUTTONUP:
    // SDL_Log("controler device button");
    break;

  case SDL_CONTROLLERAXISMOTION:
    set_movement_from_controller(c, event);
    set_angle_from_controller(c, event);
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {

    case SDLK_w:
      upKeyPressed = 1;
      set_movement_from_keys(c);
      break;
    case SDLK_s:
      downKeyPressed = 1;
      set_movement_from_keys(c);
      break;
    case SDLK_a:
      leftKeyPressed = 1;
      set_movement_from_keys(c);
      break;
    case SDLK_d:
      rightKeyPressed = 1;
      set_movement_from_keys(c);
      break;
    }
    break;
  case SDL_KEYUP:
    switch (event.key.keysym.sym) {

    case SDLK_w:
      upKeyPressed = 0;
      Player_move(&c->players[0],
                  (V2){c->players[0].vel.x / c->players[0].speed.x, 0});

      break;
    case SDLK_s:
      downKeyPressed = 0;
      Player_move(&c->players[0],
                  (V2){c->players[0].vel.x / c->players[0].speed.x, 0});

      break;
    case SDLK_a:
      leftKeyPressed = 0;
      Player_move(&c->players[0],
                  (V2){0, c->players[0].vel.y / c->players[0].speed.y});

      break;
    case SDLK_d:
      rightKeyPressed = 0;
      Player_move(&c->players[0],
                  (V2){0, c->players[0].vel.y / c->players[0].speed.y});

      break;
    }
    break;

  case SDL_FINGERMOTION: {
    handle_finger_motion(event.tfinger.x, event.tfinger.y, c);

    break;
  }
  case SDL_FINGERUP:
    // SDL_Log("FINGER up %f %f ", event.tfinger.x, event.tfinger.y);
    // c->mainSprite->move=0;
    handle_finger_press(0, c);
    break;
  case SDL_FINGERDOWN:
    // SDL_Log("FINGER DOWN %f %f ", event.tfinger.x, event.tfinger.y);

    break;

  case SDL_MOUSEMOTION:

    handle_mouse_motion(
        (V2){
            event.motion.x, event.motion.y,
        },
        c);
    break;
  case SDL_MOUSEBUTTONUP:
    handle_mouse_press(0, c);
    // c->mainSprite->isShooting=0;
    // SDL_Log("MOUSE DOWN %d %d ", event.motion.x, event.motion.y);
    break;
  case SDL_MOUSEBUTTONDOWN:
    handle_mouse_press(1, c);
    // c->mainSprite->isShooting=1;
    // SDL_Log("MOUSE UP %d %d ", event.motion.x, event.motion.y);
    break;

  default:
    break;
  }
}
