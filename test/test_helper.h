#ifndef TEST_HELPER_H_
#define TEST_HELPER_H_

#include <cmocka.h>
#include "evil.h"

#define assert_ev_ok(f) assert_int_equal(EV_OK, f)

#endif
