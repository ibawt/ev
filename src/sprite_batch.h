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
#include <map>

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
    std::map<std::string,std::shared_ptr<SpriteFrame>> frames;
    std::map<std::string, std::string> metadata;
};

struct UniformFrame
{
    float top_left[2];
    float top_right[2];
    float bottom_right[2];
    float bottom_left[2];
    float width;
    float height;
};

struct BatchVertex
{
    float x;
    float y;
    float corner;
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
    void fillUniformFrames();
    float time;
    Matrix4 transform;
    ShaderProgram program;
    GLuint vboID;
    std::vector<std::shared_ptr<Sprite>> sprites;
    std::vector<BatchVertex> verts;
    SpriteSheet sheet;
    std::shared_ptr<Texture> texture;

    std::vector<UniformFrame> texFrames;
    std::map<std::string, uint16_t> texMap;
};


}

#endif
