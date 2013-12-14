#ifndef EV_RECT_H_
#define EV_RECT_H_

namespace evil {

struct Rect
{
    Rect() = default;
    Rect( float xx, float yy, float ww, float hh) :
        x(xx), y(yy), w(ww), h(hh) { }
    float x = 0.0f, y = 0.0f, w = 0.0f, h = 0.0f;
};

}
#endif
