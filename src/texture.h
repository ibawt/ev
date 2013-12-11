#ifndef EV_TEXTURE_H_
#define EV_TEXTURE_H_

#include "evil.h"

#include <string>

namespace evil {
	
class Texture
{
public:
    Texture() : textureID(0) { }
    ~Texture();

    void bind() const;
    void dispose();
    
    bool load(std::string filename);
    
private:
    GLuint textureID;
};

}

#endif
