#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "evil.h"
#include "test_helper.h"

#define ev_free test_free
#define ev_malloc test_malloc

#include "quad_tree.c"

static void simple_create_destroy(void **state)
{
    ev_qtree *q = ev_qtree_create(4, 32);

    ev_qtree_destroy(q, NULL);
}

static ev_qleaf* make_leaf(float x, float y)
{
    ev_qleaf *l = ev_malloc(sizeof(ev_qleaf));
    l->pos.x = x;
    l->pos.y = y;
    return l;
}

static void depth_one(void **state)
{
    ev_qtree *tree = ev_qtree_create(4, 32);

    assert_ev_ok(ev_qtree_add_leaf(tree, make_leaf(-8,-8)));
    assert_ev_ok(ev_qtree_add_leaf(tree, make_leaf( 8,-8)));
    assert_ev_ok(ev_qtree_add_leaf(tree, make_leaf( 8, 8)));
    assert_ev_ok(ev_qtree_add_leaf(tree, make_leaf(-8, 8)));

    assert_int_equal(4, tree->head.leaf_cnt);

    ev_qtree_destroy(tree, NULL);
}

static void split(void **state)
{
    ev_qtree *tree = ev_qtree_create(4,32);

    ev_qleaf *q1 = make_leaf(-8,-8);
    ev_qleaf *q2 = make_leaf(8, -8);
    ev_qleaf *q3 = make_leaf(8,8);
    ev_qleaf *q4 = make_leaf(-8,8);
    ev_qleaf *q5 = make_leaf(-7,-7);


    assert_ev_ok(ev_qtree_add_leaf(tree, q1));
    assert_ev_ok(ev_qtree_add_leaf(tree, q2));
    assert_ev_ok(ev_qtree_add_leaf(tree, q3));
    assert_ev_ok(ev_qtree_add_leaf(tree, q4));
    assert_ev_ok(ev_qtree_add_leaf(tree, q5));

    assert_int_equal(0, tree->head.leaf_cnt);
    assert_null(tree->head.leaves);

    assert_true(tree->head.children[EV_UL]->leaves[0] == q1);
    assert_true(tree->head.children[EV_UL]->leaves[1] == q5);
    assert_true(tree->head.children[EV_UR]->leaves[0] == q2);
    assert_true(tree->head.children[EV_LR]->leaves[0] == q3);
    assert_true(tree->head.children[EV_LL]->leaves[0] == q4);

    ev_qtree_destroy(tree, NULL);
}


static struct bounds_check_node {
    ev_qtree *tree;
    ev_qnode *node;

    struct bounds_check_node *next;
} *bounds_check = NULL;

static void bounds_walker(ev_qtree *tree, ev_qnode *node)
{
    struct bounds_check_node *b = ev_malloc(sizeof(*b));

    b->tree = tree;
    b->node = node;

    b->next = bounds_check;
    bounds_check = b;
}

static void free_bounds_check(void)
{
    struct bounds_check_node *b;

    while(bounds_check) {
        b = bounds_check;
        bounds_check = bounds_check->next;
        ev_free(b);
    }
}

static int count_bounds_check(void)
{
    int i = 0;
    struct bounds_check_node *b = bounds_check;

    while(b) {
        i++;
        b = b->next;
    }
    return i;
}

static int count_leaves_walked(void)
{
    int i = 0;
    struct bounds_check_node *b = bounds_check;
    while(b) {
        i += b->node->leaf_cnt;
        b = b->next;
    }
    return i;
}

static void walk_with_bounds(void **state)
{
    int i;
    int walk_count[5];
    ev_rect bounds;
    struct bounds_check_node *b;

    memset(walk_count, 0, sizeof(walk_count));

    ev_qtree *tree = ev_qtree_create(4,32);
    ev_qleaf *q1 = make_leaf(-8,-8);
    ev_qleaf *q2 = make_leaf(8, -8);
    ev_qleaf *q3 = make_leaf(8,8);
    ev_qleaf *q4 = make_leaf(-8,8);
    ev_qleaf *q5 = make_leaf(-7,-7);

    bounds_check = NULL;

    ev_qtree_add_leaf(tree, q1);
    ev_qtree_add_leaf(tree, q2);
    ev_qtree_add_leaf(tree, q3);
    ev_qtree_add_leaf(tree, q4);
    ev_qtree_add_leaf(tree, q5);

    ev_qtree_walk(tree, NULL, bounds_walker);

    /* we walked 4 nodes */
    assert_int_equal( 4, count_bounds_check());
    /* consisting of 5 leaves */
    assert_int_equal( 5, count_leaves_walked());

    b = bounds_check;

    while( b ) {
        for( i = 0 ; i < b->node->leaf_cnt ; ++i ) {
            if( q1 == b->node->leaves[i] ) {
                walk_count[0]++;
            } else if( q2 == b->node->leaves[i] ) {
                walk_count[1]++;
            } else if( q3 == b->node->leaves[i] ) {
                walk_count[2]++;
            } else if( q4 == b->node->leaves[i] ) {
                walk_count[3]++;
            } else if( q5 == b->node->leaves[i] ) {
                walk_count[4]++;
            }
        }
        b = b->next;
    }

    assert_int_equal(1, walk_count[0]);
    assert_int_equal(1, walk_count[1]);
    assert_int_equal(1, walk_count[2]);
    assert_int_equal(1, walk_count[3]);
    assert_int_equal(1, walk_count[4]);

    free_bounds_check();
    memset(walk_count, 0, sizeof(walk_count));

    bounds = ev_rect_create(-16, -16, 33, 33);
    ev_qtree_walk(tree, &bounds, bounds_walker);
    b = bounds_check;

    while( b ) {
        for( i = 0 ; i < b->node->leaf_cnt ; ++i ) {
            if( q1 == b->node->leaves[i] ) {
                walk_count[0]++;
            } else if( q2 == b->node->leaves[i] ) {
                walk_count[1]++;
            } else if( q3 == b->node->leaves[i] ) {
                walk_count[2]++;
            } else if( q4 == b->node->leaves[i] ) {
                walk_count[3]++;
            } else if( q5 == b->node->leaves[i] ) {
                walk_count[4]++;
            }
        }
        b = b->next;
    }

    assert_int_equal(1, count_bounds_check());
    assert_int_equal(2, count_leaves_walked());

    assert_int_equal(1, walk_count[0]);
    assert_int_equal(0, walk_count[1]);
    assert_int_equal(0, walk_count[2]);
    assert_int_equal(0, walk_count[3]);
    assert_int_equal(1, walk_count[4]);

    ev_qtree_destroy(tree, NULL);
    free_bounds_check();
}

static void node_in_viewport_test(void **state)
{
    ev_qnode qnode;
    ev_rect r;

    qnode.bounds = ev_rect_create(0,0,128,128);

    r = ev_rect_create(512,512,128,128);

    /* all the way out */
    assert_false(node_in_viewport(&qnode, &r));

    /* all the way in */
    r = ev_rect_create(8,8,32,32);
    assert_true(node_in_viewport(&qnode, &r));

    /* in between */
    r = ev_rect_create(63,63, 128,128);
    assert_true(node_in_viewport(&qnode, &r));

    /* NULL is always true */
    assert_true(node_in_viewport(&qnode, NULL));
}


int main(void)
{
    UnitTest tests[] = {
        unit_test(simple_create_destroy),
        unit_test(depth_one),
        unit_test(split),
        unit_test(node_in_viewport_test),
        unit_test(walk_with_bounds),
    };

    return run_tests(tests);
}
