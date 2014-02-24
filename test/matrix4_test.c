#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "matrix4.c"

#include "evil.h"

#define assert_float_equals(x,y) assert_true( ev_cmpf(x,y))

static void identity_test(void **state)
{
    (void) state;

    ev_matrix4 matrix;

    ev_matrix4_identity(&matrix);

    assert_float_equals(matrix.m[M00], 1.0f);
    assert_float_equals(matrix.m[M10], 0);
    assert_float_equals(matrix.m[M20], 0);
    assert_float_equals(matrix.m[M30], 0);


    assert_float_equals(matrix.m[M01], 0);
    assert_float_equals(matrix.m[M11], 1.0f);
    assert_float_equals(matrix.m[M21], 0);
    assert_float_equals(matrix.m[M31], 0);

    assert_float_equals(matrix.m[M02], 0);
    assert_float_equals(matrix.m[M12], 0);
    assert_float_equals(matrix.m[M22], 1.0f);
    assert_float_equals(matrix.m[M32], 0);

    assert_float_equals(matrix.m[M03], 0);
    assert_float_equals(matrix.m[M13], 0);
    assert_float_equals(matrix.m[M23], 0);
    assert_float_equals(matrix.m[M33], 1.0f);
}

static void set_ortho_test(void **state)
{
    const float ortho[] = { 0.002500,0.000000,0.000000,0.000000,
                      0.000000,-0.003339,0.000000,0.000000,
                      0.000000,0.000000,-1.000000,0.000000,
                      -1.000000,1.003339,-0.000000,1.000000 };
    int i;
    ev_matrix4 matrix;

    ev_matrix4_set_ortho(&matrix, 0, 800, 600, 1, -1, 1);

    for( i = 0 ; i < 16 ; ++i ) {
        assert_float_equals(matrix.m[i], ortho[i]);
    }
}

int main(void)
{
    const UnitTest tests[] = {
        unit_test(identity_test),
        unit_test(set_ortho_test),
    };

    return run_tests(tests);
}
