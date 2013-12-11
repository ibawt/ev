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

#include "evil.h"

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
void log(const char *fmt, ... )
{
		char buffer[512];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);

		fprintf(stdout, "%s\n", buffer); 
}

}
