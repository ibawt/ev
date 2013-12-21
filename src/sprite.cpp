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
    auto textureRect = frame->textureRect;

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
