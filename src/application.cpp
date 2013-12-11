#include "application.h"

namespace evil {

#define CHECK_GL() if( (error = glGetError() ) != GL_NO_ERROR ) { SetError((const char*)gluErrorString(error)); return false; }

Application::Application(uint32_t w, uint32_t h) :
    width(w), height(h), window(nullptr)
{
}

void Application::quit()
{
    if( window ) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

bool Application::initSDL()
{
    if( window )
        return false;

    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        error("SDL init failed: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);

    window = SDL_CreateWindow( "Evil", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if( !window ) {
        error("Couldn't create window: %s", SDL_GetError());
        return false;
    }

    context = SDL_GL_CreateContext(window);
    if( !context ) {
        error("Couldn't create GL context: %s", SDL_GetError());
        return false;
    }

    if( SDL_GL_SetSwapInterval(1) < 0 ) {
        error("Couldn't set VSYNC: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool Application::initGL()
{
    GLenum error = GL_NO_ERROR;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    CHECK_GL();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    CHECK_GL();

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
    CHECK_GL();
    return true;
}

int Application::main()
{
    if( !initSDL() ) {
        return 1;
    }

    if( !initGL() ) {
        return 1;
    }

    if( !init() ) {
        return 1;
    }
    
    SDL_StartTextInput();
    bool running = true;

    while( running ) {
        SDL_Event e;
        while( SDL_PollEvent(&e) ) {
            switch( e.type ) {
            case SDL_QUIT:
                running = false;
                break;
            }
        }
        render();
        SDL_GL_SwapWindow(window);
    }
    SDL_StopTextInput();
    quit();
    return 0;
}
}
