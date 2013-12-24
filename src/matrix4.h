#ifndef EV_MATRIX_H_
#define EV_MATRIX_H_

typedef struct {
    float m[16];
} ev_matrix4;

void ev_matrix4_identity(ev_matrix4 *);
void ev_matrix4_set_ortho(ev_matrix4 *, float left, float right, float bottom, float top,
                          float near, float far);

#endif
