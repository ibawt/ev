#include "application.h"
#include <SDL_image.h>

namespace evil {

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
		IMG_Quit();
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
				//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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

		int flags = IMG_INIT_JPG | IMG_INIT_PNG;
		if( IMG_Init(flags) != flags ) {
				error("couldn't init image library: %s", IMG_GetError());
				return false;
		}

		return true;
}

bool Application::initGL()
{
		GLenum error = GL_NO_ERROR;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 1, -1, 1 );
		CHECK_GL();

		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		CHECK_GL();

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
		CHECK_GL();

		glViewport( 0, 0, width, height );

		error = glewInit();
		if(error != GLEW_OK ) {
				evil::error("error initializing glew: %d", glewGetErrorString(error));
				return false;
		}

		log("using GLEW: %s", glewGetString(GLEW_VERSION));

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
		float dt = 0.0f;

		uint32_t startTime = SDL_GetTicks();
		uint64_t numFrames = 0;

		while( running ) {
				uint32_t t1 = SDL_GetTicks();

				SDL_Event e;
				while( SDL_PollEvent(&e) ) {
						switch( e.type ) {
						case SDL_QUIT:
								running = false;
								break;
						}
				}
				update(dt);
				render();
				SDL_GL_SwapWindow(window);

				dt = ( SDL_GetTicks() - t1 ) / 1000.0f;

				numFrames++;

				if( (numFrames % 10 ) == 0 ) {
						fps = numFrames / (( SDL_GetTicks() - startTime) / 1000.0f);
						log("fps: %.2f", fps);
				}
		}
		SDL_StopTextInput();
		quit();
		return 0;
}
}
