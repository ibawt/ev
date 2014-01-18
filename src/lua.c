#include <assert.h>

#include "ev_lua.h"
#include "evil.h"

static lua_State *lua_state = NULL;

static void* lua_alloc( void *ud, void *ptr, size_t osize, size_t nsize)
{
    if( nsize ) {
        return ev_realloc(ptr, nsize);
    } else if( ptr ){
        ev_free( ptr );
    }
    return NULL;
}

static int lua_log(lua_State *l)
{
    const char *s;

    s = lua_tostring( l, 1);
    ev_logger(EV_LOG, "%s", s);

    return 0;
}

static const luaL_Reg globals[] = {
    { "log", lua_log },
    { NULL, NULL }
};



static void open_lua_libs(void)
{
    luaopen_io(lua_state);
    luaopen_base(lua_state);
    luaopen_table(lua_state);
    luaopen_string(lua_state);
    luaopen_math(lua_state);
}

void ev_lua_init(void)
{
    assert( lua_state == NULL );
    lua_state = lua_newstate(lua_alloc, NULL);
    open_lua_libs();

    luaL_openlib(lua_state, "ev", globals, 0);
    lua_pop(lua_state, 1);
}

void ev_lua_destroy(void)
{
    lua_close(lua_state);
    lua_state = NULL;
}

lua_State *ev_lua_get_state(void)
{
    return lua_state;
}
