#include "evil.h"

#include "sprite.h"
#include "animation.h"
#include "sprite_sheet.h"

using namespace std;

namespace evil {

void Sprite::update(const float dt)
{
    if( animation )
        animation->update(dt);

//    rotation += 0.1;
}

const shared_ptr<SpriteFrame>& Sprite::getCurrentFrame() const
{
    return animation ? animation->getFrame() : frame;
}

void Sprite::fillBuffer(BatchVertex* bv)
{
    assert(bv != nullptr);

    const auto& verts = getCurrentFrame()->verts;

    for( int i = 0 ; i < NUM_VERTS ; ++i ) {
        bv[i].x = verts[i].x;
        bv[i].y = verts[i].y;
        bv[i].u = verts[i].u;
        bv[i].v = verts[i].v;

        bv[i].scale = scale;
        bv[i].rotation = rotation;
        bv[i].tx = position.x;
        bv[i].ty = position.y;
    }
}

void Sprite::render()
{
    BatchVertex v[NUM_VERTS];

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
