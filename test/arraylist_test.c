#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "evil.h"
#include "test_helper.h"

#define ev_free test_free
#define ev_malloc test_malloc

#include "array_list.c"

void test_lifecycle(void **state)
{
    ev_arraylist al;

    ev_arraylist_init(&al, 10);

    ev_arraylist_destroy(&al);
}

void test_push_pop(void **state)
{
    int i;
    ev_arraylist al;
    ev_arraylist_init(&al, 5);

    for( i = 0 ; i < 10 ; ++i ) {
        ev_arraylist_push(&al, (void*)i);
    }

    assert_int_equal(10, ev_arraylist_len(&al));

    for( i = 9 ; i >= 0 ; --i) {
        assert_int_equal(i, (int)ev_arraylist_pop(&al));
    }

    assert_int_equal(NULL, ev_arraylist_pop(&al));

    assert_int_equal(0, ev_arraylist_len(&al));

    ev_arraylist_destroy(&al);
}

void test_insert_remove(void **state)
{
    int i;
    ev_arraylist al;
    int remove_1[] = { 0, 1, 2, 3, 4, 6, 7, 8, 9 };
    ev_arraylist_init(&al, 5);

    for( i = 0 ; i < 10 ; ++i ) {
        ev_arraylist_push(&al, (void*)i);
    }

    ev_arraylist_remove(&al, 5);
    assert_int_equal(9, ev_arraylist_len(&al));

    for( i = 9 ; i > 0 ; --i ) {
        int j = (int)ev_arraylist_pop(&al);
        assert_int_equal(remove_1[i-1], j);
    }

    ev_arraylist_insert(&al, 0, (void*)42);
    assert_int_equal(42, (int)ev_arraylist_pop(&al));

    ev_arraylist_push(&al, (void*)1);
    ev_arraylist_push(&al, (void*)3);

    ev_arraylist_insert(&al, 1, (void*)2);

    assert_int_equal(3, (int)ev_arraylist_pop(&al));
    assert_int_equal(2, (int)ev_arraylist_pop(&al));
    assert_int_equal(1, (int)ev_arraylist_pop(&al));

    ev_arraylist_destroy(&al);
}

static void test_clear(void **state)
{
    int i;
    ev_arraylist al;

    ev_arraylist_init(&al, 5);
    for( i = 0 ; i < 5 ; ++i ) {
        ev_arraylist_push(&al, (void*)5);
    }

    ev_arraylist_clear(&al);
    assert_int_equal(0, ev_arraylist_len(&al));

    ev_arraylist_destroy(&al);
}

static void test_reserve(void **state)
{
    ev_arraylist al;
    ev_arraylist_init(&al, 5);

    ev_arraylist_push(&al, (void*)5);
    ev_arraylist_push(&al, (void*)5);
    ev_arraylist_push(&al, (void*)5);

    ev_arraylist_reserve(&al, 512);

    assert_int_equal(3, ev_arraylist_len(&al));
    assert_int_equal(5, (int)ev_arraylist_pop(&al));
    assert_int_equal(5, (int)ev_arraylist_pop(&al));
    assert_int_equal(5, (int)ev_arraylist_pop(&al));

    ev_arraylist_destroy(&al);
}

int main(void)
{
    UnitTest tests[] = {
        unit_test(test_lifecycle),
        unit_test(test_push_pop),
        unit_test(test_insert_remove),
        unit_test(test_clear),
    };

    run_tests(tests);
}
