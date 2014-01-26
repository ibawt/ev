#include <stdio.h>
#include <assert.h>

#include "ev_lua.h"
#include "stage.h"
#include "evil.h"
#include "application.h"

static void l_app_render(ev_app *app);

struct _ev_app {
		uint32_t      width;
		uint32_t      height;
		SDL_Window   *window;
		float         fps;
		SDL_GLContext context;
		int           lua_ref;
		ev_app_render      render;
		ev_app_update      update;
		ev_app_key_event   key_event;
		ev_app_mouse_event mouse_event;

		ev_stage *stage;
};

static int initGL(ev_app *app)
{
		GLenum err = GL_NO_ERROR;

		assert(app != NULL);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glViewport(0, 0, app->width, app->height);

		err = glewInit();
		if( err != GLEW_OK ) {
				ev_error("error initializing glew: %s", glewGetErrorString(err));
				return EV_FAIL;
		}
		ev_log("Using OpenGL: %s", glGetString(GL_VERSION));
		ev_log("Using GLEW: %s", glewGetString(GLEW_VERSION));

		return EV_OK;
}

static int initSDL(ev_app *app)
{
		int flags = IMG_INIT_JPG | IMG_INIT_PNG;

		assert( app != NULL );

		if( app->window ) {
				return EV_FAIL;
		}
		if( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
				ev_error("SDL init failed: %s", SDL_GetError());
				return EV_FAIL;
		}

		app->window = SDL_CreateWindow("Evil", SDL_WINDOWPOS_UNDEFINED,
																	 SDL_WINDOWPOS_UNDEFINED,
																	 app->width, app->height,
																	 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if( !app->window ) {
				ev_error("Couldn't create window: %s", SDL_GetError());
				return EV_FAIL;
		}

		app->context = SDL_GL_CreateContext(app->window);
		if( !app->context ) {
				ev_error("Couldn't create GL context: %s", SDL_GetError());
				return EV_FAIL;
		}

		if( SDL_GL_SetSwapInterval(1) < 0 ) {
				ev_error("Couldn't set vsync: %s",  SDL_GetError());
		}

		if( IMG_Init(flags) != flags ) {
				ev_error("Couldn't init image library: %s", IMG_GetError());
				return EV_FAIL;
		}
		return EV_OK;
}

ev_err_t ev_app_init(ev_app *app)
{
		if( app ) {
				if( initSDL(app)  )
						return EV_FAIL;
				if( initGL(app) )
						return EV_FAIL;
				return EV_OK;
		}
		return EV_FAIL;
}

uint32_t ev_app_get_height(ev_app *app)
{
		return app ? app->height : 0;
}

uint32_t ev_app_get_width(ev_app *app)
{
		return app ? app->width : 0;
}

float ev_app_get_fps(ev_app *app)
{
		return app ? app->fps : 0.0f;
}

void ev_app_set_render(ev_app *app, ev_app_render fn)
{
		if( app ) {
				app->render = fn;
		}
}

void ev_app_set_update(ev_app *app, ev_app_update fn)
{
		if( app ) {
				app->update = fn;
		}
}

void ev_app_set_key_event(ev_app *app, ev_app_key_event fn)
{
		if( app ) {
				app->key_event = fn;
		}
}

void ev_app_set_mouse_event(ev_app *app, ev_app_mouse_event fn)
{
		if( app ) {
				app->mouse_event = fn;
		}
}

ev_app* ev_app_create(uint32_t width, uint32_t height)
{
		ev_app *app;
		if( width == 0 || height == 0 )
				return NULL;

		app = ev_malloc( sizeof(ev_app));
		if( !app )
				return NULL;

		memset(app, 0, sizeof(ev_app));
		app->width = width;
		app->height = height;

		return app;
}
void ev_app_quit(ev_app *app)
{
		if( app ) {
				if( app->window ) {
						SDL_DestroyWindow(app->window);
						app->window = NULL;
				}
				IMG_Quit();
				SDL_Quit();
		}
}

void ev_app_destroy(ev_app *app)
{
		ev_log("app_destroy!!");
		if( app ) {
				ev_app_quit( app );
				ev_free( app );
		}
}

ev_err_t ev_app_start(ev_app *app)
{
		int running = 1;
		float dt = 0.0f;
		uint32_t startTime;
		uint64_t numFrames = 0;

		SDL_StartTextInput();

		startTime = SDL_GetTicks();
		while( running ) {
				uint32_t t1 = SDL_GetTicks();
				SDL_Event e;

				while( SDL_PollEvent(&e)) {
						switch(e.type) {
						case SDL_QUIT:
								running = 0;
								break;
						}
				}
				if( app->update ) {
						app->update(app, dt);
				}
				if( app->render ) {
						app->render(app);
				}

				if( app->stage ) {
						ev_stage_update(app->stage, dt);
						ev_stage_render(app->stage);
				}

				SDL_GL_SwapWindow(app->window);

				dt = ( SDL_GetTicks() - t1 ) / 1000.0f;

				numFrames++;

				if( (numFrames % 10 ) == 0 ) {
						app->fps = numFrames / (( SDL_GetTicks() - startTime) / 1000.0f);
						ev_log("fps: %.2f", app->fps);
				}
		}
		return EV_OK;
}

#define EV_APP_KEY "__ev_app"
#define EV_APP_META "__ev_app_meta"

static void l_app_render(ev_app *app)
{
		lua_State *l = ev_lua_get_state();
		lua_gettable(l, LUA_REGISTRYINDEX);
		lua_rawgeti(l, -2, app->lua_ref);
		lua_rawgeti(l, 1, 1);
		lua_getfield(l, 1, "render");
		lua_call(l, 0, 0);

		lua_pop(l, 2);
}

static ev_app* get_app(lua_State *l)
{
		ev_app *app;
		assert( l != NULL );

		luaL_checktype(l, 1, LUA_TTABLE);
		lua_getfield( l, 1, EV_APP_KEY);

		app = lua_touserdata(l,-1);

		luaL_argcheck(l, app != NULL, 1, "ev_app expected");
		assert( app != NULL );

		return app;
}

static int app_create(lua_State *l)
{
		ev_app *app;

		ev_log("app_create");

		assert( l != NULL );

		lua_newtable(l);

		luaL_getmetatable(l, EV_APP_META);
		lua_setmetatable(l, -2);

		app = lua_newuserdata( l, sizeof(ev_app));

		memset(app, 0, sizeof(ev_app));

		lua_setfield( l,-2, EV_APP_KEY);

		app->lua_ref = ev_lua_create_ref( l, 1 );

		return 1;
}

static int app_destroy(lua_State *l)
{
		ev_app *app;

		ev_log("app_destroy");

		assert( l != NULL );

		app = get_app(l);

		ev_app_quit(app);

		return 0;
}

static int app_set_dimensions(lua_State *l)
{
		int n;
		ev_app *app;
		lua_Number width;
		lua_Number height;

		assert( l != NULL );

		ev_log("app_set_dimensions");

		n = lua_gettop(l);
		app = get_app(l);

		if( n < 3 ) {
				lua_pushstring(l, "Incorrect number of arguments");
				lua_error(l);
				return 0;
		}

		width = lua_tonumber(l, 2);
		height = lua_tonumber(l, 3);

		app->width = (int)width;
		app->height = (int)height;

		return 0;
}


static int app_init(lua_State *l)
{
		ev_app *app;

		ev_log("app_init");

		assert( l != NULL );

		app = get_app(l);

		ev_app_init(app);

		return 0;
}

static int app_show(lua_State *l)
{
		ev_app *app;

		ev_log("app_show");

		assert( l != NULL );

		app = get_app(l);

		ev_app_start(app);

		return 0;
}

static int app_quit(lua_State *l)
{
		ev_app *app;

		ev_log("app_quit");
		assert( l != NULL );

		app = get_app(l);

		ev_app_quit(app);

		return 0;
}

static int app_set_stage(lua_State *l)
{
		ev_app   *app;
		ev_stage *stage;

		app = get_app(l);

		stage = ev_stage_from_lua(l, 2);

		app->stage = stage;

		return 0;
}

static const luaL_Reg appMethods[] = {
		{ "create", app_create},
		{ "__gc", app_destroy },
		{ "set_dimensions", app_set_dimensions },
		{ "init", app_init },
		{ "show", app_show },
		{ "quit", app_quit },
		{ "set_stage", app_set_stage },
		{ 0, 0 }
};

ev_err_t ev_application_lua_init(lua_State *l)
{
		assert( l != NULL );

		luaL_newmetatable(l, EV_APP_META);
		luaL_setfuncs( l, appMethods,0);
		lua_pushvalue(l, -1);
		lua_setfield(l, -1, "__index");

		lua_getglobal(l, "ev");
		lua_pushvalue(l, -2);
		lua_setfield(l, -2, "app");

		return EV_OK;
}
