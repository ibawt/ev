#ifndef EV_TEXTURE_H_
#define EV_TEXTURE_H_

#include "evil.h"

#include <string>

namespace evil {

class Texture
{
public:
		Texture();
		~Texture();

		void bind() const;
		void dispose();

		int getWidth() const {
			return width;
		}
		int getHeight() const {
			return height;
		}

		bool load(const std::string& filename);
private:
		int width;
		int height;
		GLuint textureID;
};

}

#endif
