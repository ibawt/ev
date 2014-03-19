#include "debug_draw.h"
#include "vertex_buffer.h"

static ev_vbuff *vbuff = NULL;
// inspired from the cocos class

static const char vertex_shader[]=
    "#version 120\n"
    "attribute vec2 a_position;\n"
    "uniform mat4 u_projTrans;\n"
    "uniform float ratio;\n"
    "void main()\n"
    "{\n"
    "gl_Position = u_projTrans * vec4(a_position.x, a_position.y, 0, 1);\n"
    "}\n";

static const char fragment_shader[] =
    "#version 120\n"
    "uniform vec4 u_color;\n"
    "void main()\n"
    "{\n"
    "gl_FragColor = u_color;\n"
    "}\n";


b2DebugDraw::b2DebugDraw(float ratio) : mRatio(ratio), mShader(NULL)
{
    SetFlags( b2Draw::e_shapeBit );

    ev_shader *vs = ev_shader_create();

    if( ev_shader_compile(vs, GL_VERTEX_SHADER, vertex_shader) ) {
        ev_error("vertex shader failed to compile");
    }

    ev_shader *fs = ev_shader_create();
    if( ev_shader_compile(fs, GL_FRAGMENT_SHADER, fragment_shader) ) {
        ev_error("fragment shader failed to compile");
    }

    mShader = ev_program_create();
    ev_program_set_shader(mShader, vs, GL_VERTEX_SHADER);
    ev_program_set_shader(mShader, fs, GL_FRAGMENT_SHADER);
    if( ev_program_compile(mShader) ) {
        ev_error("shader failed to compile");
    }

    ev_matrix4_set_ortho(&mMatrix, 0, 800, 600, 1, -1, 1);

    vbuff = ev_vbuff_create();
    ev_vbuff_set_capacity(vbuff, 2*2048);
    CHECK_GL();
}

b2DebugDraw::~b2DebugDraw()
{
    ev_program_destroy(mShader);
}


void b2DebugDraw::DrawPolygon(const b2Vec2* vertices, int cnt, const b2Color& color)
{
    ev_log("?");
    ev_program_use(mShader);

    glUniformMatrix4fv( ev_program_get_uniform_loc(mShader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);


    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r, color.g, color.b, 1.0f);

    glUniform1f(ev_program_get_uniform_loc(mShader, "ratio"),
                mRatio);

    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                          2, GL_FLOAT, GL_FALSE, 0, vertices);

    glDrawArrays(GL_LINE_LOOP, 0, cnt);
}

void b2DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int cnt, const b2Color& color)
{
    ev_log("how about here?");
    ev_program_use(mShader);


    glUniformMatrix4fv( ev_program_get_uniform_loc(mShader, "u_projTrans"),
                        1, GL_FALSE, mMatrix.m);


    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r*.5f, color.g*.5f, color.b*.5f, 0.5f);

    glUniform1f(ev_program_get_uniform_loc(mShader, "ratio"), mRatio);


    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                          2, GL_FLOAT, GL_FALSE, 0,vertices);

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

    // const float32 k_segments = 16.0f;
    // const float32 k_increment = 2.0f * b2_pi / k_segments;
    // float32 theta = 0.0f;
    // glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    // glBegin(GL_TRIANGLE_FAN);
    // for (int32 i = 0; i < k_segments; ++i) {
    //     b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
    //     glVertex2f(v.x*mRatio, v.y*mRatio);
    //     theta += k_inc;
    // }
    // glEnd();
    //   // glDisable(GL_BLEND);

    //   // theta = 0.0f;
    //   // glColor4f(color.r, color.g, color.b, 1.0f);
    // glBegin(GL_LINE_LOOP);
    // for (int32 i = 0; i < k_segments; ++i) {
    //     b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
    //     glVertex2f(v.x*mRatio, v.y*mRatio);
    //     theta += k_inc;
    // }
    // glEnd();

    // b2Vec2 p = center + radius * axis;
    // glBegin(GL_LINES);
    // glVertex2f(center.x*mRatio, center.y*mRatio);
    // glVertex2f(p.x*mRatio, p.y*mRatio);
    // glEnd();
    int i;
    GLfloat *verts = (GLfloat*)ev_vbuff_map(vbuff);


    for( i = 0 ; i < vertex_cnt ; ++i ) {
        b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
        verts[i*2] = v.x * mRatio;
        verts[i*2+1] = v.y * mRatio;

        //ev_log("verts[%d] - %.2f, %.2f", i, verts[i*2], verts[i*2+1]);
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

    //    glUniform1f(ev_program_get_uniform_loc(mShader, "ratio"), mRatio);

    glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
                           2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_cnt);

    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                color.r, color.g, color.b, 1.0f);


    glDrawArrays(GL_LINE_LOOP, 0, vertex_cnt);

    //    DrawSegment(center, center + radius * axis, color );
}

void b2DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    // ev_program_use(mShader);

    // SetColor(color);

    // GLfloat verts[] = {
    //     p1.x * mRatio, p1.y * mRatio,
    //     p2.x * mRatio, p2.y * mRatio
    // };


    // glVertexAttribPointer(ev_program_get_attrib_loc(mShader, "a_position"),
    //                       2, GL_FLOAT, GL_FALSE, 0,verts);
    // glDrawArrays(GL_LINES, 0, 2);
}


void b2DebugDraw::SetColor(const b2Color& c, float ratio)
{
    glUniform4f(ev_program_get_uniform_loc(mShader, "u_color"),
                c.r * ratio, c.g * ratio, c.b * ratio, ratio);
}

void b2DebugDraw::DrawTransform(const b2Transform& xf)
{
}
