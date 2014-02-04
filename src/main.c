#include <stdio.h>
#include "evil.h"
#include "ev_lua.h"
#include "application.h"

int main(int argc, char **argv)
{
    ev_lua_init();

    if( argc < 2 ) {
        fprintf(stderr, "Please supply a main lua file\n");
    } else {
        if( luaL_dofile(ev_lua_get_state(), argv[1] ) ) {
            ev_error( "Error in lua: %s", lua_tostring(ev_lua_get_state(),-1));
        }
        ev_app_start(NULL);
    }
    ev_lua_destroy();
}
