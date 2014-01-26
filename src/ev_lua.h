#ifndef EV_LUA_H_
#define EV_LUA_H_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int  ev_lua_create_ref(lua_State *L, int weak_ref);
void ev_lua_init(void);
void ev_lua_destroy(void);
lua_State* ev_lua_get_state(void);
void ev_lua_init_module(lua_State *l, luaL_Reg *funcs, const char *meta, const char *name);
#endif
