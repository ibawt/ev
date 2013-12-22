#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include "texture.h"
#include "rect.h"
#include "vector2.h"
#include "shader.h"
#include "evil.h"
#include "matrix4.h"
#include "vertex_buffer.h"
#include "sprite_sheet.h"

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <array>

namespace evil {

class Sprite;

class SpriteBatch
{
public:
    SpriteBatch();

    bool load(const std::string& json);
    void render();
    void setTexture(std::shared_ptr<Texture>& t);
    std::shared_ptr<Sprite> get(const std::string& name);

    void addSprite(std::shared_ptr<Sprite>&sprite) { sprites.push_back(sprite); }
    unsigned getNumSprites() const { return sprites.size() ; }

    void generateBuffer();
    SpriteSheet& getSheet() { return sheet; }

    void setTransform(Matrix4 t) {
        transform = t;
    }

    void update(const float dt);
private:
    void fillBuffer(std::shared_ptr<SpriteFrame>& s);
    void fillVertexBuffer();

    VertexBuffer<BatchVertex> vertexBuffer;

    Matrix4 transform;
    ShaderProgram program;

    std::vector<std::shared_ptr<Sprite>> sprites;
    SpriteSheet sheet;

    std::shared_ptr<Texture> texture;
};


}

#endif
