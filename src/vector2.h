#ifndef EV_VECTOR2_H_
#define EV_VECTOR2_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

EV_API ev_vec2 ev_vec2_add(ev_vec2 lh, ev_vec2 rh);
EV_API ev_bool ev_vec2_equal(ev_vec2 lh, ev_vec2 rh);
EV_API float   ev_vec2_length(ev_vec2 v);
EV_API float   ev_vec2_lengthsq(ev_vec2 v);

EV_API ev_vec2 ev_vec2_scale(ev_vec2 v, float s);
EV_API ev_vec2 ev_vec2_normalize(ev_vec2 v);

#ifdef __cplusplus
}
#endif

#endif
