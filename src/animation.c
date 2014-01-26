#include <assert.h>
#include "ev_lua.h"
#include "animation.h"

#define FRAME_GROW_FAC 16

struct _ev_anim
{
		float        time;
		ev_anim_mode mode;
		uint32_t     index;
		float        delay;
		ev_sframe  **frames;
		uint32_t     frame_cnt;
		size_t       frame_size;
		int          lua_ref;
};

static void grow_array(ev_anim *a)
{
		void *p;

		assert( a != NULL );

		p = a->frames;
		a->frames = ev_malloc(sizeof(ev_sframe*)*(a->frame_size + FRAME_GROW_FAC));
		a->frame_size += FRAME_GROW_FAC;
		memset(a->frames, 0, sizeof(ev_sframe*)*a->frame_size);
		memcpy(a->frames, p, sizeof(ev_sframe*)*a->frame_cnt);

		ev_free(p);
}

static void init_ev_anim(ev_anim* a)
{
		assert( a != NULL );

		a->time = 0.0f;
		a->mode = EV_LOOP;
		a->frames = ev_malloc(sizeof(ev_sframe*)*FRAME_GROW_FAC);
		memset(a->frames, 0, sizeof(ev_sframe*)*FRAME_GROW_FAC);
		a->frame_cnt = 0;
		a->delay = 0.3f;
		a->frame_size = FRAME_GROW_FAC;
		a->index = 0;
}

ev_anim* ev_anim_create(void)
{
		ev_anim * a = ev_malloc(sizeof(ev_anim));
		if( !a )
				return NULL;

		init_ev_anim(a);

		return a;
}

void ev_anim_destroy(ev_anim *a)
{
		if( a ) {
				ev_free(a->frames);
				a->frames = NULL;
				ev_free(a);
		}
}

void ev_anim_update(ev_anim *a, float dt)
{
		if( a && a->delay > 0.0f ) {
				a->time += dt;

				while( a->time >= a->delay ) {
						a->time -= a->delay;
						switch( a->mode ) {
						case EV_LOOP:
								a->index++;
								if( a->index >= a->frame_cnt ) {
										a->index = 0;
								}
								break;
						case EV_ONE_SHOT:
								a->index++;
								if( a->index >= a->frame_cnt ) {
										a->index = a->frame_cnt - 1;
								}
								break;
						case EV_REVERSE:
								if( a->index > 0 ) {
										a->index--;
								}
								break;
						case EV_PING_PONG:
								break;
						}
				}
		}
}

void ev_anim_add_sframe(ev_anim* a, ev_sframe *s)
{
		if( a && s ) {
				if( a->frame_cnt >= a->frame_size ) {
						grow_array(a);
				}

				a->frames[a->frame_cnt++] = s;
		}
}

ev_sframe* ev_anim_get_current_sframe(ev_anim *a)
{
		assert( a->frames[a->index] != NULL );

		return a ? a->frames[a->index] : NULL;
}

void ev_anim_set_mode(ev_anim *a, ev_anim_mode m)
{
		if( a ) {
				a->mode = m;
		}
}
ev_anim_mode ev_anim_get_mode(ev_anim *a)
{
		if( a ) {
				return a->mode;
		}
		return EV_LOOP;
}

#define EV_ANIM_KEY "__ev_anim"
#define EV_ANIM_META "__ev_anim_meta"

static ev_anim* check_anim(lua_State *l)
{
		ev_anim *a;

		luaL_checktype(l, 1, LUA_TTABLE);
		lua_getfield(l, 1, EV_ANIM_KEY );
		a = lua_touserdata(l, -1 );

		luaL_argcheck( l, a != NULL, 1, "ev_anim expected");

		return a;
}

static int l_anim_add_frame(lua_State *l)
{
		ev_anim *a;
		ev_sframe *s;

		a = check_anim(l);
		if( !lua_islightuserdata(l,2)) {
				lua_pushstring(l, "it's not a sframe!");
				lua_error(l);
		} else {
				s = lua_touserdata(l, 2);

				luaL_argcheck(l, s != NULL, 1, "frame expected");

				ev_anim_add_sframe(a, s);
		}

		return 0;
}

static int l_anim_create(lua_State *l)
{
		ev_anim *a;

		ev_log("anim_create");
		lua_newtable(l);
		luaL_getmetatable( l, EV_ANIM_META);
		lua_setmetatable(l, -2);

		a = lua_newuserdata(l, sizeof(ev_anim));
		init_ev_anim(a);

		lua_setfield(l, -2, EV_ANIM_KEY );
		a->lua_ref = ev_lua_create_ref(l, 1);

		return 1;
}

static int l_anim_destroy(lua_State *l)
{
		ev_anim *a;

		ev_log("anim_destroy");

		a = check_anim(l);

		if( a->frames ) {
				ev_free(a->frames);
		}

		return 0;
}

ev_err_t ev_anim_lua_init(lua_State *l)
{
		luaL_Reg anim_lua_funcs[] = {
				{ "create",  l_anim_create },
				{ "__gc",    l_anim_destroy},
				{ "add_frame", l_anim_add_frame},
				{ 0, 0 }
		};

		ev_lua_init_module(l, anim_lua_funcs, EV_ANIM_META, "anim");
		return EV_OK;
}
