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

#define SPRITE_BATCH_MAX 1024*10
#define GROW_FAC 128

#define VBUFF_SIZE(n) ((n)*sizeof(ev_bvertex)*EV_SPRITE_NUM_VERTS)

static const char *animationVertexShader =
    "#version 120\n"
    "attribute vec2 a_position;\n"
    "attribute vec2 a_texCoord0;\n"
    "attribute vec2 transform;\n"
    "attribute vec2 translation;\n"
    "uniform mat4 u_projTrans;\n"
    "varying vec2 v_texCoords;\n"
    "void main()\n"
    "{\n"
    "mat4 t = mat4( cos( transform.x ),      -sin( transform.x ), 0.0, 0.0,\n"
    "                    sin( transform.x ),  cos( transform.x ), 0.0, 0.0,\n"
    "                    0.0,                 0.0,                1.0, 0.0,\n"
    "                    translation.x,       translation.y,      0.0, 1.0 );\n"
    "v_texCoords = a_texCoord0;\n"
    "gl_Position =  u_projTrans * t * vec4(transform.y * a_position.x, transform.y *a_position.y,0.0,1.0) ;\n"
    "}\n";

static const char *defaultFragmentShader =
    "#version 120\n"
    "varying vec2 v_texCoords;\n"
    "uniform sampler2D u_texture;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = texture2D(u_texture, v_texCoords);\n"
    "}\n";

struct _ev_sbatch {
    ev_texture* texture;
    ev_program* program;
    UT_array   *sprites;
    size_t      num_filled_sprites;
    ev_matrix4  matrix;
    ev_vbuff   *vbuff;
    ev_ssheet  *sheet;
    int         lua_ref;
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
    if(!batch)
        return EV_FAIL;

    batch->vbuff = ev_vbuff_create();

    if(!batch->vbuff) {
        ev_error("failed to create vertex buffer");
        return EV_FAIL;
    }

    if( ev_vbuff_set_capacity(batch->vbuff, VBUFF_SIZE(size))) {
        ev_error("failed to set vbuff capacity");
        return EV_FAIL;
    }

    return EV_OK;
}

static void sbatch_init(ev_sbatch *s)
{
    memset(s, 0, sizeof(ev_sbatch));

    utarray_new(s->sprites, &icd);

    if( ev_sbatch_set_vbuff_capacity(s, GROW_FAC) ) {
        ev_error("can't create sbatch");
    }
}

ev_sbatch* ev_sbatch_create(void)
{
    ev_sbatch* s = ev_malloc(sizeof(ev_sbatch));

    sbatch_init(s);

    return s;
}

ev_err_t ev_sbatch_add_sprite(ev_sbatch *batch, ev_sprite *s)
{
    assert( batch != NULL );
    assert( s != NULL );
    assert( batch->vbuff != NULL );

    if( batch && s ) {
        size_t capacity = ev_vbuff_get_capacity(batch->vbuff);

        if( VBUFF_SIZE(utarray_len(batch->sprites)) >= capacity ) {
            capacity += VBUFF_SIZE(GROW_FAC);

            if( capacity >= VBUFF_SIZE(SPRITE_BATCH_MAX) ) {
                ev_error("out of capacity for this sprite batch!");
                return EV_FAIL;
            }
            ev_vbuff_set_capacity(batch->vbuff, capacity);
        }

        utarray_push_back( batch->sprites, &s);
    }
    return EV_OK;
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
        if( !batch->sheet) {
            return EV_FAIL;
        }
        if( ev_ssheet_load_file(batch->sheet, file) ) {
            ev_error("error in loading sprite json");
            ev_ssheet_destroy(batch->sheet);
            batch->sheet = NULL;
            return EV_FAIL;
        }

        if( set_default_shader_program(batch) ) {
            ev_error("error in shader compilation");
            ev_ssheet_destroy(batch->sheet);
            return EV_FAIL;
        }

        return EV_OK;
    }
    return EV_FAIL;
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

void ev_sbatch_render(ev_sbatch *batch)
{
    int pos,tex,transform,translation;

    if(!batch)
        return;

    ev_vbuff_bind( batch->vbuff );
    glEnable(GL_TEXTURE_2D);
    ev_texture_bind( batch->texture );
    ev_program_use( batch->program );

    glUniformMatrix4fv( ev_program_get_uniform_loc(batch->program ,"u_projTrans" ), 1, GL_FALSE, batch->matrix.m );


    pos = ev_program_get_attrib_loc(batch->program, "a_position");
    tex = ev_program_get_attrib_loc(batch->program, "a_texCoord0");
    transform = ev_program_get_attrib_loc(batch->program, "transform");
    translation = ev_program_get_attrib_loc(batch->program, "translation");

    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(tex);
    glEnableVertexAttribArray(transform);
    glEnableVertexAttribArray(translation);

    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex,x) );
    glVertexAttribPointer(tex, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex,u));
    glVertexAttribPointer(transform, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex,rotation));
    glVertexAttribPointer(translation, 2, GL_FLOAT, GL_TRUE, sizeof(ev_bvertex), (void*)offsetof(ev_bvertex, tx));

    glDrawArrays(GL_TRIANGLES, 0, batch->num_filled_sprites*EV_SPRITE_NUM_VERTS);

    glDisableVertexAttribArray(translation);
    glDisableVertexAttribArray(transform);
    glDisableVertexAttribArray(tex);
    glDisableVertexAttribArray(pos);
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

