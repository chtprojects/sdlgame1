#ifndef ENGINE_DESKTOP
#define ENGINE_DESKTOP

#include "Geometry.h"

#include <SDL2/SDL_opengl.h>

void setup_opengl( int width , int height );
void prepare_draw();
void on_surface_changed(int width, int height);
void Engine_set_camera(float width, float height, float x,float y, float z);
void Engine_glBegin(GLenum type);
void Engine_glVertex3f(float x, float y, float z);
void Engine_glColor3f(float r, float g, float b);
void Engine_glVertex2d(double x, double y);
void Engine_glLineWidth(float size);
void Engine_glPointSize(float size);
void Engine_glEnd();

#endif

