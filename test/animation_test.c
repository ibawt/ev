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

static void destroy_test(void **state)
{
    /* make sure it doesn't die on NULL */
    ev_anim_destroy(NULL);
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

static void update_test(void **state)
{
    ev_anim *a = ev_anim_create();

    /* dengerate case */
    a->delay = 0.0f;
    a->time = 0;

    ev_anim_update(a, 1);

    assert_true(ev_cmpf(a->time, 0));

    /* loop with only one frame */
    a->delay = 1.0f;
    a->mode = EV_LOOP;
    a->frame_cnt = 1;

    ev_anim_update(a,1);
    assert_int_equal(a->index, 0);
    assert_true(ev_cmpf(a->time, 0));

    /* loop with multiple frames */
    a->frame_cnt = 5;
    a->time = 0;

    ev_anim_update(a, 1);
    assert_int_equal(a->index, 1);

    ev_anim_update(a, 4);
    assert_int_equal(a->index, 0);

    /* one shot */
    a->time = 0;
    a->mode = EV_ONE_SHOT;
    a->index = 0;

    ev_anim_update(a, 5);
    assert_int_equal(a->index, 4);

    /* reverse */
    a->mode = EV_REVERSE;
    ev_anim_update(a, 1);
    assert_int_equal(a->index, 3);

    ev_anim_destroy(a);
}

static void add_sframe_test(void **state)
{
    ev_anim *a = ev_anim_create();
    ev_sframe* v = (ev_sframe*) 0xdeadbeef;
    int i;
    /* add an sframe and increment the frame count */
    ev_anim_add_sframe(a, v);

    assert_int_equal(a->frame_cnt, 1);
    assert_int_equal(a->frames[0], v);

    for( i = 0 ; i < FRAME_GROW_FAC ; ++i ) {
        ev_anim_add_sframe(a, v);
    }

    assert_int_equal(a->frame_size, FRAME_GROW_FAC*2);

    ev_anim_destroy(a);
}

int main(void)
{
    UnitTest tests[] = {
        unit_test(create_test),
        unit_test(destroy_test),
        unit_test(grow_array_test),
        unit_test(update_test),
        unit_test(add_sframe_test)
    };

    return run_tests(tests);
}
