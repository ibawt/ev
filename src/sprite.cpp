#include "sprite.h"

using namespace std;

namespace evil {

void Sprite::update(const float dt)
{
    if( animation )
        animation->update(dt);

    rotation += 0.01f;
}

const shared_ptr<SpriteFrame>& Sprite::getCurrentFrame() const
{
    return animation ? animation->getFrame() : frame;
}

void Sprite::fillBuffer(BatchVertex* bv)
{
    assert(bv != nullptr);

    auto frame = getCurrentFrame();

    for( int i = 0 ; i < 6 ; ++i ) {
        bv[i].x = frame->verts[i].x;
        bv[i].y = frame->verts[i].y;
        bv[i].u = frame->verts[i].u;
        bv[i].v = frame->verts[i].v;

        bv[i].scale = scale;
        bv[i].rotation = rotation;
        bv[i].tx = position.x;
        bv[i].ty = position.y;
    }
}

void Sprite::fill(vector<BatchVertex>& verts)
{
    BatchVertex v[6];

    fillBuffer(v);

    for( int i = 0 ; i < 6 ; ++i ) {
        verts.push_back(v[i]);
    }
}

void Sprite::render()
{
    BatchVertex v[6];

    fillBuffer(v);

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(BatchVertex), &v[0].u);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(BatchVertex), &v[0].x);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
}
