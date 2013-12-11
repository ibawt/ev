#include "texture.h"
#include "jansson.h"

#include <stdio.h>

using namespace std;

namespace evil {

Texture::~Texture()
{
}    

void Texture::bind() const
{
}


void Texture::dispose()
{
}

bool Texture::load(string filename)
{
    return true;
}

}
