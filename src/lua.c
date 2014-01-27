#include <assert.h>

#include "ev_lua.h"
#include "evil.h"

static lua_State *lua_state = NULL;

ev_err_t ev_application_lua_init(lua_State *l);
ev_err_t ev_sbatch_lua_init(lua_State *l);
ev_err_t ev_sprite_lua_init(lua_State *l);
ev_err_t ev_anim_lua_init(lua_State *l);
ev_err_t ev_texture_lua_init(lua_State *l);

int ev_lua_create_ref(lua_State *L, int weak_ref)
{
    assert( L != NULL );

    lua_newtable(L); /* new_table={} */

    if (weak_ref) {
        lua_newtable(L); /* metatable={} */

        lua_pushliteral(L, "__mode");
        lua_pushliteral(L, "v");
        lua_rawset(L, -3); /* metatable._mode='v' */

        lua_setmetatable(L, -2); /* setmetatable(new_table,metatable) */
    }

    lua_pushvalue(L,-2); /* push the previous top of stack */
    lua_rawseti(L,-2,1); /* new_table[1]=original value on top of the stack */

    return luaL_ref(L, LUA_REGISTRYINDEX); /* this pops the new_table */
}

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
    assert( l != NULL );

    s = lua_tostring( l, 1);
    ev_logger(EV_LOG, "[LOG-Lua]: %s", s);

    return 0;
}

static const luaL_Reg globals[] = {
    { "log", lua_log },
    { NULL, NULL }
};

static void open_lua_libs(void)
{
    luaL_openlibs(lua_state);
}

void ev_lua_init(void)
{
    assert( lua_state == NULL );
    lua_state = lua_newstate(lua_alloc, NULL);
    open_lua_libs();

    luaL_newmetatable(lua_state, "ev_meta");
    luaL_setfuncs( lua_state, globals, 0 );
    lua_pushvalue( lua_state, -1);
    lua_setfield( lua_state, -1, "__index");
    lua_setglobal(lua_state, "ev");

    ev_application_lua_init(lua_state);

    ev_sbatch_lua_init(lua_state);
    ev_sprite_lua_init(lua_state);
    ev_anim_lua_init(lua_state);
    ev_texture_lua_init(lua_state);
    ev_stage_lua_init(lua_state);
}

void ev_lua_destroy(void)
{
    assert( lua_state != NULL );

    lua_close(lua_state);
    lua_state = NULL;
}

lua_State *ev_lua_get_state(void)
{
    assert( lua_state != NULL );

    return lua_state;
}

void ev_lua_init_module(lua_State *l, luaL_Reg *funcs, const char *meta, const char *name)
{
    luaL_newmetatable(l, meta );
    luaL_setfuncs( l, funcs, 0 );
    lua_pushvalue(l, -1);
    lua_setfield(l, -1, "__index");
    lua_getglobal(l, "ev");
    lua_pushvalue(l, -2);
    lua_setfield(l, -2, name);
}
