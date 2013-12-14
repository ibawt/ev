#include "sprite.h"

namespace evil {

void Sprite::update(const float dt)
{
    animation->update(dt);
}

void Sprite::render()
{
    const auto& f = ( animation ? animation->getFrame() : frame );
    
    const auto& textureRect = f->textureRect;
    glBegin(GL_QUADS);
    // top left
    glTexCoord2f( textureRect.x, textureRect.y );
    glVertex2f( position.x, position.y );
    
    // top right
    glTexCoord2f( textureRect.x + textureRect.w, textureRect.y );
    glVertex2f( position.x + f->size.w, position.y );
    
    //bottom right
    glTexCoord2f( textureRect.x + textureRect.w, textureRect.y + textureRect.h);
    glVertex2f( position.x + f->size.w, position.y + f->size.h );

    // bottom left
    glTexCoord2f( textureRect.x, textureRect.y + textureRect.h );
    glVertex2f( position.x, position.y + f->size.h);
    glEnd();
}
}
