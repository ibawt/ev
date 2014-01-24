#include <assert.h>

#include "texture.h"
#include "ev_lua.h"

#ifdef WIN32
#define COLOR_FMT GL_RGBA
#else
#define COLOR_FMT GL_BGRA
#endif

struct _ev_texture
{
    GLuint   id;
    uint32_t width;
    uint32_t height;
    int      lua_ref;
};

ev_texture* ev_texture_create(void)
{
    ev_texture *t = ev_malloc( sizeof(ev_texture));
    memset( t, 0, sizeof(ev_texture));

    return t;
}

void ev_texture_destroy(ev_texture *t)
{
    if( t ) {
        if( t->id ) {
            glDeleteTextures(1, &t->id);
            t->id = 0;
        }
        ev_free(t);
    }
}

void ev_texture_bind(ev_texture* t)
{
    if( t && t->id) {
        glBindTexture( GL_TEXTURE_2D, t->id );
    }
}

ev_err_t ev_texture_load(ev_texture* t, const char *filename)
{
    SDL_Surface *s;

    if( !t || !filename )
        return EV_FAIL;

    s = IMG_Load(filename);

    if( !s )
        return EV_FAIL;

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, COLOR_FMT, GL_UNSIGNED_BYTE, s->pixels);

    t->width = s->w;
    t->height = s->h;

    SDL_FreeSurface(s);

    return EV_OK;
}

#define EV_TEXTURE_META "__ev_texture_meta"
#define EV_TEXTURE_KEY "__ev_texture"

static ev_texture* check_texture(lua_State *l)
{
    ev_texture *t;

    luaL_checktype(l, 1, LUA_TTABLE);
    lua_getfield(l, 1, EV_TEXTURE_KEY);

    t = lua_touserdata(l, -1);

    luaL_argcheck(l, t != NULL, 1, "ev_texture expected");

    return t;
}

static void ev_texture_get_lua(ev_texture *t, lua_State *l)
{
    lua_gettable(l, LUA_REGISTRYINDEX);
    lua_rawgeti(l, -2, t->lua_ref);
    lua_rawgeti(l, 1, 1);
}

static int l_texture_load(lua_State *l)
{
    ev_texture *t;

    t = check_texture(l);

    if( ev_texture_load(t, lua_tostring(l, 2)) ) {
        lua_pushstring(l,"error in loading texture");
        lua_error(l);
    }
    return 0;
}

static int l_texture_destroy(lua_State *l)
{
    ev_texture *t;

    t = check_texture(l);

    if( t->id ) {
        glDeleteTextures(1, &t->id);
        t->id = 0;
    }
    return 0;
}

static int l_texture_create(lua_State *l)
{
    ev_texture *t;

    lua_newtable(l);
    luaL_getmetatable(l, EV_TEXTURE_META);
    lua_setmetatable(l, -2);

    t = lua_newuserdata( l, sizeof(ev_texture));
    memset(t, 0, sizeof(ev_texture));

    lua_setfield(l, -2, EV_TEXTURE_KEY);
    t->lua_ref = ev_lua_create_ref(l, 1);
    return 1;
}

static const luaL_Reg texture_lua_funcs[] = {
    { "create", l_texture_create },
    { "__gc",   l_texture_destroy },
    { "load",   l_texture_load },
};

ev_err_t ev_texure_lua_init(lua_State *l)
{
    assert( l != NULL );

    luaL_newmetatable(l, EV_TEXTURE_META);
    luaL_setfuncs(l, texture_lua_funcs, 0);
    lua_pushvalue(l, -1);
    lua_setfield(l, -1, "__index");
    lua_getglobal(l, "ev");
    lua_pushvalue(l, -2);
    lua_setfield(l, -2, "texture");

    return EV_OK;
}
