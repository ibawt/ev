#ifndef EV_MATRIX_H_
#define EV_MATRIX_H_

namespace evil {

class Matrix4
{
public:
    Matrix4() {
        setIdentity();
    }
    void setIdentity();
    void setOrtho(float left, float right, float bottom,
                  float top, float near, float far);
    float m[16];
};
}

#endif
