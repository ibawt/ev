#include "graphics.h"
#include <string.h>

struct _ev_graphics {
#ifdef WIN32
    int _unused;
#endif
};

ev_graphics* ev_graphics_create(void)
{
    ev_graphics *g = ev_malloc(sizeof(ev_graphics));

    if( g ) {
        memset(g, 0, sizeof(ev_graphics));
    }
    return g;
}

void ev_graphics_destroy(ev_graphics *g)
{
    ev_free(g);
}
void ev_graphics_clear(ev_graphics *graphics, float r, float g, float b, float a)
{
    UNUSED(graphics);

    glClearColor(r, g, b, a);
    glClear( GL_COLOR_BUFFER_BIT );
}
