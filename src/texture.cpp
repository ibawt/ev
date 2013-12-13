#include "evil.h"
#include "texture.h"
#include "utils.h"
#include <stdio.h>

using namespace std;

namespace evil {

Texture::Texture() : textureID(0)
{
}


Texture::~Texture()
{
    dispose();
}    

void Texture::bind() const
{
    if( textureID != 0 ) {
        glBindTexture(GL_TEXTURE_2D, textureID );
    }
}


void Texture::dispose()
{
    if( textureID != 0 ) {
        glDeleteTextures(1,&textureID);
        textureID = 0;
    }
}

bool Texture::load(const string& filename)
{
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if( !surface ) {
        error("Can't load texture %s: %s", filename.c_str(), SDL_GetError());
        return false;
    }
    auto defer = MakeScopeExit([surface] { SDL_FreeSurface(surface); } );
    
    glGenTextures(1, &textureID );
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

    width = surface->w;
    height = surface->h;
    
    return true;
}

}
