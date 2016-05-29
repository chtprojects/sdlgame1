#include "Geometry.h"

#include <SDL2/SDL_opengl.h>
#include <math.h>

#include "engine_desktop.h"

/*
    x,y----------x+w,y
     |
     |
    x,y+h--------x+w,y+h
 */

static const double DEG2RAD = 180.0 / M_PI;

double angleBetween(V2 a, V2 b) {
    return atan2(b.y-a.y, b.x-a.x) * DEG2RAD;
}

double distanceBetween(V2 a, V2 b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return sqrt(dx*dx + dy*dy);
}

V2 CBox_center(Collision_box *cbox)
{
    return V2add(cbox->box.tl, (V2) {cbox->box.w / 2.0, cbox->box.h / 2.0});
}

V2 V2add(const V2 a, const V2 b) {
    V2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

V2 V2minus(const V2 a, const V2 b) {
    V2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

V2 V2mul(const V2 a, const V2 b) {
    V2 r;
    r.x = a.x * b.x;
    r.y = a.y * b.y;
    return r;
}


Quad Quad_from(V2 topLeft, double w, double h) {
    Quad q;
    q.tl = topLeft;
    q.w = w;
    q.h = h;
    q.angle = 0.0;
    return q;
}

void Quad_update_position(Quad *q, V2 newTopLeft) {
    q->tl = newTopLeft;
}

void Quad_update_from_center(Quad *q, V2 newCenter) {
    q->tl = V2minus(newCenter, (V2) {q->w, q->h});
}

void Quad_render(const Quad *q) {
    double val = M_PI / 180;
       float s = sin(q->angle*val);
        float c = cos(q->angle*val);
        float centerX = q->tl.x+q->w/2;
        float centerY = q->tl.y+q->h/2;


        float tlx = centerX+ ((q->tl.x-centerX) * c - (q->tl.y-centerY) * s);
        float tly = centerY+ ((q->tl.x-centerX) * s + (q->tl.y-centerY) * c);

        float trx = centerX+ ((q->tl.x+q->h-centerX) * c - (q->tl.y-centerY) * s);
        float try = centerY+ ((q->tl.x+q->h-centerX) * s + (q->tl.y-centerY) * c);

        float blx = centerX+ ((q->tl.x-centerX) * c - (q->tl.y+q->w-centerY) * s);
        float bly = centerY+ ((q->tl.x-centerX) * s + (q->tl.y+q->w-centerY) * c);

        float brx = centerX+ ((q->tl.x+q->h-centerX) * c - (q->tl.y+q->w-centerY) * s);
        float bry = centerY+ ((q->tl.x+q->h-centerX) * s + (q->tl.y+q->w-centerY) * c);

        Engine_glBegin(GL_TRIANGLE_STRIP);
        Engine_glVertex3f(tlx, tly, 0.0); //tl
        Engine_glVertex3f(blx, bly, 0.0); //bl
        Engine_glVertex3f(trx, try, 0.0); //tr
        Engine_glVertex3f(brx, bry, 0.0); //br
    Engine_glEnd();
}
