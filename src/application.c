#include <stdio.h>
#include <assert.h>

#include "evil.h"
#include "application.h"

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
    if( width == 0 || height == 0 )
        return NULL;

    ev_app *app = ev_malloc( sizeof(ev_app));
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
                running = false;
                break;
            }
        }
        if( app->update ) {
            app->update(app, dt);
        }
        if( app->render ) {
            app->render(app);
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
