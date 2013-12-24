#ifndef EV_SPRITE_H_
#define EV_SPRITE_H_

#define EV_SPRITE_NUM_VERTS 6

#include "evil.h"

ev_sprite*    ev_sprite_create(void);
ev_vec2*      ev_sprite_get_position(ev_sprite*);
void          ev_sprite_set_position(ev_sprite*, float x, float y);
float         ev_sprite_get_rotation(ev_sprite*);
void          ev_sprite_set_rotation(ev_sprite*, float);
void          ev_sprite_set_animation(ev_sprite*, ev_anim*);
ev_anim*      ev_sprite_get_animation(ev_sprite*);
void          ev_sprite_update(ev_sprite*, float);
void          ev_sprite_render(ev_sprite*);
void          ev_sprite_fill(ev_bvertex*);

#endif
