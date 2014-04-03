#include "matrix4.h"

#define M00  0
#define M01  4
#define M02  8
#define M03  12
#define M10  1
#define M11  5
#define M12  9
#define M13  13
#define M20  2
#define M21  6
#define M22  10
#define M23  14
#define M30  3
#define M31  7
#define M32  11
#define M33  15

void ev_matrix4_identity(ev_matrix4 *matrix)
{
    if( matrix ) {
        float *m = matrix->m;
        m[M00] = 1.0f;
        m[M10] = 0;
        m[M20] = 0;
        m[M30] = 0;
        m[M01] = 0;
        m[M11] = 1.0f;
        m[M21] = 0;
        m[M31] = 0;
        m[M02] = 0;
        m[M12] = 0;
        m[M22] = 1.0f;
        m[M32] = 0;
        m[M03] = 0;
        m[M13] = 0;
        m[M23] = 0;
        m[M33] = 1.0f;
    }
}
void ev_matrix4_set_ortho(ev_matrix4 *matrix, float left, float right, float bottom, float top,
                          float n, float f)
{
    if( matrix ) {
        float *m = matrix->m;

        float x_orth = 2 / ( right - left );
        float y_orth = 2 / ( top - bottom );
        float z_orth = -2 / (f - n );

        float tx = -(right + left) / ( right - left );
        float ty = -(top + bottom) / ( top - bottom );
        float tz = -(f + n ) / ( f - n );

        m[M00] = x_orth;
        m[M10] = 0;
        m[M20] = 0;
        m[M30] = 0;
        m[M01] = 0;
        m[M11] = y_orth;
        m[M21] = 0;
        m[M31] = 0;
        m[M02] = 0;
        m[M12] = 0;
        m[M22] = z_orth;
        m[M32] = 0;
        m[M03] = tx;
        m[M13] = ty;
        m[M23] = tz;
        m[M33] = 1.0f;
    }
}
