#ifndef RENDER_TEXTURE_H_
#define RENDER_TEXTURE_H_

#include "evil.h"
#include "texture.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    GLuint fb_id;
    ev_texture texture;
    GLfloat viewport[4];
} ev_rtex;

EV_API ev_rtex* ev_rtex_create(int width, int height);
EV_API void     ev_rtex_destroy(ev_rtex*);
EV_API void     ev_rtex_bind(ev_rtex*);

#ifdef __cplusplus
}
#endif

#endif
