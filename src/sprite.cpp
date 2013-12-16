#include "sprite.h"

using namespace std;

namespace evil {

void Sprite::update(const float dt)
{
		if( animation )
				animation->update(dt);
}

void Sprite::fill(vector<BatchVertex>& verts)
{
		const auto& f = ( animation ? animation->getFrame() : frame );

		const auto& textureRect = f->textureRect;

		BatchVertex v1,v2,v3,v4;

		// top left
		v1.u = textureRect.x, v1.v = textureRect.y;
		v1.x = position.x, v1.y = position.y;

		// top right
		v2.u = textureRect.x + textureRect.w, v2.v = textureRect.y;
		v2.x = position.x + f->size.w, v2.y = position.y;

		//bottom right
		v3.u = textureRect.x + textureRect.w, v3.v = textureRect.y + textureRect.h;
		v3.x = position.x + f->size.w, v3.y = position.y + f->size.h;

		// bottom left
		v4.u =  textureRect.x, v4.v = textureRect.y + textureRect.h;
		v4.x = position.x, v4.y = position.y + f->size.h;

		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);
		verts.push_back(v4);
}

void Sprite::render()
{
		const auto& f = ( animation ? animation->getFrame() : frame );

		const auto& textureRect = f->textureRect;

		BatchVertex v[4];

		v[0].x = position.x;
		v[0].y = position.y;
		v[0].u = textureRect.x;
		v[0].v = textureRect.y;

		v[1].x = position.x + f->size.w;
		v[1].y = position.y;
		v[1].u = textureRect.x + textureRect.w;
		v[1].v = textureRect.y;

		v[2].x = position.x + f->size.w;
		v[2].y = position.y + f->size.h;
		v[2].u = textureRect.x + textureRect.w;
		v[2].v = textureRect.y + textureRect.h;

		v[3].x = position.x;
		v[3].y = position.y + f->size.h;
		v[3].u = textureRect.x;
		v[3].v = textureRect.y + textureRect.h;


        GLfloat verts[ 2 * 4];
        GLfloat tex[ 2* 4];
    GLfloat colors[3 * 4 ];
        for( int i = 0 ; i< 4 ; ++i ) {
            verts[i*2] = v[i].x;
            verts[i*2+1] = v[i].y;
            tex[i*2] = v[i].u;
            tex[i*2+1] = v[i].v;
            colors[i*3] = 1.0f;

        }
  //
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, tex);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, verts);
        glDrawArrays(GL_TRIANGLES, 0, 4);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

    CHECK_GL();
/*
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
 */
}
}
