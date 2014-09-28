#include "evil.h"

#include <assert.h>
#include <stdlib.h>

#include "ev_lua.h"

static lua_State *lua_state = NULL;
ev_err_t ev_lua_load_libraries(lua_State *l)
{
    const char *dir;
    char path[512];

    dir = getenv("EV_LUA_LIBS");

    if (dir) {
        ev_log("overriding default lua libs");
        snprintf(path, sizeof(path), "%s/ev.lua", dir);

        if (luaL_dofile(l, path)) {
            ev_error("Error in lua: %s", lua_tostring(ev_lua_get_state(), -1));
            return EV_FAIL;
        }
    } else {
        /* if( luaL_dostring(l, ev_lua_lib)) { */
        /*    ev_error("Error in lua: %s", lua_tostring(ev_lua_get_state(), -1)); */
        /*    return EV_FAIL; */
        /* } */
    }

    return EV_OK;
}


static void open_lua_libs(void)
{
    luaL_openlibs(lua_state);
}

void ev_lua_init(void)
{
    assert( lua_state == NULL );
    lua_state = luaL_newstate();
    open_lua_libs();
    ev_lua_load_libraries(lua_state);
}

void ev_lua_destroy(void)
{
    assert( lua_state != NULL );

    lua_close(lua_state);
    lua_state = NULL;
}

lua_State *ev_lua_get_state(void)
{
    return lua_state;
}

void ev_lua_dump_stack(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        fprintf(stderr,"[%d]", i);
        int t = lua_type(L, i);
        switch (t) {

        case LUA_TSTRING:  /* strings */
            fprintf(stderr,"`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN:  /* booleans */
            fprintf(stderr, "%s", (lua_toboolean(L, i) ? "true" : "false"));
            break;

        case LUA_TNUMBER:  /* numbers */
            fprintf(stderr, "%g", lua_tonumber(L, i));
            break;

        default:  /* other values */
            fprintf(stderr,"%s", lua_typename(L, t));
            break;

        }
        fprintf(stderr, "  ");  /* put a separator */
      }
    fprintf(stderr, "\n");  /* end the listing */
}
