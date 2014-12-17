#ifndef EV_SPRITE_SHEET_H_
#define EV_SPRITE_SHEET_H_

#include "evil.h"
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ev_sframe
{
    const char *key;
    ev_size     source_size;
    ev_size     size;
    int         trimmed;
    ev_rect     texture_rect;
    ev_vec2     offset;
    ev_bool     rotated;
    ev_rect     color_rect;
    ev_bvertex  batch_verts[EV_SPRITE_NUM_VERTS];
} ev_sframe;

typedef struct _ev_ssheet
{
    ev_smap *frames;
    ev_smap *metadata;
} ev_ssheet;

EV_API ev_sframe* ev_sframe_create_quad(float w, float h, float left, float top,float right,float bottom);
EV_API ev_sframe*  ev_sframe_create(const char *);
EV_API void        ev_sframe_destroy(ev_sframe *);
EV_API ev_bvertex* ev_sframe_get_bvertex(ev_sframe*);

EV_API ev_ssheet* ev_ssheet_create(void);
EV_API ev_err_t   ev_ssheet_load_file(ev_ssheet *, const char *);
EV_API void       ev_ssheet_destroy(ev_ssheet *);
EV_API ev_sframe* ev_ssheet_get_sframe(ev_ssheet *, const char *);
EV_API ev_size ev_sframe_get_size(ev_sframe *s);

#ifdef __cplusplus
}
#endif

#endif
