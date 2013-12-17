#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include "texture.h"
#include "rect.h"
#include "vector2.h"
#include "shader.h"
#include "evil.h"
#include "matrix4.h"

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace evil {

class Sprite;

struct SpriteFrame
{
    std::string key;
    std::vector<std::string> aliases;
    Rect sourceSize;
    Rect size;
    bool trimmed;
    Rect textureRect;
    Vector2 offset;
    bool rotated;
    Rect colorRect;
};

struct SpriteSheet
{
    std::unordered_map<std::string,std::shared_ptr<SpriteFrame>> frames;
    std::unordered_map<std::string, std::string> metadata;
};

struct BatchVertex
{
    float x;
    float y;
    float u;
    float v;
};

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

private:
    Matrix4 transform;
    ShaderProgram program;
    GLuint vboID;
    std::vector<std::shared_ptr<Sprite>> sprites;
    std::vector<BatchVertex> verts;
    SpriteSheet sheet;
    std::shared_ptr<Texture> texture;
};


}

#endif
