#ifndef EV_MATRIX_H_
#define EV_MATRIX_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float m[16];
} ev_matrix4;

void ev_matrix4_identity(ev_matrix4 *);
void ev_matrix4_set_ortho(ev_matrix4 *,
                          float left, float right, float bottom, float top,
                          float near, float far);

#ifdef __cplusplus
}
#endif

#endif
