#ifndef EV_ANIMATION_H_
#define EV_ANIMATION_H_

#include <memory>
#include <vector>

#include "evil.h"
#include "sprite_batch.h"

namespace evil {

class Animation
{
public:
		typedef enum  {
				LOOP,
				ONE_SHOT,
				REVERSE
		} Mode;
		unsigned getNumFrames() const {
				return frames.size();
		}

		void addFrame(std::shared_ptr<SpriteFrame>& frame) {
				frames.push_back(frame);
		}

		Mode getMode() const {  return mode; }
		void setMode(Mode m) {
				mode = m;
		}

		void setDelay(float d) { delay = d; }
		float getDelay() const { return delay; }

		const std::shared_ptr<SpriteFrame>& getFrame() const {
				return frames[index];
		}
		void update(const float dt);

		void setFrameIndex(unsigned i) {
				if( i >= getNumFrames() ) {
						i = getNumFrames() - 1;
				}
				index = i;
		}

		unsigned getFrameIndex() const {
				return index;
		}
private:
		Mode mode = LOOP;
		float time = 0.0f;
		float delay = 0.0f;
		unsigned index = 0;
    std::vector<std::shared_ptr<SpriteFrame>> frames;
};

}

#endif
