#include "vector2.h"


ev_vec2 ev_vec2_add(ev_vec2 lh, ev_vec2 rh)
{
    ev_vec2 out;

    out.x = lh.x + rh.x;
    out.y = lh.y + rh.y;

    return out;
}
ev_bool ev_vec2_equal(ev_vec2 lh, ev_vec2 rh)
{
    return ev_cmpf(lh.x, rh.x) && ev_cmpf(lh.y, rh.y);
}

float ev_vec2_length(ev_vec2 v)
{
    float len = sqrtf( v.x * v.x + v.y * v.y );

    return len;
}
float ev_vec2_lengthsq(ev_vec2 v)
{
    return v.x * v.x + v.y * v.y;
}

ev_vec2 ev_vec2_scale(ev_vec2 v, float s)
{
    v.x *= s;
    v.y *= s;

    return v;
}

ev_vec2 ev_vec2_normalize(ev_vec2 v)
{
    float len = 1 / ev_vec2_length(v);

    v.x *= len;
    v.y *= len;

    return v;
}
