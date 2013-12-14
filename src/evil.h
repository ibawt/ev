#ifndef EVIL_H_
#define EVIL_H_

#include <math.h>

#include "SDL_opengl.h"
#include "SDL_image.h"

#ifdef  __APPLE__
#include <OpenGL/glu.h>
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#include "GL/glu.h"
#endif

#include "animation.h"
#include "sprite.h"
#include "sprite_batch.h"
#include "texture.h"
#include "utils.h"

namespace evil {

void log(const char *fmt, ...);
void error(const char *fmt, ... );
void SetError(const char *);

}   

#endif
