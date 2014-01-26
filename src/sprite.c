#include <stdio.h>

#include "ev_lua.h"
#include "sprite.h"
#include "animation.h"
#include "sprite_sheet.h"

struct _ev_sprite
{
		ev_vec2  position;
		float    rotation;
		float    scale;
		ev_anim *animation;
		int      lua_ref;
};

ev_sprite* ev_sprite_create(void)
{
		ev_sprite *s = ev_malloc(sizeof(ev_sprite));
		if( s ) {
				memset(s, 0, sizeof(ev_sprite));
				s->scale = 1.0f;
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

void ev_sprite_fill(ev_sprite* s, ev_bvertex* b)
{
		int i;
		ev_bvertex *src;

		if( s && b ) {
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
		}
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

		ev_log("sprite create");

		lua_newtable(l);
		luaL_getmetatable(l, EV_SPRITE_META);
		lua_setmetatable(l, -2);

		s = lua_newuserdata(l, sizeof(ev_sprite));
		memset(s, 0, sizeof(ev_sprite));
		lua_setfield(l, -2, EV_SPRITE_KEY);
		s->lua_ref = ev_lua_create_ref(l, 1);

		return 1;
}

int l_sprite_destroy(lua_State *l)
{
		ev_sprite *s;

		ev_log("sprite destroy");

		s = check_sprite(l);

		return 0;
}

static int l_sprite_set_animation(lua_State *l)
{
		ev_sprite *s;
		ev_anim   *a;
		ev_log("sprite set animation");

		s = check_sprite(l);

		a = ev_anim_from_lua(l, 2);

		ev_sprite_set_animation( s, a);

		return 0;
}

static int l_sprite_set_rotation(lua_State *l)
{
		ev_sprite *s;
		float rot;

		s = check_sprite(l);
		rot = (float)lua_tonumber(l, 2);
		ev_log("set_rotation: %.2f", rot );

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

		ev_log("sprite set_position [%.2f,%.2f]", x, y);

		s->position.x = x;
		s->position.y = y;

		return 0;
}
ev_err_t ev_sprite_lua_init(lua_State *l)
{
		luaL_Reg luaFuncs[] = {
				{ "create", l_sprite_create },
				{ "__gc",   l_sprite_destroy },
				{ "set_animation", l_sprite_set_animation },
				{ "set_rotation", l_sprite_set_rotation },
				{ "set_position", l_sprite_set_position },
				{ 0, 0 }
		};

		ev_lua_init_module(l, luaFuncs, EV_SPRITE_META, "sprite");

		return EV_OK;
}
