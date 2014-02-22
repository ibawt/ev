#include <assert.h>
#include <stdio.h>

#include "utils.h"
#include "ev_lua.h"
#include "sprite.h"
#include "animation.h"
#include "sprite_sheet.h"

struct _ev_sprite
{
    ev_vec2  position;
    float    rotation;
    float    scale;
    int      visible;
    ev_anim *animation;
    int      lua_ref;
};

static void ev_sprite_init(ev_sprite *s)
{
    assert( s != NULL);

    memset(s, 0, sizeof(ev_sprite));
    s->visible = 1;
    s->scale = 1.0f;
}

ev_sprite* ev_sprite_create(void)
{
    ev_sprite *s = ev_malloc(sizeof(ev_sprite));
    if( s ) {
        ev_sprite_init(s);
    }
    return s;
}


void ev_sprite_destroy(ev_sprite* s)
{
    if( s ) {
        ev_free(s);
    }
}

ev_vec2* ev_sprite_get_position(ev_sprite* s)
{
    return s ? &s->position : NULL;
}

void ev_sprite_set_position(ev_sprite *s, float x, float y)
{
    if( s ) {
        s->position.x = x;
        s->position.y = y;
    }
}

float ev_sprite_get_rotation(ev_sprite* s)
{
    return s ? s->rotation : 0.0f;
}

void ev_sprite_set_rotation(ev_sprite* s, float r)
{
    if( s ) {
        s->rotation = r;
    }
}

void ev_sprite_set_animation(ev_sprite* s, ev_anim* a)
{
    if( s ) {
        s->animation = a;
    }
}

ev_anim* ev_sprite_get_animation(ev_sprite* s)
{
    return s ? s->animation : NULL;
}

void ev_sprite_update(ev_sprite* s, float dt)
{
    if( s ) {
        if( s->animation ) {
            ev_anim_update(s->animation, dt);
        }
    }
}

void ev_sprite_render(ev_sprite* s)
{
}

int ev_sprite_get_visiblity(ev_sprite *s)
{
    assert( s != NULL );

    return s->visible;
}

int ev_sprite_fill(ev_sprite* s, ev_bvertex* b)
{
    int i;
    ev_bvertex *src;

    if( s && b && s->visible) {
        src = ev_sframe_get_bvertex(ev_anim_get_current_sframe(s->animation));
        if( src ) {

            for( i = 0 ; i < EV_SPRITE_NUM_VERTS ; ++i,b++,src++ ) {
                *b = *src;
                b->scale = s->scale;
                b->rotation = s->rotation;
                b->tx = s->position.x;
                b->ty = s->position.y;
            }
        }
        return 1;
    }
    return 0;
}

#define EV_SPRITE_META "__ev_sprite_meta"
#define EV_SPRITE_KEY "__ev_sprite"

#define check_sprite(l) ev_sprite_from_lua(l, 1)

ev_sprite* ev_sprite_from_lua(lua_State *l, int arg)
{
    ev_sprite *s;

    luaL_checktype(l, arg, LUA_TTABLE);
    lua_getfield(l, arg, EV_SPRITE_KEY);

    s = lua_touserdata(l, -1);
    luaL_argcheck(l, s != NULL, 1, "ev_sprite expected");

    return s;
}

int l_sprite_create(lua_State *l)
{
    ev_sprite *s;

    lua_newtable(l);
    luaL_getmetatable(l, EV_SPRITE_META);
    lua_setmetatable(l, -2);

    s = lua_newuserdata(l, sizeof(ev_sprite));
    ev_sprite_init(s);

    lua_setfield(l, -2, EV_SPRITE_KEY);
    s->lua_ref = ev_lua_create_ref(l, 1);

    return 1;
}

int l_sprite_destroy(lua_State *l)
{
    return 0;
}

static int l_sprite_set_animation(lua_State *l)
{
    ev_sprite *s;
    ev_anim   *a;

    s = check_sprite(l);

    a = ev_anim_from_lua(l, 2);

    ev_sprite_set_animation( s, a);

    lua_pop(l, 2);

    lua_setfield(l, -2, "animation");

    return 0;
}


static int l_sprite_set_rotation(lua_State *l)
{
    ev_sprite *s;
    float rot;

    s = check_sprite(l);
    rot = (float)lua_tonumber(l, 2);

    s->rotation = rot;

    return 0;
}

static int l_sprite_set_position(lua_State *l)
{
    ev_sprite *s;
    float x,y;

    s = check_sprite(l);

    x = (float)lua_tonumber(l, 2);
    y = (float)lua_tonumber(l, 3);

    s->position.x = x;
    s->position.y = y;

    return 0;
}

static int l_sprite_set_scale(lua_State *l)
{
    ev_sprite *s;
    float scale;

    s = check_sprite(l);

    scale = (float)lua_tonumber(l,2);

    s->scale = scale;

    return 0;
}

static int l_sprite_get_size(lua_State *l)
{
    ev_sprite *s;
    ev_sframe *frame;
    ev_size    size;

    s = check_sprite(l);

    frame = ev_anim_get_current_sframe(s->animation);
    size = ev_sframe_get_size(frame);

    lua_pop(l,1);

    lua_pushnumber(l, size.w);
    lua_pushnumber(l, size.h);

    return 2;
}

static int l_sprite_set_visibilty(lua_State *l)
{
    ev_sprite *s;

    s = check_sprite(l);

    s->visible = (int)lua_toboolean(l, 2);

    return 0;
}

static int l_sprite_get_visibilty(lua_State *l)
{
    ev_sprite *s;

    s = check_sprite(l);

    lua_pushboolean(l, s->visible);

    return 1;
}

ev_err_t ev_sprite_lua_init(lua_State *l)
{
    luaL_Reg luaFuncs[] = {
        { "create", l_sprite_create },
        { "__gc",   l_sprite_destroy },
        { "set_animation", l_sprite_set_animation },
        { "set_rotation", l_sprite_set_rotation },
        { "set_position", l_sprite_set_position },
        { "set_scale", l_sprite_set_scale },
        { "get_size", l_sprite_get_size },
        { "set_visiblity", l_sprite_set_visibilty },
        { "get_visibilty", l_sprite_get_visibilty },
        { 0, 0 }
    };

    ev_lua_init_module(l, luaFuncs, EV_SPRITE_META, "sprite");

    return EV_OK;
}
