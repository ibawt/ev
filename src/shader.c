#include "shader.h"

struct _ev_shader
{
    GLuint id;
};

struct _ev_program
{
    GLuint id;
    ev_shader *vertex;
    ev_shader *fragment;
};

ev_shader* ev_shader_create(void)
{
    ev_shader *s = ev_malloc(sizeof(ev_shader));
    memset( s, 0, sizeof(ev_shader));
    return s;
}

void ev_shader_destroy(ev_shader *s)
{
    if( s ) {
        if( s->id ) {
            glDeleteShader(s->id);
            s->id = 0;
        }
        ev_free(s);
    }
}

ev_err_t ev_shader_compile(ev_shader* shader, GLenum type, const char *source)
{
    int status;

    if( !shader || !source )
        return EV_FAIL;

    glShaderSource( shader->id, 1, &source, 0);
    glCompileShader(shader->id);

    glGetShaderiv( shader->id, GL_COMPILE_STATUS, &status);

    if( status == GL_FALSE ) {
        int   loglength;
        char *log;

        glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &loglength);
        log = ev_malloc(loglength+1);
        if( !log )
            return EV_NOMEM;

        glGetShaderInfoLog(shader->id, loglength,&loglength, log);

        log[loglength] = '\0';

        ev_error("Error in compiling shader:\n%s", log);

        ev_free(log);

        return EV_FAIL;
    }
    return EV_OK;
}

ev_program* ev_program_create(void)
{
    ev_program* p = ev_malloc(sizeof(ev_program));
    memset(p, 0, sizeof(ev_program));
    return p;
}

void ev_program_destroy(ev_program *p)
{
    if( p ) {
        if( p->vertex ) {
            ev_shader_destroy(p->vertex);
            p->vertex = NULL;
        }
        if( p->fragment ) {
            ev_shader_destroy(p->fragment);
            p->fragment = NULL;
        }

        if( p->id ) {
            glDeleteProgram(p->id);
            p->id = 0;
        }

        ev_free(p);
    }
}

void ev_program_set_shader(ev_program *p, ev_shader* s, GLenum type)
{
    if( p ) {
        switch( type ) {
        case GL_VERTEX_SHADER:
            p->vertex = s;
            break;
        case GL_FRAGMENT_SHADER:
            p->fragment = s;
        }
    }
}

ev_err_t ev_program_compile(ev_program* p)
{
    if( p && p->vertex && p->fragment ) {
        if( !p->vertex->id || !p->fragment->id ) {
            return EV_FAIL;
        }

        if( p->id ) {
            return EV_FAIL;
        }

        p->id = glCreateProgram();
        if( !p->id )
            return EV_FAIL;

        glAttachShader(p->id, p->vertex->id);
        glAttachShader(p->id, p->fragment->id);
        glLinkProgram(p->id);

        return EV_OK;
    }
    return EV_FAIL;
}
