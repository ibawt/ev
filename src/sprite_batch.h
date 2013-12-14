#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include "evil.h"
#include "texture.h"
#include "rect.h"
#include "vector2.h"

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

class SpriteBatch
{
public:
    bool load(const std::string& json);
    void render();
    void setTexture(std::shared_ptr<Texture>& t);
    std::shared_ptr<Sprite> get(const std::string& name);

    SpriteSheet& getSheet() { return sheet; }
private:
    SpriteSheet sheet;
    std::shared_ptr<Texture> texture;
};


}

#endif
