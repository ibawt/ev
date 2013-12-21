#include "evil.h"
#include "sprite_batch.h"
#include "sprite.h"
#include "jansson.h"
#include "utils.h"

#include <stdio.h>

using namespace std;

namespace evil {

static const string animationVertexShader(
"#version 120\n"
"attribute vec4 a_position;\n"
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
"}\n"
 );


static const string defaultFragmentShader(
"#version 120\n"
"varying vec2 v_texCoords;\n"
"uniform sampler2D u_texture;\n"
"void main()\n"
"{\n"
"gl_FragColor = texture2D(u_texture, v_texCoords);\n"
"}\n");

SpriteBatch::SpriteBatch()
{
}

static bool parseRect(json_t *obj, const char *key, Rect& r)
{
    if( obj ) {
        json_t *value = json_object_get(obj,key);
        if( value && json_is_string(value)) {
            const char *data = json_string_value(value);
            int x,y,w,h;

            if( sscanf(data, "{{%d, %d}, {%d, %d}}", &x, &y, &w, &h ) != 4 )
                return false;

            r.x = x;
            r.y = y;
            r.w = w;
            r.h = h;

            return true;
        }
    }
    return false;
}


static bool parseSize(json_t *obj, const char *key, Rect& r)
{
    if( obj ) {
        json_t *value = json_object_get(obj, key );
        if( value && json_is_string(value) ) {
            const char *data = json_string_value(value);
            int w,h;
            if (sscanf(data, "{%d, %d}", &w, &h) < 2 ) {
                return false;
            }
            r.w = w;
            r.h = h;

            return true;
        }
    }
    return false;
}

static bool parseVector(json_t *obj, const char *key, Vector2& v ) {
    Rect r;
    if( parseSize(obj, key, r) ) {
        v.x = r.w;
        v.y = r.h;

        return true;
    }
    return false;
}

void SpriteBatch::generateBuffer()
{
    verts.clear();
    vboID = 0;

    for(auto& s : sprites ) {
        s->fill(verts);
    }

    glGenBuffers( 1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(BatchVertex), &verts[0], GL_STATIC_DRAW);
}

void SpriteBatch::fillVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    BatchVertex* bv = (BatchVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    if( bv ) {
        for(auto &s :sprites ) {
            s->fillBuffer(bv);
            bv += 6; // 6 per
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
}

void SpriteBatch::setTexture( shared_ptr<Texture>& t )
{
    texture = t;

    for( auto& frame : sheet.frames ) {
        frame.second->textureRect.x /= t->getWidth();
        frame.second->textureRect.y /= t->getHeight();
        frame.second->textureRect.w /= t->getWidth();
        frame.second->textureRect.h /= t->getHeight();
        fillBuffer(frame.second);
    }
}
void SpriteBatch::fillBuffer(shared_ptr<SpriteFrame>& frame)
{
    auto textureRect = frame->textureRect;

    auto bv = &frame->verts[0];

    bv->x = -frame->size.w/2;
    bv->y = -frame->size.h/2;
    bv->u = textureRect.x;
    bv->v = textureRect.y;

    bv[1].x = frame->size.w/2;
    bv[1].y = -frame->size.h/2;
    bv[1].u = textureRect.x + textureRect.w;
    bv[1].v = textureRect.y;

    bv[2].x = frame->size.w/2;
    bv[2].y = frame->size.h/2;
    bv[2].u = textureRect.x + textureRect.w;
    bv[2].v = textureRect.y + textureRect.h;

    bv[3] = bv[2];

    bv[4].x = -frame->size.w/2;
    bv[4].y = frame->size.h/2;
    bv[4].u = textureRect.x;
    bv[4].v = textureRect.y + textureRect.h;

    bv[5] = bv[0];

    for( int i = 0 ; i < 6 ; ++i ) {
        bv[i].scale = 1.0f;
        bv[i].rotation = 0.0f;
        bv[i].tx = 0.0f;
        bv[i].ty = 0.0f;
    }
}

bool SpriteBatch::load(const string& json)
{
    sheet.frames.clear();

    json_t *root;
    json_error_t err;

    root = json_load_file( json.c_str(), 0, &err );

    if( !root ) {
        error("File didn't load");
        return false;
    }

    auto dl = MakeScopeExit([&root]() { json_decref(root); root = nullptr; });

#if 0
    char *output = json_dumps(root, JSON_INDENT(1) );
    if( !output ) {
        json_decref(root);
        return false;
    }

    printf("%s\n", output);
    free( output );
#endif

    json_t* frames = json_object_get(root, "frames");

    if( !frames ) {
        json_decref(root);
        return false;
    }

    const char *key;
    json_t *value;

    json_object_foreach( frames, key, value ) {
        auto frame = make_shared<SpriteFrame>();

        frame->key = string(key);
        frame->trimmed = json_is_true(json_object_get(value, "spriteTrimmed"));
        frame->rotated = json_is_true(json_object_get(value, "textureRotated"));

        if( !(parseSize(value, "spriteSourceSize", frame->sourceSize ) &&
              parseSize(value, "spriteSize", frame->size ) &&
              parseRect(value, "textureRect", frame->textureRect) &&
              parseVector(value, "spriteOffset", frame->offset) &&
              parseRect(value, "spriteColorRect", frame->colorRect)) ) {
            return false;
        }
        sheet.frames[key] = frame;
    }

    program.setVertexShader(animationVertexShader);
    program.setFragmentShader(defaultFragmentShader);

    if( !program.compile() ) {
        return false;
    }

    return true;
}

shared_ptr<Sprite> SpriteBatch::get(const string& name)
{
    auto s = make_shared<Sprite>();

    auto& frame = sheet.frames[name];

    s->setFrame(frame);
    return s;
}

void SpriteBatch::update(const float dt)
{
    fillVertexBuffer();
}

void SpriteBatch::render()
{
#ifdef NO_SHADERS
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnable(GL_TEXTURE_2D);
    texture->bind();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT,  sizeof(BatchVertex), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(BatchVertex), (void*)8 );
    glDrawArrays(GL_TRIANGLES, 0, sprites.size()*6);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnable(GL_TEXTURE_2D);
    texture->bind();
    program.use();
    program.setUniformMatrix("u_projTrans", transform);
    glEnableVertexAttribArray(program.getAttribLocation("a_position"));
    glEnableVertexAttribArray(program.getAttribLocation("a_texCoord0"));
    glEnableVertexAttribArray(program.getAttribLocation("transform"));
    glEnableVertexAttribArray(program.getAttribLocation("translation"));

    glVertexAttribPointer(program.getAttribLocation("a_position"), 2, GL_FLOAT, GL_TRUE,
                          sizeof(BatchVertex), 0 );
    glVertexAttribPointer(program.getAttribLocation("a_texCoord0"),
                          2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex), (void*)8);
    glVertexAttribPointer(program.getAttribLocation("transform"), 2, GL_FLOAT, GL_TRUE,
                          sizeof(BatchVertex), (void*)16);

    glVertexAttribPointer(program.getAttribLocation("translation"), 2, GL_FLOAT, GL_TRUE,
                          sizeof(BatchVertex), (void*)24);

    glDrawArrays(GL_TRIANGLES, 0, sprites.size()*6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
#endif
}

}
