#ifndef EV_ANIMATION_H_
#define EV_ANIMATION_H_

#include <memory>
#include <vector>

#include "evil.h"
#include "sprite_batch.h"

using namespace std;

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

		void addFrame(shared_ptr<SpriteFrame>& frame) {
				frames.push_back(frame);
		}

		Mode getMode() const {  return mode; }
		void setMode(Mode m) {
				mode = m;
		}

		void setDelay(float d) { delay = d; }
		float getDelay() const { return delay; }

		const shared_ptr<SpriteFrame>& getFrame() const {
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
		Mode mode;
		float time;
		float delay;
		unsigned index = 0;
		vector<shared_ptr<SpriteFrame>> frames;
};

}

#endif
