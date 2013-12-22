#ifndef EV_SPRITE_SHEET_H_
#define EV_SPRITE_SHEET_H_

#include <array>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "animation.h"
#include "sprite.h"
#include "vector2.h"
#include "rect.h"

struct json_t;

namespace evil
{
struct BatchVertex
{
    float x;
    float y;
    float u;
    float v;
    float rotation;
    float scale;
    float tx;
    float ty;
};

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

    std::array<BatchVertex, Sprite::NUM_VERTS> verts;
};

class SpriteSheet
{
public:
    bool load(const std::string& name);
    std::shared_ptr<SpriteFrame> getFrame(const std::string& name)  {
        return frames[name];
    }
    size_t getNumFrames() const { return frames.size(); }
private:
    void fillBuffer(std::shared_ptr<SpriteFrame> frame);
    void normalizeTextureRect(Rect&r);
    static bool parseSize(json_t*, const std::string& key, Rect& r);
    static bool parseVector(json_t*, const std::string& key, Vector2& v);
    static bool parseRect(json_t*, const std::string& key, Rect& r);

    Rect textureSize;

    std::map<std::string, std::shared_ptr<SpriteFrame>> frames;
    std::map<std::string, std::string> metadata;
};

}

#endif
