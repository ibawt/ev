#ifndef EV_VECTOR2_H_
#define EV_VECTOR2_H_

namespace evil {

struct Vector2
{
    Vector2() : x(0.0f), y(0.0f) { }
    Vector2( float xx, float yy) : x(xx), y(yy) { }

    float getLength() const {
        return sqrtf( x * x + y * y );
    }

    void normalize() {
      float length = getLength();
      if( length ) {
        x /= length;
        y /= length;
      }
    }

    float x;
    float y;
};

}

#endif
