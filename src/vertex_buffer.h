#ifndef EV_VERTEX_BUFFER_H_
#define EV_VERTEX_BUFFER_H_

#include <stdlib.h>

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

ev_vbuff* ev_vbuff_create(void);
void      ev_vbuff_destroy(ev_vbuff*);
ev_err_t  ev_vbuff_set_capacity(ev_vbuff*, size_t);
void*     ev_vbuff_get_buffer(ev_vbuff*);
void      ev_vbuff_bind(ev_vbuff*);
void*     ev_vbuff_map(ev_vbuff*);
void      ev_vbuff_unmap(ev_vbuff*);
size_t    ev_vbuff_get_capacity(ev_vbuff*);

#ifdef __cplusplus
}
#endif

#endif
