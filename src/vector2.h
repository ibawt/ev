#ifndef EV_VECTOR2_H_
#define EV_VECTOR2_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

ev_vec2 ev_vec2_add(ev_vec2 lh, ev_vec2 rh);
int     ev_vec2_equal(ev_vec2 lh, ev_vec2 rh);
ev_vec2 ev_vec2_mul(ev_vec2 lh, ev_vec2 rh);
float   ev_vec2_length(ev_vec2 v);
float   ev_vec2_lengthsq(ev_vec2 v);

#ifdef __cplusplus
}
#endif

#endif
