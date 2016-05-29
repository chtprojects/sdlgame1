#ifndef CONTROLLS_H
#define CONTROLLS_H


void handle_mouse_motion(float x, float y,Config* c);
void handle_finger_motion(float x, float y,Config* c);
static void set_movement_from_keys(Config *c);
static void set_movement_from_controller(Config *c, SDL_Event event);
void AddController( int id );
void RemoveController( int id );
void Controlls_handle_events(Config* c,SDL_Event event);

#endif
