#ifndef EV_RECT_H_
#define EV_RECT_H_

namespace evil {

struct Rect
{
    Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) { }
    Rect( float xx, float yy, float ww, float hh) :
        x(xx), y(yy), w(ww), h(hh) { }
    float x,y,w,h;
};

}
#endif
