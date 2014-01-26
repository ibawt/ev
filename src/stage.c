#include "stage.h"
#include "sprite_batch.h"

typedef struct {
		void           *opaque;
		ev_stage_render_fn render_fn;
		ev_stage_update_fn update_fn;
} node;

struct _ev_stage {
		node* nodes;
		size_t node_len;
		size_t node_cnt;
		int    lua_ref;
};

static void stage_init(ev_stage *stage)
{
		memset(stage, 0, sizeof(ev_stage));

		stage->nodes = ev_malloc(sizeof(node)*16);
		stage->node_len = 16;
		stage->node_cnt = 0;
}

ev_stage* ev_stage_create(void)
{
		ev_stage* stage = ev_malloc(sizeof(ev_stage));

		stage_init(stage);

		return stage;
}

void ev_stage_free(ev_stage *s)
{
		if( s ) {
				if( s->nodes ) {
						ev_free(s->nodes);
				}
				ev_free(s);
		}
}

void ev_stage_render(ev_stage *s)
{
		int i;

		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for( i = s->node_cnt - 1 ; i >= 0 ; --i ) {
				s->nodes[i].render_fn( s->nodes[i].opaque);
		}
}

void ev_stage_add_actor(ev_stage *stage, ev_stage_render_fn render_fn, ev_stage_update_fn update_fn, void *d)
{
		node *n;

		if( stage->node_cnt == stage->node_len ) {
				stage->node_len += 16;
				stage->nodes = ev_realloc( stage->nodes, (stage->node_len)*sizeof(node));
		}

		n = stage->nodes + stage->node_cnt;

		n->opaque = d;
		n->render_fn = render_fn;
		n->update_fn = update_fn;

		stage->node_cnt++;
}

void ev_stage_update(ev_stage *stage, float dt)
{
		int i;

		for( i = 0 ; i < stage->node_cnt ; ++i ) {
				stage->nodes[i].update_fn(stage->nodes[i].opaque, dt);
		}
}


#define EV_STAGE_KEY "__ev_stage"
#define EV_STAGE_META "__ev_stage_meta"

ev_stage* ev_stage_from_lua(lua_State *l, int arg)
{
		ev_stage *s;

		luaL_checktype(l, arg, LUA_TTABLE);
		lua_getfield(l, arg, EV_STAGE_KEY);

		s = lua_touserdata(l, -1);

		luaL_argcheck(l, s != NULL, 1, "ev_stage expected");

		return s;
}

static int l_stage_create(lua_State *l)
{
		ev_stage *s;

		ev_log("stage create");

		lua_newtable(l);
		luaL_getmetatable(l, EV_STAGE_META);
		lua_setmetatable(l, -2);

		s = lua_newuserdata(l, sizeof(ev_stage));
		stage_init(s);

		lua_setfield(l, -2, EV_STAGE_KEY);
		s->lua_ref = ev_lua_create_ref(l, 1);

		return 1;
}

static int l_stage_destroy(lua_State *l)
{
		ev_stage *s;

		s = ev_stage_from_lua(l, 1);

		if( s->nodes ) {
				ev_free(s->nodes);
		}

		return 0;
}

static int l_stage_add_sbatch(lua_State *l)
{
		ev_stage  *stage;
		ev_sbatch *batch;

		stage = ev_stage_from_lua(l, 1);
		batch = ev_sbatch_from_lua(l, 2);

		ev_stage_add_actor(stage, (ev_stage_render_fn)ev_sbatch_render, (ev_stage_update_fn)ev_sbatch_update, batch);

		return 0;
}

ev_err_t ev_stage_lua_init(lua_State *l)
{
		luaL_Reg funcs[] = {
				{ "create", l_stage_create },
				{ "__gc", l_stage_destroy },
				{ "add_sbatch", l_stage_add_sbatch },
				{ 0, 0 }
		};

		ev_lua_init_module(l, funcs, EV_STAGE_META, "stage");

		return EV_OK;
}
