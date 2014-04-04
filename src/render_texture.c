#include "texture.h"
#include "render_texture.h"
#include <assert.h>

ev_rtex* ev_rtex_create(int width, int height)
{
    GLenum status;
    ev_rtex *rtex = ev_malloc(sizeof(ev_rtex));

    if( !rtex ) {
        return NULL;
    }

    memset(rtex, 0, sizeof(ev_rtex));

    glGenFramebuffers(1, &rtex->fb_id);
    glBindFramebuffer(GL_FRAMEBUFFER, rtex->fb_id);

    rtex->texture.width = width;
    rtex->texture.height = height;

    glGenTextures(1, &rtex->texture.id);
    glBindTexture(GL_TEXTURE_2D, rtex->texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           rtex->texture.id, 0);

    GLuint rboId;
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                          width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                              GL_DEPTH_ATTACHMENT, // 2. attachment point
                              GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                              rboId);              // 4. rbo ID

    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( status != GL_FRAMEBUFFER_COMPLETE ) {
        ev_error("Error in generating framebuffer object: %d", status);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        glGetFloatv(GL_VIEWPORT, rtex->viewport);
        glBindFramebuffer(GL_FRAMEBUFFER, rtex->fb_id);
        glDisable(GL_DEPTH_TEST);
        glViewport(0, 0, rtex->texture.width, rtex->texture.height);
    }
}

void ev_rtex_unbind(ev_rtex *rtex)
{
    assert(rtex != NULL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(rtex->viewport[0], rtex->viewport[1], rtex->viewport[2], rtex->viewport[3]);
}

void ev_rtex_render(ev_rtex *r)
{
}
