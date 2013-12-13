#include "sprite.h"

namespace evil {

void Sprite::render()
{
		if( animation ) {
				;
		}
		else {
				const auto& textureRect = frame->textureRect;
				glBegin(GL_QUADS);
				// top left
				glTexCoord2f( textureRect.x, textureRect.y );
				glVertex2f( position.x, position.y );

				// top right
				glTexCoord2f( textureRect.x + textureRect.w, textureRect.y );
				glVertex2f( position.x + frame->size.w, position.y );

				//bottom right
				glTexCoord2f( textureRect.x + textureRect.w, textureRect.y + textureRect.h);
				glVertex2f( position.x + frame->size.w, position.y + frame->size.h );

				// bottom left
				glTexCoord2f( textureRect.x, textureRect.y + textureRect.h );
				glVertex2f( position.x, position.y + frame->size.h);
				glEnd();
		}
}
}
