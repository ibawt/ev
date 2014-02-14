#include <assert.h>

#include "evil.h"
#include "vertex_buffer.h"

struct _ev_vbuff {
    GLuint id;
    void  *buff;
    size_t size;
};

static void dispose(ev_vbuff *v)
{
    assert( v != NULL );

    if( v->id ) {
        glDeleteBuffers(1, &v->id);
        v->id = 0;
    }

    if( v->buff ) {
        ev_free(v->buff);
        v->buff = NULL;
    }
    v->size = 0;
}

ev_vbuff* ev_vbuff_create(void)
{
    ev_vbuff* p = ev_malloc(sizeof(ev_vbuff));
    if( p ) {
        memset(p, 0, sizeof(ev_vbuff));
    }
    return p;
}

size_t ev_vbuff_get_capacity(ev_vbuff *v)
{
    if( v ) {
        return v->size;
    }
    return 0;
}

ev_err_t ev_vbuff_set_capacity(ev_vbuff *v, size_t num_bytes)
{
    if( v && num_bytes ) {
        if( v->size < num_bytes ) {
            dispose(v);
        }

        glGenBuffers(1, &v->id);
        if( !v->id )
            return EV_FAIL;

        ev_vbuff_bind(v);

        v->buff = ev_malloc(num_bytes);
        if(!v->buff)
            return EV_NOMEM;

        memset(v->buff, 0, num_bytes);
        glBufferData( GL_ARRAY_BUFFER, num_bytes, v->buff, GL_DYNAMIC_DRAW);
        v->size = num_bytes;
        return EV_OK;
    }
    return EV_FAIL;
}

void ev_vbuff_bind(ev_vbuff *v)
{
    if( v && v->id ) {
        glBindBuffer(GL_ARRAY_BUFFER, v->id);
    }
}

void ev_vbuff_destroy(ev_vbuff* v)
{
    if( v ) {
        dispose(v);
        ev_free(v);
    }
}

void *ev_vbuff_map(ev_vbuff* v)
{
    if( v ) {
        ev_vbuff_bind(v);
        return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }
    return NULL;
}

void ev_vbuff_unmap(ev_vbuff* v)
{
    if( v ) {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}
