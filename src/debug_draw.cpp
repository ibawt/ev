#include "debug_draw.h"
#include "vertex_buffer.h"

static const char vertex_shader[]=
    "#version 120\n"
    "attribute vec2 a_position;\n"
    "uniform mat4 u_projTrans;\n"
    "uniform float ratio;\n"
    "void main()\n"
    "{\n"
    "gl_Position = u_projTrans * vec4(ratio * a_position.x, ratio * a_position.y, 0, 1);\n"
    "}\n";

static const char fragment_shader[] =
    "#version 120\n"
    "uniform vec4 u_color;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = u_color;\n"
    "}\n";

static const char particle_vertex_shader[] =
    "#version 120\n"
    "attribute vec2 a_position;\n"
    "attribute vec4 a_color;\n"
    "varying vec4 color;\n"
    "uniform mat4 u_projTrans;\n"
    "uniform float ratio;\n"
    "void main()\n"
    "{\n"
    "gl_PointSize = 5;\n"
    "color = a_color;\n"
    "gl_Position = u_projTrans * vec4(ratio * a_position.x, ratio *a_position.y, 0, 1);\n"
    "}\n";

static const char particle_fragment_shader[] =
    "#version 120\n"
    "varying vec4 color;\n"
    "uniform sampler2D texture;\n"
    "void main()"
    "{"
    "gl_FragColor = color;\n"
    "}";


#define smoothstep(x) ( x * x * (3 - 2*x ) )


ev_program* gen_shader(const char *vertex_shader, const char *fragment_shader)
{
    ev_shader *vs = ev_shader_create();

    if( ev_shader_compile(vs, GL_VERTEX_SHADER, vertex_shader) ) {
        ev_error("vertex shader failed to compile");
        assert(true);
    }

    ev_shader *fs = ev_shader_create();
    if( ev_shader_compile(fs, GL_FRAGMENT_SHADER, fragment_shader) ) {
        ev_error("fragment shader failed to compile");
        assert(true);
    }

    ev_program* program = ev_program_create();
    ev_program_set_shader(program, vs, GL_VERTEX_SHADER);
    ev_program_set_shader(program, fs, GL_FRAGMENT_SHADER);
    if( ev_program_compile(program) ) {
        ev_error("shader failed to compile");
        assert(true);
    }
    return program;
}
b2DebugDraw::b2DebugDraw(float ratio) : mShader(NULL), mRatio(ratio)
{
    SetFlags( b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit | b2Draw::e_particleBit);

    vbuff = ev_vbuff_create();
    ev_vbuff_set_capacity(vbuff, 1024*1024*sizeof(float)*2);

    segment_vbuff = ev_vbuff_create();
    ev_vbuff_set_capacity(segment_vbuff, sizeof(float)*4);

    mShader = gen_shader(vertex_shader, fragment_shader);

    color_buff = ev_vbuff_create();
    ev_vbuff_set_capacity(color_buff, sizeof(float)*4*4096);

    particle_shader = gen_shader(particle_vertex_shader, particle_fragment_shader);

    CHECK_GL();
}

b2DebugDraw::~b2DebugDraw()
{
    ev_program_destroy(mShader);
    ev_vbuff_destroy(vbuff);
    ev_vbuff_destroy(segment_vbuff);
    ev_vbuff_destroy(color_buff);
    ev_program_destroy(particle_shader);
}


void b2DebugDraw::DrawPolygon(const b2Vec2* vertices, int cnt, const b2Color& color)
{
    GLfloat *verts = (GLfloat*)ev_vbuff_map(vbuff);
    memcpy(verts, vertices, cnt * sizeof(b2Vec2));
    ev_vbuff_unmap(vbuff);

    ev_program_use(mShader);
    ev_vbuff_bind(vbuff);

    glUniformMatrix4fv( ev_program_get_uniform_loc(mShader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);


    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r, color.g, color.b, 1.0f);

    glUniform1f(ev_program_get_uniform_loc(mShader, "ratio"),
                mRatio);

    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                          2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_LOOP, 0, cnt);
}

