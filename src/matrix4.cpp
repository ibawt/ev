#include "matrix4.h"

namespace evil {
static const int M00 = 0;// 0;
static const int M01 = 4;// 1;
static const int M02 = 8;// 2;
static const int M03 = 12;// 3;
static const int M10 = 1;// 4;
static const int M11 = 5;// 5;
static const int M12 = 9;// 6;
static const int M13 = 13;// 7;
static const int M20 = 2;// 8;
static const int M21 = 6;// 9;
static const int M22 = 10;// 10;
static const int M23 = 14;// 11;
static const int M30 = 3;// 12;
static const int M31 = 7;// 13;
static const int M32 = 11;// 14;
static const int M33 = 15;// 15;

void Matrix4::setIdentity()
{
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

void Matrix4::setOrtho(float left, float right, float bottom,
                       float top, float near, float far)
{
    float x_orth = 2 / ( right - left );
    float y_orth = 2 / ( top - bottom );
    float z_orth = -2 / (far - near );

    float tx = -(right + left) / ( right - left );
    float ty = -(top + bottom) / ( top - bottom );
    float tz = -(far + near ) / ( far - near );

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