#define EV_SBATCH_KEY "__ev_sbatch"
#define EV_SBATCH_META "__ev_sbatch_meta"

#define check_sbatch(s) ev_sbatch_from_lua(s, 1)

ev_sbatch* ev_sbatch_from_lua(lua_State *l, int arg)
{
    ev_sbatch *s;

    assert( l != NULL );

    luaL_checktype(l, arg, LUA_TTABLE);
    lua_getfield(l, arg, EV_SBATCH_KEY);

    s = lua_touserdata(l, -1);

    luaL_argcheck(l, s != NULL, 1, "ev_sbatch expected");
    assert( s != NULL );

    return s;
}

static int l_sbatch_create(lua_State *l)
{
    ev_sbatch *s;

    assert( l != NULL );

    if( ev_app_get_state(ev_app_get_lua_instance()) != EV_APP_STATE_READY) {
        lua_pushstring(l, "app not ready, call init first!");
        lua_error(l);
        return 1;
    }

    lua_newtable(l);

    luaL_getmetatable(l, EV_SBATCH_META);
    lua_setmetatable(l, -2);

    s = lua_newuserdata(l, sizeof(ev_sbatch));
    sbatch_init(s);

    lua_setfield(l, -2, EV_SBATCH_KEY);
    s->lua_ref = ev_lua_create_ref( l, 1);

    return 1;
}

static int l_sbatch_destroy(lua_State *l)
{
    ev_sbatch *s;

    assert( l != NULL );

    s = check_sbatch(l);

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

    return 0;
}

static int l_sbatch_load(lua_State *l)
{
    ev_sbatch *s;

    s = check_sbatch(l);

    if( ev_sbatch_load(s, lua_tostring(l, 2)) ) {
        lua_pushstring(l, "error in loading sbatch");
        lua_error(l);
    }

    return 0;
}

int l_sbatch_set_texture(lua_State *l)
{
    ev_sbatch  *s;
    ev_texture *t;

    s = check_sbatch(l);
    t = ev_texture_from_lua(l, 2);

    s->texture = t;

    //lua_setfield(l, 1, "texture");

    return 0;
}

static int l_sbatch_get_sframe(lua_State *l)
{
    ev_sbatch *s;
    ev_sframe *frame;
    const char *key;

    s = check_sbatch(l);

    key = lua_tostring(l, 2);

    frame = ev_sbatch_get_sframe(s, key);
    if( !frame ) {
        lua_pushfstring(l, "can't find frame for key: %s", key);
        lua_error(l);
        return 0;
    } else {
        lua_pushlightuserdata(l, frame);
        return 1;
    }
}

static int l_sbatch_add_sprite(lua_State *l)
{
    ev_sbatch *batch;
    ev_sprite *s;

    batch = check_sbatch(l);

    s = ev_sprite_from_lua(l, 2);

    ev_sbatch_add_sprite(batch, s);

    lua_pop(l,2);

    luaL_getsubtable(l, 1, "sprites");

    lua_pushvalue(l, 2);

    lua_remove(l,1);

    lua_remove(l,1);

    ev_lua_table_insert(l);

    return 0;
}

static int l_sbatch_set_ortho(lua_State *l)
{
    ev_sbatch *batch;
    float w,h;

    batch = check_sbatch(l);

    w = (float)lua_tonumber(l, 2);
    h = (float)lua_tonumber(l, 3);

    ev_matrix4_set_ortho(&batch->matrix, 0, w, h, 1, -1, 1);

    return 0;
}

static const luaL_Reg sbatch_lua_funcs[] = {
    { "create", l_sbatch_create },
    { "__gc", l_sbatch_destroy },
    { "load", l_sbatch_load },
    { "get_frame", l_sbatch_get_sframe},
    { "set_texture", l_sbatch_set_texture},
    { "add_sprite", l_sbatch_add_sprite },
    { "set_ortho", l_sbatch_set_ortho },
    { 0, 0 }
};

ev_err_t ev_sbatch_lua_init(lua_State *l)
{
    assert( l != NULL );

    luaL_newmetatable(l, EV_SBATCH_META);
    luaL_setfuncs(l, sbatch_lua_funcs, 0);
    lua_pushvalue(l, -1);
    lua_setfield(l, -1, "__index");

    lua_getglobal(l, "ev");
    lua_pushvalue(l, -2);
    lua_setfield(l, -2, "sbatch");

    return EV_OK;
}
