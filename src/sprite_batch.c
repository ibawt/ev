#include "matrix4.h"
#include "sprite_batch.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "utarray.h"

struct _ev_sbatch {
    ev_texture* texture;
    ev_program* program;
    UT_array   *sprites;
    ev_matrix4  matrix;
    ev_vbuff   *vbuff;
    ev_ssheet  *sheet;
};

/* TODO: make sprite array a value array */
static const UT_icd icd = { sizeof(ev_sprite*), NULL, NULL, NULL };

ev_sbatch* ev_sbatch_create(void)
{
    ev_sbatch* s = ev_malloc(sizeof(ev_sbatch));

    memset(s, 0, sizeof(ev_sbatch));

    utarray_new(s->sprites, &icd);

    return s;
}

void ev_sbatch_add_sprite(ev_sbatch *batch, ev_sprite *s)
{
    if( batch && s ) {
        utarray_push_back( batch->sprites, s);
    }
}

void ev_sbatch_destroy(ev_sbatch* s)
{
    if( s ) {
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
}

ev_err_t ev_sbatch_load(ev_sbatch *batch, const char *file)
{
    if( batch && file ) {
        batch->sheet = ev_ssheet_create();

    }
    return EV_FAIL;
}

void ev_sbatch_update(ev_sbatch* batch, float dt)
{
}

void ev_sbatch_render(ev_sbatch *batch)
{
}

void ev_sbatch_set_matrix4(ev_sbatch *batch, ev_matrix4 *matrix)
{
    if( batch && matrix ) {
        memcpy(&batch->matrix, matrix, sizeof(ev_matrix4));
    }
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
