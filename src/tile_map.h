#ifndef EV_TILEMAP_H_
#define EV_TILEMAP_H_

#include "evil.h"

typedef enum {
    MAP_ORTHO     = 0,
    MAP_ISOMETRIC = 1,
    MAP_STAGGERED = 2
} ev_map_type_t;

ev_tilemap* ev_tilemap_create(void);
void        ev_tilemap_destroy(ev_tilemap*);
ev_vec2*    ev_tilemap_get_position(ev_tilemap*);
void        ev_tilemap_set_position(ev_tilemap *, float x, float y);
void        ev_tilemap_render(ev_tilemap*);
void        ev_tilemap_update(ev_tilemap*, float);

#endif
