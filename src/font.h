#ifndef EV_FONT_H_
#define EV_FONT_H_

#include "evil.h"
#include "matrix4.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ev_font ev_font;

EV_API ev_font* ev_font_create(const char *name, float pt_size);
EV_API void     ev_font_destroy(ev_font*);
EV_API float    ev_font_set_text(ev_font *font, const char *text, int len);
EV_API void     ev_font_render(ev_font *, ev_matrix4 *);
EV_API void     ev_font_set_position(ev_font *, ev_vec2 pos);
EV_API ev_vec2  ev_font_get_position(ev_font *);
EV_API void     ev_font_set_colour(ev_font *font, float r, float g, float b, float a);

#ifdef __cplusplus
}
#endif

#endif
