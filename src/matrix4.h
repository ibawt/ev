#ifndef EV_MATRIX_H_
#define EV_MATRIX_H_

namespace evil {

class Matrix4
{
public:
    float m[16];
    void setOrtho(float left, float right, float bottom,
                  float top, float near, float far);
};
}

#endif
