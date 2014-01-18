#ifndef EV_LUA_H_
#define EV_LUA_H_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

void ev_lua_init(void);
void ev_lua_destroy(void);
lua_State* ev_lua_get_state(void);

#endif
