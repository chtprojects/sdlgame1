#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "Geometry.h"

void Engine_set_camera(float width, float height, float x, float y, float z) {

  int newHeight = height / z;
  int newWidth = width / z;

  int diffH = newWidth / 2;
  int diffV = newHeight / 2;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(x - diffH, x + diffH, y + diffV, y - diffV, -1, 1);
  glMatrixMode(GL_MODELVIEW);
}

void setup_opengl(int width, int height) {
  glClearColor(0, 0, 0, 0);

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, width, height, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void prepare_draw() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void on_surface_changed(int width, int height) {
  glViewport(0, 0, width, height);
}

void Engine_glBegin(GLenum type) { glBegin(type); }

void Engine_glVertex3f(float x, float y, float z) { glVertex3f(x, y, z); }
void Engine_glColor3f(float r, float g, float b) { glColor3f(r, g, b); }
void Engine_glVertex2d(double x, double y) { glVertex2d(x, y); }

void Engine_glEnd() { glEnd(); }

void Engine_glLineWidth(float size) { glLineWidth(size); }

void Engine_glPointSize(float size) { glPointSize(size); }
