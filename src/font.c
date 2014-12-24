#include <assert.h>
#include <wchar.h>
#include <string.h>

#include "freetype-gl.h"
#include "vertex-buffer.h"
#include "mat4.h"
#include "utils.h"
#include "evil.h"
#include "font.h"
#include "shader.h"

static const char* VERTEX_SHADER = \
"uniform mat4 projection; \
attribute vec3 vertex; \
attribute vec2 tex_coord; \
attribute vec4 color; \
void main() \
{ \
    gl_TexCoord[0].xy = tex_coord.xy; \
    gl_FrontColor     = color; \
    gl_Position       = projection*vec4(vertex,1.0); \
}";

static const char* FRAGMENT_SHADER = " \
uniform sampler2D texture; \
void main() \
{ \
    float a = texture2D(texture, gl_TexCoord[0].xy).r; \
    gl_FragColor = vec4(gl_Color.rgb, gl_Color.a*a); \
}";

typedef struct {
    float x, y, z;
    float s, t;
    float r, g, b, a;
} vertex_t;

struct _ev_font {
    wchar_t    *wbuff;
    size_t      wbuff_len;
    ev_program *program;
    ev_vec2     position;
    /* freetype-gl things */
    vertex_buffer_t *vbuff;
    texture_font_t  *tfont;
    texture_atlas_t *tatlas;
    float            colour[4]; /* rgba */
};

ev_font* ev_font_create(const char *filename, float pt_size)
{
    ev_font *font = ev_malloc(sizeof(ev_font));

    if( font ) {
        memset(font, 0, sizeof(ev_font));

        font->tatlas = texture_atlas_new(512,512,1); /* TODO: parameterize */
        font->vbuff = vertex_buffer_new( "vertex:3f,tex_coord:2f,color:4f" );

        font->tfont = texture_font_new_from_file(font->tatlas, pt_size, filename);

        assert(font->tfont != NULL);
        
        font->program = ev_program_create_with_shaders(VERTEX_SHADER, FRAGMENT_SHADER);

        mat4_set_orthographic(&font->projection, 0, 800, 0, 600, -1, 1);
        assert( font->program != NULL );
     }

    return font;
}

void ev_font_destroy(ev_font *font)
{
    assert( font );

    if( font->tfont ) {
        texture_font_delete(font->tfont);
        font->tfont = NULL;
    }

    if( font->vbuff ) {
        vertex_buffer_delete(font->vbuff);
        font->vbuff = NULL;
    }

    if( font->tatlas ) {
        texture_atlas_delete(font->tatlas);
        font->tatlas = NULL;
    }

    if( font->program ) {
        ev_program_destroy(font->program);
        font->program = NULL;
    }

    if( font->wbuff ) {
        ev_free(font->wbuff);
        font->wbuff = NULL;
    }

    ev_free(font);
}

/* shamelessly copied from the freetype demos */
static float fill_vertex_buff(ev_font *font, const wchar_t *text, int len)
{
    int i;
    float penx = font->position.x;
    int x0,y0, x1,y1;
    float s0,t0,s1,t1;
    
    for( i = 0 ; i < len ; ++i ) {
        texture_glyph_t *glyph = texture_font_get_glyph(font->tfont, text[i]);
        if( glyph ) {
            float kerning = 0.0f;
            GLuint indices[6] = { 0, 1, 2, 0, 2, 3 };
            vertex_t vertices[4];
            if( i > 0 ) {
                kerning = texture_glyph_get_kerning(glyph, text[i-1]);
            }
            penx += kerning;
            x0 = (int)(penx + glyph->offset_x);
            y0 = (int)(font->position.y + glyph->offset_y);
            x1 = (int)(x0 + glyph->width);
            y1 = (int)(y0 - glyph->height);
            s0 = glyph->s0;
            t0 = glyph->t0;
            s1 = glyph->s1;
            t1 = glyph->t1;

            vertices[0].x = x0;
            vertices[0].y = y0;
            vertices[0].z = 0;
            vertices[0].s = s0;
            vertices[0].t = t0;
            vertices[0].r = font->colour[0];
            vertices[0].g = font->colour[1];
            vertices[0].b = font->colour[2];
            vertices[0].a = font->colour[3];
            

            vertices[1].x = x0;
            vertices[1].y = y1;
            vertices[1].z = 0;
            vertices[1].s = s0;
            vertices[1].t = t1;
            vertices[1].r = font->colour[0];
            vertices[1].g = font->colour[1];
            vertices[1].b = font->colour[2];
            vertices[1].a = font->colour[3];

            vertices[2].x = x1;
            vertices[2].y = y1;
            vertices[2].z = 0;
            vertices[2].s = s1;
            vertices[2].t = t1;
            vertices[2].r = font->colour[0];
            vertices[2].g = font->colour[1];
            vertices[2].b = font->colour[2];
            vertices[2].a = font->colour[3];

            vertices[3].x = x1;
            vertices[3].y = y0;
            vertices[3].z = 0;
            vertices[3].s = s1;
            vertices[3].t = t0;
            vertices[3].r = font->colour[0];
            vertices[3].g = font->colour[1];
            vertices[3].b = font->colour[2];
            vertices[3].a = font->colour[3];

            vertex_buffer_push_back(font->vbuff, vertices, 4, indices, 6);
            penx += glyph->advance_x;
        }
    }
    return penx;
}

void ev_font_render(ev_font *font, ev_matrix4 *transform)
{
    assert(font != NULL);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glBindTexture(GL_TEXTURE_2D, font->tatlas->id);
    ev_program_use( font->program );

    glUniform1i(ev_program_get_uniform_loc(font->program, "texture"), 0);
    glUniformMatrix4fv( ev_program_get_uniform_loc(font->program, "projection"), 1, 0, transform->m);

    vertex_buffer_render(font->vbuff, GL_TRIANGLES);
}

float ev_font_set_text(ev_font *font, const char *text, int in_len)
{
    int dst_len;
    int len;
    int r;
    float width;
    
    assert(font);
    assert(text);

    if( in_len < 0 ) {
        in_len = strlen(text);
    }
    /* POSIX lets query the number of chars */
    dst_len = mbstowcs(NULL, text, 0);

    if(!font->wbuff || dst_len > font->wbuff_len ) {
        if( font->wbuff ) {
            ev_free(font->wbuff);
        }
        font->wbuff = ev_malloc(sizeof(wchar_t)*dst_len + 1);
        font->wbuff_len = dst_len;
    }
    
    len = mbstowcs(font->wbuff, text, font->wbuff_len);

    assert(len > 0 );

    r = texture_font_load_glyphs(font->tfont, font->wbuff);

    assert(r == 0);

    width = fill_vertex_buff(font, font->wbuff, len);

    return width;
}

void ev_font_set_position(ev_font *f, ev_vec2 pos)
{
    assert(f);

    f->position = pos;
}

ev_vec2 ev_font_get_position(ev_font *f)
{
    assert(f);
    return f->position;
}

void ev_font_set_colour(ev_font *f, float r, float g, float b, float a)
{
    assert(f);
    f->colour[0] = r;
    f->colour[1] = g;
    f->colour[2] = b;
    f->colour[3] = a;
}
