#ifndef EV_STAGE_H_
#define EV_STAGE_H_

#include "ev_lua.h"
#include "evil.h"
#include "matrix4.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ev_stage_render_fn)(void *d, ev_matrix4* );
typedef void (*ev_stage_update_fn)(void *, float);

ev_stage* ev_stage_create(void);
void      ev_stage_destroy(ev_stage *s);

void ev_stage_add_actor(ev_stage *stage, ev_stage_render_fn func, ev_stage_update_fn update, void *d);
void ev_stage_remove_actor(ev_stage *stage, void *d);
void ev_stage_update(ev_stage *stage, float dt);
void ev_stage_render(ev_stage *s);
void ev_stage_set_transform(ev_stage*, const ev_matrix4 *);

ev_err_t ev_stage_lua_init(lua_State *l);
ev_stage* ev_stage_from_lua(lua_State *l, int arg);

#ifdef __cplusplus
}
#endif

#endif
