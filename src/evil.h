#ifndef EVIL_H_
#define EVIL_H_

#include <math.h>
#define NO_SDL_GLEXT
#include "GL/glew.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

#ifdef  __APPLE__
#include <OpenGL/glu.h>
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#include "GL/glu.h"
#endif

void *ev_malloc(size_t);
void  ev_free(void *);

typedef enum {
  EV_OK = 0,
  EV_FAIL,
  EV_NOMEM
} ev_err_t;

void ev_log(const char *fmt, ...);
void ev_error(const char *fmt, ...);

typedef struct {
  float w;
  float h;
} ev_size;

typedef struct {
    float x,y;
} ev_vec2;

typedef struct {
  ev_vec2 origin;
  ev_size size;
} ev_rect;

typedef struct {
    void *opaque;
} ev_key_event;

typedef struct {
    void *opaque;
} ev_mouse_event;


typedef struct {
    float x;
    float y;
    float u;
    float v;
    float rotation;
    float scale;
    float tx;
    float ty;
} ev_bvertex;

typedef struct _ev_sbatch ev_sbatch;
typedef struct _ev_texture ev_texture;
typedef struct _ev_anim ev_anim;
typedef struct _ev_program ev_program;
typedef struct _ev_matrix4 ev_matrix4;
typedef struct _ev_sprite  ev_sprite;
typedef struct _ev_vbuff   ev_vbuff;
typedef struct _ev_shader ev_shader;

#endif
