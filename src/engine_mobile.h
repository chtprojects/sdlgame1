#include "Geometry.h"
void setup_opengl(int width, int height);
void prepare_draw();

void on_surface_changed(int width, int height);
void Engine_glBegin(GLenum type);
void Engine_glVertex3f(float x, float y, float z);
void Engine_glColor3f(float r, float g, float b);
void Engine_glVertex2d(float x, float y);
void Engine_glLineWidth(float size);
void Engine_glEnd();
