#ifndef EV_SPRITE_H_
#define EV_SPRITE_H_

#include "evil.h"
#include "animation.h"
#include "sprite_batch.h"

#include <memory>

namespace evil {
class Sprite
{
public:
		void render();

		void setPosition(float x, float y) {
				position.x = x;
				position.y = y;
		}

		const Vector2& getPosition() const {
				return position;
		}
		void update(const float dt);

		void setAnimation(unique_ptr<Animation>& a) {
			animation = move(a);
		}

		void setFrame(shared_ptr<SpriteFrame>& f) { frame = f; }
private:
		shared_ptr<SpriteFrame> frame;
		unique_ptr<Animation> animation;
		Vector2 position;
};
}

#endif
