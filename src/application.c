#include <stdio.h>
#include <assert.h>

#include "ev_lua.h"
#include "stage.h"
#include "evil.h"
#include "application.h"
#include "ev_box2d.h"

#ifdef TRACE_APP
#define LOG(...) ev_log(##__VA_ARGS__)
#else
#define LOG(...)
#endif

static void l_app_update(ev_app *app, float dt);

static ev_app *lua_app = NULL;

ev_app* ev_app_get_lua_instance(void)
{
    return lua_app;
}

struct _ev_app {
    uint32_t      width;
    uint32_t      height;
    SDL_Window   *window;
    float         fps;
    SDL_GLContext context;

    ev_app_render      render;
    ev_app_update      update;
    ev_app_key_event   key_event;
    ev_app_mouse_event mouse_event;

    ev_stage *stage;

    ev_app_state state;

    ev_world *world;

    ev_bool done;

    char key_states[256];
};

ev_app_state ev_app_get_state(ev_app *a)
{
    if( a ) {
        return a->state;
    }
    return EV_APP_INVALID;
}

ev_world* ev_app_get_world()
{
    ev_app *app = ev_app_get_lua_instance();
    return app ? app->world : NULL;
}

static int initGL(ev_app *app)
{
    GLenum err = GL_NO_ERROR;

    assert(app != NULL);

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, app->width, app->height);

    err = glewInit();
    if( err != GLEW_OK ) {
        ev_error("error initializing glew: %s", glewGetErrorString(err));
        return EV_FAIL;
    }
    LOG("Using OpenGL: %s", glGetString(GL_VERSION));
    LOG("Using GLEW: %s", glewGetString(GLEW_VERSION));

    return EV_OK;
}

static ev_err_t initSDL(ev_app *app)
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

    app->window = SDL_CreateWindow("ev", SDL_WINDOWPOS_UNDEFINED,
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

        app->world = ev_world_create();
        if( !app->world )
            return EV_FAIL;

        ev_world_set_dimensions(app->world, app->width, app->height);

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

        if( app->world ) {
            ev_world_destroy(app->world);
            app->world = NULL;
        }

        IMG_Quit();
        SDL_Quit();
    }
}

void ev_app_destroy(ev_app *app)
{
    LOG("app_destroy!!");
    if( app ) {
        ev_app_quit( app );
        ev_free( app );
    }
}

int ev_app_get_ticks(ev_app *app)
{
    return SDL_GetTicks();
}

void ev_app_swap_buffers(ev_app *app)
{
    lua_State *l = ev_lua_get_state();

    if( app && app->window ) {
        SDL_GL_SwapWindow(app->window);
    }

    if( l ) {
        lua_gc(l, LUA_GCSTEP, 1 );
    }
}

ev_bool ev_app_poll_event(ev_app *app, ev_event *event)
{
    SDL_Event e;

    if( SDL_PollEvent(&e) ) {
        switch( e.type ) {
        case SDL_KEYDOWN:
            event->type = EV_KEYDOWN;
            event->key = SDL_GetKeyName(e.key.keysym.sym);
            break;
        case SDL_KEYUP:
            event->type = EV_KEYUP;
            event->key = SDL_GetKeyName(e.key.keysym.sym);
            break;
        case SDL_QUIT:
            event->type = EV_QUIT;
            break;
        }
        return EV_TRUE;
    }
    return EV_FALSE;
}

void ev_app_set_stage(ev_app *app, ev_stage *s)
{
    if( app ) {
        app->stage = s;
    }
}
