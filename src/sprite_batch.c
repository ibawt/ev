#include <assert.h>

#include "matrix4.h"
#include "sprite_batch.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "utarray.h"
#include "sprite.h"
#include "sprite_sheet.h"
#include "application.h"
#include "ev_lua.h"
#include "animation.h"

#define SPRITE_BATCH_MAX 1024*10
#define GROW_FAC 4

#define VBUFF_SIZE(n) ((n)*sizeof(ev_bvertex)*EV_SPRITE_NUM_VERTS)

static const char *animationVertexShader =
    "#version 120\n"
    "attribute vec2 a_position;\n"
    "attribute vec2 a_texCoord0;\n"
    "attribute vec2 transform;\n"
    "attribute vec2 translation;\n"
    "attribute float opacity;\n"
    "varying float out_opacity;\n"
    "uniform mat4 u_projTrans;\n"
    "varying vec2 v_texCoords;\n"
    "void main()\n"
    "{\n"
    "mat4 t = mat4( cos( transform.x ),      -sin( transform.x ), 0.0, 0.0,\n"
    "                    sin( transform.x ),  cos( transform.x ), 0.0, 0.0,\n"
    "                    0.0,                 0.0,                1.0, 0.0,\n"
    "                    translation.x,       translation.y,      0.0, 1.0 );\n"
    "v_texCoords = a_texCoord0;\n"
    "out_opacity = opacity;\n"
    "gl_Position =  u_projTrans * t * vec4(transform.y * a_position.x, transform.y *a_position.y,0.0,1.0) ;\n"
    "}\n";

static const char *defaultFragmentShader =
    "#version 120\n"
    "varying vec2 v_texCoords;\n"
    "varying float out_opacity;\n"
    "uniform sampler2D u_texture;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = texture2D(u_texture, v_texCoords) * vec4(1,1,1,out_opacity) ;\n"
    "}\n";

struct _ev_sbatch {
    ev_texture   *texture;
    ev_program   *program;
    UT_array     *sprites;
    size_t        num_filled_sprites;
    ev_matrix4    matrix;
    ev_blend_func blend_func;
    ev_vbuff     *vbuff;
    ev_ssheet    *sheet;
};

static ev_err_t set_default_shader_program(ev_sbatch *b)
{
    ev_shader *vertex = NULL;
    ev_shader *fragment = NULL;
    assert( b != NULL);

    vertex = ev_shader_create();
    if( !vertex )
        return EV_FAIL;

    if( ev_shader_compile( vertex, GL_VERTEX_SHADER, animationVertexShader) ) {
        ev_error("vertex shader failed to compile");
        ev_shader_destroy(vertex);
        return EV_FAIL;
    }

    fragment = ev_shader_create();
    if( !fragment )
        return EV_FAIL;

    if( ev_shader_compile( fragment, GL_FRAGMENT_SHADER, defaultFragmentShader) ) {
        ev_error("fragment shader failed to compile");
        ev_shader_destroy(vertex);
        ev_shader_destroy(fragment);
        return EV_FAIL;
    }

    b->program = ev_program_create();
    if( !b->program ) {
        ev_shader_destroy(vertex);
        ev_shader_destroy(fragment);
        return EV_FAIL;
    }

    ev_program_set_shader(b->program, vertex, GL_VERTEX_SHADER);
    ev_program_set_shader(b->program, fragment, GL_FRAGMENT_SHADER);

    if( ev_program_compile(b->program) ) {
        ev_error("program failed to compile");
        ev_program_destroy(b->program);
        b->program = NULL;
        return EV_FAIL;
    }
    return EV_OK;
}

/* TODO: make sprite array a value array */
static const UT_icd icd = { sizeof(ev_sprite*), NULL, NULL, NULL };

ev_err_t ev_sbatch_set_vbuff_capacity(ev_sbatch *batch, size_t size)
{
    assert( batch );

    if(!batch->vbuff) {
        batch->vbuff = ev_vbuff_create();

        if(!batch->vbuff) {
            ev_error("failed to create vertex buffer");
            return EV_FAIL;
        }
    }

    if( ev_vbuff_set_capacity(batch->vbuff, VBUFF_SIZE(size))) {
        ev_error("failed to set vbuff capacity");
        return EV_FAIL;
    }

    return EV_OK;
}

int ev_sbatch_get_vbuff_capacity(ev_sbatch* s)
{
    assert( s );
    size_t cap = ev_vbuff_get_capacity(s->vbuff);
    return cap / VBUFF_SIZE(1);
}

