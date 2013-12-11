#ifndef EVIL_H_
#define EVIL_H_

#include <math.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#ifdef  __APPLE__
#include <OpenGL/glu.h>
#else
#include "GL/glu.h"
#endif

namespace evil {

void log(const char *fmt, ...);
void error(const char *fmt, ... );
void SetError(const char *);


struct Vector2 {
    Vector2() = default;
    Vector2( float xx, float yy) : x(xx), y(yy) { }

    float getLength() const {
        return sqrtf( x * x + y * y );
    }

    void normalize() {
    }
    
    float x = 0.0f;
    float y = 0.0f;
};

struct Rect {
    Rect() = default;
    Rect( float xx, float yy, float ww, float hh) :
        x(xx), y(yy), w(ww), h(hh) { }
    float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
};
}   

#endif
