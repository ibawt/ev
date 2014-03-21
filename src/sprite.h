#ifndef EV_SPRITE_H_
#define EV_SPRITE_H_

#define EV_SPRITE_NUM_VERTS 6

#include "ev_lua.h"
#include "evil.h"
#include "ev_box2d.h"

#ifdef __cplusplus
extern "C" {
#endif

EV_API ev_sprite*    ev_sprite_create(void);
EV_API ev_vec2*      ev_sprite_get_position(ev_sprite*);
EV_API void          ev_sprite_set_position(ev_sprite*, float x, float y);
EV_API float         ev_sprite_get_rotation(ev_sprite*);
EV_API void          ev_sprite_set_rotation(ev_sprite*, float);
EV_API void          ev_sprite_set_animation(ev_sprite*, ev_anim*);
EV_API ev_anim*      ev_sprite_get_animation(ev_sprite*);
EV_API void          ev_sprite_update(ev_sprite*, float);
EV_API void          ev_sprite_render(ev_sprite*);
EV_API int           ev_sprite_fill(ev_sprite*, ev_bvertex*);
EV_API void          ev_sprite_set_body(ev_sprite *, ev_body *);

#ifdef __cplusplus
}
#endif



#endif
