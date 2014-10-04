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

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#define strdup _strdup
#define snprintf _snprintf
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ev_cmpf(x,y) ( fabs((x) - (y)) < 0.1f)

#ifndef NDEBUG
#define CHECK_GL() do { GLint err = glGetError(); if( err ) { ev_log("ERROR in gl: %d", err);  } } while(0)
#else
#define CHECK_GL()
#endif

#define UNUSED(x) (void)(x)

#ifdef WIN32
#define EV_API __declspec(dllexport)
#else
#define EV_API __attribute__((visibility ("default")))
#endif

EV_API void *ev_malloc(size_t);
EV_API void *ev_realloc(void *, size_t);
EV_API void  ev_free(void *);
EV_API char *ev_strdup(const char*);

typedef struct {
    GLenum src;
    GLenum dst;
} ev_blend_func;

typedef enum {
    EV_OK = 0,
    EV_FAIL,
    EV_NOMEM
} ev_err_t;

typedef enum {
    EV_LOG   = 0,
    EV_WARN  = 1,
    EV_ERROR = 2,
    EV_DEBUG = 3
} ev_log_level;

typedef enum {
    EV_FALSE = 0,
    EV_TRUE
} ev_bool;

#define ev_log(fmt, ...) ev_logger(EV_LOG, "[LOG]%s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

EV_API void ev_error(const char *fmt, ...);

EV_API void ev_logger(ev_log_level, const char *fmt, ...);

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
    float opacity;
} ev_bvertex;

typedef struct _ev_sbatch ev_sbatch;
typedef struct _ev_program ev_program;
typedef struct _ev_vbuff   ev_vbuff;
typedef struct _ev_shader ev_shader;
typedef struct _ev_sframe ev_sframe;
typedef struct _ev_ssheet ev_ssheet;
typedef struct _ev_tilemap ev_tilemap;

/* physics */
typedef struct ev_world ev_world;
typedef struct ev_body  ev_body;
typedef struct ev_particle_group ev_particle_group;
typedef struct ev_particle_system ev_particle_system;

#ifdef __cplusplus
}
#endif

#endif
