#ifndef EV_LUA_H_
#define EV_LUA_H_

#include "evil.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
extern "C"
#endif

EV_API void       ev_lua_init(void);
EV_API void       ev_lua_destroy(void);
EV_API lua_State* ev_lua_get_state(void);
EV_API void       ev_lua_dump_stack(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif
