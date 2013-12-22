#include "animation.h"
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

void SpriteBatch::generateBuffer()
{
    vertexBuffer.resize( sprites.size() * Sprite::NUM_VERTS );
    fillVertexBuffer();
}

void SpriteBatch::fillVertexBuffer()
{
    BatchVertex* bv = vertexBuffer.map();
    if( bv ) {
        for( const auto& sprite : sprites ) {
            sprite->fillBuffer(bv);
            bv += Sprite::NUM_VERTS;
        }
        vertexBuffer.unmap();
    }
}

void SpriteBatch::setTexture( shared_ptr<Texture>& t )
{
    texture = t;
}

bool SpriteBatch::load(const string& json)
{
    if( !sheet.load(json) )
        return false;

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

    auto frame = sheet.getFrame(name);

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
    vertexBuffer.bind();
    glEnable(GL_TEXTURE_2D);
    texture->bind();
    program.use();
    program.setUniformMatrix("u_projTrans", transform);
    glEnableVertexAttribArray(program.getAttribLocation("a_position"));
    glEnableVertexAttribArray(program.getAttribLocation("a_texCoord0"));
    glEnableVertexAttribArray(program.getAttribLocation("transform"));
    glEnableVertexAttribArray(program.getAttribLocation("translation"));

    glVertexAttribPointer(program.getAttribLocation("a_position"), 2, GL_FLOAT, GL_TRUE,
                          sizeof(BatchVertex), (void*)offsetof(BatchVertex,x) );
    glVertexAttribPointer(program.getAttribLocation("a_texCoord0"),
                          2, GL_FLOAT, GL_TRUE, sizeof(BatchVertex), (void*)offsetof(BatchVertex,u));
    glVertexAttribPointer(program.getAttribLocation("transform"), 2, GL_FLOAT, GL_TRUE,
                          sizeof(BatchVertex), (void*)offsetof(BatchVertex,rotation));

    glVertexAttribPointer(program.getAttribLocation("translation"), 2, GL_FLOAT, GL_TRUE,
                          sizeof(BatchVertex), (void*)offsetof(BatchVertex, tx));

    glDrawArrays(GL_TRIANGLES, 0, sprites.size()*6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
#endif
}

}
