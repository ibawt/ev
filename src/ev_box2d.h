#ifndef EV_BOX2D_H_
#define EV_BOX2D_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ev_world ev_world;

ev_world* ev_world_create(void);
void      ev_world_destroy(ev_world*);
ev_vec2   ev_world_get_gravity(ev_world*);
void      ev_world_set_gravity(ev_vec2);

#ifdef __cplusplus
}
#endif

#endif
