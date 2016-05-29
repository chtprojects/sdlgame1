//
//  FrameRate.h
//  xxxxxxxx
//
//  Created by Charlie Turner on 21/05/2016.
//  Copyright © 2016 Charlie Turner. All rights reserved.
//

#ifndef FrameRate_h
#define FrameRate_h

#include <SDL2/SDL.h>

void frame_begin();
void frame_end();
Uint32 print_fr(Uint32 interval, void *);
void show_fps();

#endif /* FrameRate_h */
