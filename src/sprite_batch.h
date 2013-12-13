#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include "evil.h"
#include "texture.h"

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

namespace evil {

class Sprite;

struct SpriteFrame
{
    string key;
    vector<string> aliases;
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
    unordered_map<string,shared_ptr<SpriteFrame>> frames;
    unordered_map<string, string> metadata;
};

class SpriteBatch
{
public:
    bool load(const string& json);
    void render();
    void setTexture(shared_ptr<Texture>& t);
    shared_ptr<Sprite> get(const string& name);

    const SpriteSheet& getSheet() const { return sheet; }
private:
    SpriteSheet sheet;
    shared_ptr<Texture> texture;
};


}

#endif
