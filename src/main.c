#include "evil.h"
#include "ev_lua.h"
#include "application.h"

int main(int argc, char **argv)
{
    int ret = 0;

    ev_lua_init();

    if( argc < 2 ) {
        ev_error("Please supply a main lua file\n");
        ret = 1;
    } else {
        if( luaL_dofile(ev_lua_get_state(), argv[1] ) ) {
            ev_error( "Error in lua: %s", lua_tostring(ev_lua_get_state(),-1));
            ret = 1;
        }
    }

    ev_lua_destroy();

    return ret;
}
