#include <assert.h>

#include "texture.h"
#include "ev_lua.h"

#define COLOR_FMT GL_RGBA

struct _ev_texture
{
    GLuint   id;
    uint32_t width;
    uint32_t height;
};

ev_texture* ev_texture_create(void)
{
    ev_texture *t = ev_malloc( sizeof(ev_texture));
    memset( t, 0, sizeof(ev_texture));

    return t;
}

void ev_texture_destroy(ev_texture *t)
{
    if( t ) {
        if( t->id ) {
            glDeleteTextures(1, &t->id);
            t->id = 0;
        }
        ev_free(t);
    }
}

void ev_texture_bind(ev_texture* t)
{
    assert( t->id != 0 );
    if( t && t->id) {
        glBindTexture( GL_TEXTURE_2D, t->id );
    }
}

ev_err_t ev_texture_load(ev_texture* t, const char *filename)
{
    SDL_Surface *s;

    if( !t || !filename )
        return EV_FAIL;

    s = IMG_Load(filename);

    if( !s )
        return EV_FAIL;

    glGenTextures(1, &t->id);
    glBindTexture(GL_TEXTURE_2D, t->id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, COLOR_FMT, GL_UNSIGNED_BYTE, s->pixels);

    t->width = s->w;
    t->height = s->h;

    SDL_FreeSurface(s);

    return EV_OK;
}