void b2DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int cnt, const b2Color& color)
{
    GLfloat *verts = (GLfloat*)ev_vbuff_map(vbuff);
    memcpy(verts, vertices, cnt * sizeof(b2Vec2));
    ev_vbuff_unmap(vbuff);

    ev_program_use(mShader);
    ev_vbuff_bind(vbuff);

    glUniformMatrix4fv( ev_program_get_uniform_loc(mShader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);


    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r*.5f, color.g*.5f, color.b*.5f, 0.5f);

    glUniform1f(ev_program_get_uniform_loc(mShader, "ratio"), mRatio);


    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                          2, GL_FLOAT, GL_FALSE, 0,0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, cnt);

    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r, color.g, color.b, 1.0f);

    glDrawArrays(GL_LINE_LOOP, 0, cnt);

    CHECK_GL();
}

void b2DebugDraw::DrawCircle(const b2Vec2& center, float radius,  const b2Color& color )
{
    DrawSolidCircle(center, radius, b2Vec2(0,0), color);
}

void b2DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    const float k_segments = 16.0f;
    const int vertex_cnt = 16;
    const float k_inc = 2.0f * b2_pi / k_segments;
    float theta = 0.0f;
    int i;
    GLfloat *verts = (GLfloat*)ev_vbuff_map(vbuff);

    for( i = 0 ; i < vertex_cnt ; ++i ) {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        verts[i*2] = v.x;
        verts[i*2+1] = v.y;
        theta += k_inc;
    }

    ev_vbuff_unmap(vbuff);

    ev_program_use(mShader);
    ev_vbuff_bind(vbuff);

    glEnableVertexAttribArray(ev_program_get_attrib_loc(mShader, "a_position"));
    CHECK_GL();
    glUniformMatrix4fv( ev_program_get_uniform_loc(mShader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);

    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r*.5f, color.g*.5f, color.b*.5f, 0.5f);

    glUniform1f(ev_program_get_uniform_loc(mShader, "ratio"),
                mRatio);

    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                           2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_cnt);

    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r, color.g, color.b, 1.0f);


    glDrawArrays(GL_LINE_LOOP, 0, vertex_cnt);

    DrawSegment(center, center + radius * axis, color );
}

void b2DebugDraw::SetTransform(ev_matrix4 *m)
{
    memcpy( &mMatrix.m, m->m, sizeof(ev_matrix4));
}

