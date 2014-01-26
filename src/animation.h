#ifndef EV_ANIMATION_H_
#define EV_ANIMATION_H_

#include "evil.h"
#include "ev_lua.h"

typedef enum {
		EV_LOOP = 0,
		EV_ONE_SHOT = 1,
		EV_REVERSE = 2,
		EV_PING_PONG = 3
} ev_anim_mode;

ev_anim*     ev_anim_create(void);
void         ev_anim_destroy(ev_anim *);
void         ev_anim_add_sframe(ev_anim*, ev_sframe*);
ev_sframe   *ev_anim_get_current_sframe(ev_anim*);
void         ev_anim_set_mode(ev_anim *, ev_anim_mode);
ev_anim_mode ev_anim_get_mode(ev_anim *);
void         ev_anim_update(ev_anim*, float);

ev_anim* ev_anim_from_lua(lua_State *l, int arg);
#endif