ev_sbatch* ev_sbatch_create(void)
{
    ev_sbatch* s = ev_malloc(sizeof(ev_sbatch));

    if( !s ) {
        return NULL;
    }

    memset(s, 0, sizeof(ev_sbatch));

    utarray_new(s->sprites, &icd);

    if( ev_sbatch_set_vbuff_capacity(s, GROW_FAC) ) {
        ev_error("can't create sbatch");
    }

    if( set_default_shader_program(s) ) {
        ev_error("error in compiling default shader");
        ev_sbatch_destroy(s);
        return NULL;
    }

    ev_sbatch_set_blend_func(s, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    return s;
}

void ev_sbatch_blend_func(ev_sbatch *s, ev_blend_func func)
{
    if( s ) {
        s->blend_func = func;
    }
}

int ev_sbatch_num_filled_sprites(ev_sbatch *s)
{
    return s->num_filled_sprites;
}

int ev_sbatch_add_sprite(ev_sbatch *batch, ev_sprite *s)
{
    assert( batch != NULL );
    assert( s != NULL );
    assert( batch->vbuff != NULL );
    int len = -1;

    if( batch && s ) {
        size_t capacity = ev_vbuff_get_capacity(batch->vbuff);

        if( VBUFF_SIZE(utarray_len(batch->sprites)) >= capacity ) {
            capacity += VBUFF_SIZE(GROW_FAC);

            if( capacity >= VBUFF_SIZE(SPRITE_BATCH_MAX) ) {
                ev_error("out of capacity for this sprite batch!");
                return -1;
            }
            ev_vbuff_set_capacity(batch->vbuff, capacity);
        }
        len = utarray_len( batch->sprites);
        utarray_push_back( batch->sprites, &s);
    }
    return len;
}

void ev_sbatch_destroy(ev_sbatch* s)
{
    assert( s != NULL );

    if( s->program ) {
        ev_program_destroy(s->program);
        s->program = NULL;
    }

    if( s->sprites ) {
        utarray_free(s->sprites);
        s->sprites = NULL;
    }

    if( s->vbuff ) {
        ev_vbuff_destroy(s->vbuff);
        s->vbuff = NULL;
    }

    ev_free(s);
}

ev_err_t ev_sbatch_load(ev_sbatch *batch, const char *file)
{
    if( batch && file ) {
        batch->sheet = ev_ssheet_create();
        if( !batch->sheet) {
            return EV_FAIL;
        }
        if( ev_ssheet_load_file(batch->sheet, file) ) {
            ev_error("error in loading sprite json");
            ev_ssheet_destroy(batch->sheet);
            batch->sheet = NULL;
            return EV_FAIL;
        }

        return EV_OK;
    }
    return EV_FAIL;
}

ev_bvertex* ev_sbatch_lock(ev_sbatch *batch)
{
    batch->num_filled_sprites = 0;
    return ev_vbuff_map(batch->vbuff);
}

void ev_sbatch_unlock(ev_sbatch *batch, int num_filled)
{
    ev_vbuff_unmap(batch->vbuff);
    batch->num_filled_sprites = num_filled;
}

void ev_sbatch_update(ev_sbatch* batch, float dt)
{
    if( batch ) {
        batch->num_filled_sprites = 0;
        ev_bvertex *b = ev_vbuff_map(batch->vbuff);
        ev_sprite **s = NULL;

        while( ( s = (ev_sprite**)utarray_next( batch->sprites, s ) ) ) {
            ev_sprite_update(*s, dt);

            if( ev_sprite_fill( *s, b ) ) {
                batch->num_filled_sprites += 1;
                b += EV_SPRITE_NUM_VERTS;
            }
        }
        ev_vbuff_unmap(batch->vbuff);
    }
}

void ev_sbatch_render(ev_sbatch *batch, ev_matrix4 *t)
{
    int pos,tex,transform,translation,opacity;

    assert(batch);
    
    ev_vbuff_bind( batch->vbuff );
    glEnable(GL_TEXTURE_2D);
    ev_texture_bind( batch->texture );
    ev_program_use( batch->program );

    glUniformMatrix4fv( ev_program_get_uniform_loc(batch->program ,"u_projTrans" ), 1, GL_FALSE, t ? t->m : batch->matrix.m );

    pos = ev_program_get_attrib_loc(batch->program, "a_position");
    tex = ev_program_get_attrib_loc(batch->program, "a_texCoord0");
    transform = ev_program_get_attrib_loc(batch->program, "transform");
    translation = ev_program_get_attrib_loc(batch->program, "translation");
    opacity = ev_program_get_attrib_loc(batch->program, "opacity");

    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(tex);
    glEnableVertexAttribArray(transform);
    glEnableVertexAttribArray(translation);
    glEnableVertexAttribArray(opacity);

    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex,x) );
    glVertexAttribPointer(tex, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex,u));
    glVertexAttribPointer(transform, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex,rotation));
    glVertexAttribPointer(translation, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex, tx));
    glVertexAttribPointer(opacity, 1, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex, opacity));
    glEnable(GL_BLEND);
    glBlendFunc(batch->blend_func.src, batch->blend_func.dst);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, batch->num_filled_sprites*EV_SPRITE_NUM_VERTS);
    
    glDisableVertexAttribArray(opacity);
    glDisableVertexAttribArray(translation);
    glDisableVertexAttribArray(transform);
    glDisableVertexAttribArray(tex);
    glDisableVertexAttribArray(pos);
}

void ev_sbatch_destroy_sprite(ev_sbatch *s, int index)
{
    if( s ) {
        utarray_erase(s->sprites, index, 1);
    }
}


void ev_sbatch_set_blend_func(ev_sbatch *s, int dst, int src)
{
    assert(s);

    s->blend_func.dst = dst;
    s->blend_func.src = src;
}


void ev_sbatch_set_matrix4(ev_sbatch *batch, ev_matrix4 *matrix)
{
    if( batch && matrix ) {
        memcpy(&batch->matrix, matrix, sizeof(ev_matrix4));
    }
}


ev_anim* ev_sbatch_create_anim(ev_sbatch *batch, ...)
{
    ev_anim *a = ev_anim_create();
    const char *frame;
    va_list args;
    ev_sframe *sframe;

    va_start(args, batch);

    for(;;) {
        frame = va_arg(args, const char *);
        if( !frame )
            break;

        sframe = ev_sbatch_get_sframe(batch, frame);
        assert(sframe != NULL);

        ev_anim_add_sframe(a, sframe);
    }
    va_end(args);

    return a;
}

void ev_sbatch_set_texture(ev_sbatch *s, ev_texture *t)
{
    if( s ) {
        s->texture = t;
    }
}

ev_sframe* ev_sbatch_get_sframe(ev_sbatch *batch, const char *name)
{
    if( batch && name ) {
        return ev_ssheet_get_sframe(batch->sheet, name);
    }
    return NULL;
}
