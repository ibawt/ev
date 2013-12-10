#include <cstdio>
#include <string>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include "GL/glu.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#endif



namespace evil {

static const char* lastError = "";

const char *GetError() {
		return lastError;
}

void SetError(const char *error) {
		lastError = error;
}

void SetError( const unsigned char* error) {
		lastError = (const char*)error;
}

void error(const char *fmt, ... )
{
		char buffer[512];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);

		fprintf(stderr, "%s\n", buffer); 
}


static SDL_Window *window = nullptr;
static SDL_GLContext context = nullptr;

static bool initGL()
{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		GLenum error = glGetError();

		if( error != GL_NO_ERROR ) {
				SetError( gluErrorString(error));
				return false;
		}

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		error = glGetError();
		if( error != GL_NO_ERROR ) {
				SetError( gluErrorString(error));
				return false;
		}

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

		error = glGetError();
		if( error != GL_NO_ERROR ) {
				SetError( gluErrorString(error));
				return false;
		}
		return true;
}

static bool init()
{
		if( SDL_Init(SDL_INIT_EVERYTHING ) < 0 ) {
				error("SDL init failed: %s", SDL_GetError());
				return false;
		}

		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);

		window = SDL_CreateWindow( "Evil", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
															 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

		if( !window ) {
				error( "Couldn't create Window: %s", SDL_GetError());
				return false;
		}

		context = SDL_GL_CreateContext( window );
		if( !context ) {
				error("Couldnt' create GL context: %s", SDL_GetError());
				return false;
		}


		if( SDL_GL_SetSwapInterval(1) < 0 ) {
				error("Couldn't set VSYNC: %s", SDL_GetError());
				return false;
		}

		if( !initGL()) {
				error("Couldn't initialize OpenGL: %s", GetError());
				return false;
		}
		return true;
}

static void render() {
		glClear(GL_COLOR_BUFFER_BIT);
}

static void close() {
		if( context ) {
				SDL_GL_DeleteContext( context );
				context = nullptr;
		}
		if( window ) {
				SDL_DestroyWindow( window );
				window = nullptr;
		}
		SDL_Quit();
}

}
using namespace std;
using namespace evil;

int main(int argc, char** argv)
{
		if( !init() ) {
				return 1;
		}
		
		SDL_StartTextInput();

		bool running = true;
		
		while(running) {
				SDL_Event e;
				while( SDL_PollEvent(&e) ) {
						switch( e.type ) {
						case SDL_QUIT:
								running = false;
								break;
						case SDL_TEXTINPUT:
								// do stuff
								break;
						}
				}
				render();
				SDL_GL_SwapWindow( window );
		}
		
		SDL_StopTextInput();

		close();
		
		return 0;
}