void b2DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count)
{
    static unsigned particle_texture = 0;
    float currentscale = 2.0f;

    //    count = 2048;
    // if (!particle_texture ||
    //     !glIsTexture(particle_texture))  {
    //     // generate a "gaussian blob" texture procedurally
    //     glGenTextures(1, &particle_texture);
    //     b2Assert(particle_texture);
    //     const int TSIZE = 64;
    //     unsigned char tex[TSIZE][TSIZE][4];
    //     for (int y = 0; y < TSIZE; y++) {
    //         for (int x = 0; x < TSIZE; x++)  {
    //             float fx = (x + 0.5f) / TSIZE * 2 - 1;
    //             float fy = (y + 0.5f) / TSIZE * 2 - 1;
    //             float dist = sqrtf(fx * fx + fy * fy);
    //             unsigned char intensity = (unsigned char)(dist <= 1 ? smoothstep(1 - dist) * 255 : 0);
    //             tex[y][x][0] = tex[y][x][1] = tex[y][x][2] = 128;
    //             tex[y][x][3] = intensity;
    //         }
    //     }
    //     glEnable(GL_TEXTURE_2D);
    //     glBindTexture(GL_TEXTURE_2D, particle_texture);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

    //     glDisable(GL_TEXTURE_2D);

    //     glEnable(GL_POINT_SMOOTH);
    // }

    ev_program_use(particle_shader);
    CHECK_GL();
    //glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, particle_texture);

    // but for some reason this is not applying textures, so we use alpha instead
    //    glEnable(GL_POINT_SPRITE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    const float particle_size_multiplier = 2;  // no falloff
    const float global_alpha = 0.35f;  // instead of texture

    glPointSize(radius * currentscale * particle_size_multiplier);
    //    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    //    glEnableClientState(GL_VERTEX_ARRAY);
    CHECK_GL();
    GLfloat *verts = (GLfloat*)ev_vbuff_map(vbuff);
    // for( int i = 0 ; i < count ; ++i ) {
    //     verts[i*2]   = centers[i].x;
    //     verts[i*2+1] = centers[i].y;
    // }

    memcpy(verts, &centers[0].x, count*2*sizeof(float));
    ev_vbuff_unmap(vbuff);
    ev_vbuff_bind(vbuff);


    CHECK_GL();

    glEnableVertexAttribArray(ev_program_get_attrib_loc(particle_shader, "a_position"));
    CHECK_GL();
    glUniformMatrix4fv( ev_program_get_uniform_loc(particle_shader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);


    glUniform1f(ev_program_get_uniform_loc(particle_shader, "ratio"),
                mRatio);
    CHECK_GL();
    glVertexAttribPointer(ev_program_get_attrib_loc(particle_shader, "a_position"),
                           2, GL_FLOAT, GL_FALSE, 0, 0);

    CHECK_GL();
    //glVertexPointer(2, GL_FLOAT, 0, &centers[0].x);


    if (colors)   {
        verts = (GLfloat*)ev_vbuff_map(color_buff);
        // for(int i = 0 ; i < count ; ++i ) {
        //     verts[i*4] = colors[i].r  / 255.0;
        //     verts[i*4+1] = colors[i].g / 255.0;
        //     verts[i*4+2] = colors[i].b / 255.0;
        //     verts[i*4+3] = colors[i].a / 255.0;
        // }
        memcpy( verts, &colors[0].r, count* 4);
        ev_vbuff_unmap(color_buff);
        ev_vbuff_bind(color_buff);

        glEnableVertexAttribArray(ev_program_get_attrib_loc(particle_shader,
                                                            "a_color"));

        glVertexAttribPointer(ev_program_get_attrib_loc(particle_shader,
                                                        "a_color"),
                              4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

    }
         // hack to render with proper alpha on desktop for Testbed
    //     b2ParticleColor * mcolors = const_cast<b2ParticleColor *>(colors);
    //     for (int i = 0; i < count; i++) {
    //         mcolors[i].a = static_cast<uint8>(global_alpha * 255);
    //     }
    //     glEnableClientState(GL_COLOR_ARRAY);
    //     glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0].r);
    // }
    // else {
    //     glColor4f(1, 1, 1, global_alpha);
    // }

    glDrawArrays(GL_POINTS, 0, count);
    CHECK_GL();
    //  glDisableClientState(GL_VERTEX_ARRAY);
    //    if (colors) glDisableClientState(GL_COLOR_ARRAY);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void b2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    ev_program_use(mShader);
    SetColor(color);
    GLfloat *verts = (GLfloat*)ev_vbuff_map(segment_vbuff);

    verts[0] = p1.x;
    verts[1] = p1.y;
    verts[2] = p2.x;
    verts[3] = p2.y;

    ev_vbuff_unmap(segment_vbuff);

    ev_vbuff_bind(segment_vbuff);

    glUniformMatrix4fv( ev_program_get_uniform_loc(mShader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);

    glUniform1f( ev_program_get_uniform_loc(mShader, "ratio"),
                 mRatio);

    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                          2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 2);
}


void b2DebugDraw::SetColor(const b2Color& c, float ratio)
{
    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                c.r * ratio, c.g * ratio, c.b * ratio, ratio);
}

void b2DebugDraw::DrawTransform(const b2Transform& xf)
{
}
