//
//  FrameRate.c
//  xxxxxxxx
//
//  Created by Charlie Turner on 21/05/2016.
//  Copyright © 2016 Charlie Turner. All rights reserved.
//

#include "FrameRate.h"

//#include <GLUT/glut.h>

static Uint32 frame_start_time, frame_end_time, frame_counter;
static double frame_seconds = 0.0, frame_acc_time = 0.0;
static double frame_update_rate = 0.5;
static Uint32 frame_rate_var;

static char frame_str[16] = "fps: xx\0";

void frame_begin() { frame_start_time = SDL_GetTicks(); }

//// Routine to draw a bitmap character string.
// void writeBitmapString(void *font, char *string)
//{
//    char *c;
//
//    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
//}
//
// void show_fps() {
//    glColor3f(1.0, 0.0, 0.0);
//    glRasterPos3f(500.0, 100.0, 0.0);
//    writeBitmapString(GLUT_BITMAP_8_BY_13, frame_str);
//}

void frame_end() {
  frame_end_time = SDL_GetTicks();
  frame_seconds = ((double)(frame_end_time - frame_start_time) / 1000.0f);
  frame_acc_time += frame_seconds;
  frame_counter++;
  if (frame_acc_time > frame_update_rate) {
    frame_rate_var = round((double)frame_counter / frame_acc_time);
    frame_counter = 0;
    frame_acc_time = 0.0;
    sprintf(frame_str, "fps: %2d", frame_rate_var);
  }
}

Uint32 print_fr(Uint32 interval, void *param) {
  (void)param;
  SDL_Log("Current frame rate: %i\n", frame_rate_var);
  return interval;
}