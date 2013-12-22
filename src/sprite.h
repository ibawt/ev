#ifndef EV_SPRITE_H_
#define EV_SPRITE_H_

#include "vector2.h"

#include <memory>

namespace evil {

class Animation;
struct SpriteFrame;
struct BatchVertex;

class Sprite
{
public:
    static const uint32_t NUM_VERTS = 6;

    Sprite() : rotation(0.0f),
               scale(1.0f)
        { }

    void render();

    void setPosition(float x, float y) {
        position.x = x;
        position.y = y;
    }

    float getRotation() const { return rotation; }
    void setRotation(float r) { rotation = r; }

    float getScale() const { return scale; }
    void setScale(float f) { scale = f; }
    const Vector2& getPosition() const {
        return position;
    }
    void update(const float dt);

    const std::shared_ptr<SpriteFrame>& getCurrentFrame() const;

    void fillBuffer(BatchVertex *bv);

    void setAnimation(std::unique_ptr<Animation>& a) {
      animation = move(a);
    }
    void setFrame(std::shared_ptr<SpriteFrame>& f) { frame = f; }
private:
    std::shared_ptr<SpriteFrame> frame;
    std::unique_ptr<Animation> animation;
    Vector2 position;
    float   rotation;
    float   scale;
};
}

#endif
