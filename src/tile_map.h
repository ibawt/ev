#ifndef EV_TILEMAP_H_
#define EV_TILEMAP_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MAP_ORTHO     = 0,
    MAP_ISOMETRIC = 1,
    MAP_STAGGERED = 2
} ev_map_type_t;

EV_API ev_tilemap* ev_tilemap_create(void);
EV_API void        ev_tilemap_destroy(ev_tilemap*);
EV_API ev_vec2*    ev_tilemap_get_position(ev_tilemap*);
EV_API void        ev_tilemap_set_position(ev_tilemap *, float x, float y);
EV_API void        ev_tilemap_render(ev_tilemap*);
EV_API void        ev_tilemap_update(ev_tilemap*, float);

#ifdef __cplusplus
}
#endif

#endif
