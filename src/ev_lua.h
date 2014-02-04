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
void ev_lua_dump_stack(lua_State *L);

int ev_lua_table_insert(lua_State *l);
int ev_lua_table_remove(lua_State *l);

#define ev_lua_getn(L,n)	(luaL_checktype(L, n, LUA_TTABLE), luaL_len(L, n))

#endif
