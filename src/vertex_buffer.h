#ifndef EV_VERTEX_BUFFER_H_
#define EV_VERTEX_BUFFER_H_

#include "sprite_sheet.h"

typedef struct _ev_vbuff ev_vbuff;
ev_vbuff* ev_vbuff_create(void);
void      ev_vbuff_destroy(ev_vbuff*);
void      ev_vbuff_set_capacity(ev_vbuff*, size_t);
void*     ev_vbuff_get_buffer(ev_vbuff*);
void      ev_vbuff_bind(ev_vbuff*);

#endif
