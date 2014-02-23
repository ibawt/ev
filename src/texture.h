#ifndef EV_TEXTURE_H_
#define EV_TEXTURE_H_

#include "ev_lua.h"
#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

ev_texture* ev_texture_create(void);
void        ev_texture_destroy(ev_texture*);
void        ev_texture_bind(ev_texture*);
ev_err_t    ev_texture_load(ev_texture*, const char*);
uint32_t    ev_texture_get_width(ev_texture*);
uint32_t    ev_texture_get_height(ev_texture*);

ev_texture* ev_texture_from_lua(lua_State *l, int arg);
ev_err_t    ev_texture_lua_init(lua_State *l);

#ifdef __cplusplus
}
#endif

#endif
