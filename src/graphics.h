#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "evil.h"

typedef struct _ev_graphics ev_graphics;

EV_API ev_graphics* ev_graphics_create(void);
EV_API void ev_graphics_destroy(ev_graphics*);
EV_API void ev_graphics_clear(ev_graphics*, float, float, float, float);

#endif
