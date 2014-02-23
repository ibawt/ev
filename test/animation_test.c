#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "evil.h"

#define ev_free test_free
#define ev_malloc test_malloc

#include "animation.c"

int ev_lua_create_ref(lua_State *l, int weak_ref)
{
    return 1;
}

void ev_lua_init_module(lua_State *l, luaL_Reg *funcs, const char *meta, const char *name)
{
}

static void create_test(void **state)
{
    ev_anim *a;

    (void)state;

    a = ev_anim_create();

    assert_int_equal( a->time, 0.0f);
    assert_int_equal( a->mode, EV_LOOP);
    assert_non_null(a->frames);
    assert_int_equal( a->frame_cnt, 0);
    assert_int_equal( a->delay, 0.3f);
    assert_int_equal( a->frame_size, FRAME_GROW_FAC);

    ev_anim_destroy(a);
}

static void grow_array_test(void **state)
{
    ev_anim *a = ev_anim_create();
    int vals[] = { 0xdeadbeef, 0xdeadbeef, 0xdeadbeef };

    memcpy(a->frames, vals, sizeof(int)*3);
    a->frame_cnt = 3;

    grow_array(a);

    assert_int_equal(a->frame_size, FRAME_GROW_FAC*2);

    assert_int_equal(a->frame_cnt, 3);

    assert_memory_equal(a->frames, vals, sizeof(int)*3);

    ev_anim_destroy(a);
}

int main(void)
{
    UnitTest tests[] = {
        unit_test(create_test),
        unit_test(grow_array_test)
    };

    return run_tests(tests);
}
