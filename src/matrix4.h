#ifndef EV_MATRIX_H_
#define EV_MATRIX_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
    float m[16];
    struct {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
} ev_matrix4;

EV_API void ev_matrix4_identity(ev_matrix4 *);
EV_API void ev_matrix4_set_ortho(ev_matrix4 *,
                          float left, float right, float bottom, float top,
                          float near, float far);

#ifdef __cplusplus
}
#endif

#endif
