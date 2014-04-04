#include "texture.h"
#include "render_texture.h"

ev_rtex* ev_rtex_create(int width, int height)
{
    GLenum status;
    ev_rtex *rtex = ev_malloc(sizeof(ev_rtex));

    memset(rtex, 0, sizeof(ev_rtex));

    glGenFramebuffers(1, &rtex->fb_id);
    glBindFramebuffer(GL_FRAMEBUFFER, rtex->fb_id);

    glGenTextures(1, &rtex->texture.id);
    glBindTexture(GL_TEXTURE_2D, rtex->texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           rtex->texture.id, 0);

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( status != GL_FRAMEBUFFER_COMPLETE ) {
        ev_error("Error in generating framebuffer object: %d", status);
    }
    return rtex;
}

void ev_rtex_destroy(ev_rtex *rtex)
{
    if( rtex) {
        glDeleteTextures(1, &rtex->texture.id);
        glDeleteFramebuffers(1, &rtex->fb_id);

        ev_free(rtex);
    }
}

void ev_rtex_bind(ev_rtex* rtex)
{
    if( rtex ) {
        glBindFramebuffer(GL_FRAMEBUFFER, rtex->fb_id);
    }
}

void ev_rtex_unbind(ev_rtex *s)
{
    UNUSED(s);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ev_rtex_render(ev_rtex *r)
{
}
