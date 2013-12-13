#include "animation.h"

namespace evil {

void Animation::update(const float dt)
{
    time += dt;

    if( time >= delay ) {
        time -= delay;

        switch( mode ) {
        case LOOP:
            index += 1;
            if( index >= getNumFrames() ) {
                index = 0;
            }
            break;
        case ONE_SHOT:
            index += 1;
            if( index >= getNumFrames() ) {
                index = getNumFrames() - 1;
            }
            break;
        case REVERSE:
            if( index > 0 )
                index -= 1;
        }
    }
}

};
