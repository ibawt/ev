#include "evil.h"
#include "vertex_buffer.h"

struct _ev_vbuff {
    GLuint id;
    void  *buff;
    size_t size;
};

ev_vbuff* ev_vbuff_create(void)
{
    ev_vbuff* p = ev_malloc(sizeof(ev_vbuff));
    if( p ) {
        memset(p, 0, sizeof(ev_vbuff));
    }
    return p;
}

void ev_vbuff_destroy(ev_vbuff* v)
{
    if( v ) {
        ev_free(v);
    }
}
