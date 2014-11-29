#include "rect.h"

ev_rect ev_rect_create(float x, float y, float w, float h)
{
    ev_rect r;

    r.origin.x = x;
    r.origin.y = y;
    r.size.w = w;
    r.size.h = h;

    return r;
}
