#include <assert.h>

#include "quad_tree.h"

#define MAX_WIDTH 4096

static int quadrant_of_point(ev_vec2 *center, ev_vec2 *p)
{
    float dx = p->x - center->x;
    float dy = p->y - center->y;

    if( dx < 0 && dy < 0 ) {
        return UPPER_LEFT;
    } else if( dx > 0 && dy < 0 ) {
        return UPPER_RIGHT;
    } else if( dx > 0 && dy > 0 ) {
        return LOWER_RIGHT;
    } else {
        return LOWER_LEFT;
    }
}

void ev_quad_tree_init(ev_quad_tree *t, float dim, uint32_t leaves_per_node)
{
    assert( t != NULL );

    memset(t, 0, sizeof(ev_quad_tree));
}

static int in_viewport(ev_quad_tree *b, ev_rect *viewport)
{
    ev_rect r;

    r.origin.x = b->pos.x - b->dim/2;
    r.origin.y = b->pos.y - b->dim/2;
    r.size.w = b->dim;
    r.size.h = b->dim;

    return ev_rect_intersects(&r, viewport);
}

ev_quad_tree *ev_quad_at_point(ev_quad_tree* tree, ev_vec2 *v)
{
    int q;

    assert( v != NULL );

    for(;;) {
        if(!tree)
            return NULL;

        if(!tree->nodes)
            return tree;

        q = quadrant_of_point(&tree->pos, v);

        assert( q < 4 );

        tree = &tree->nodes[q];
    }
}

static void split_tree(ev_quad_tree *tree)
{
    float target_dim;
    unsigned i;

    assert( tree != NULL );

    tree->nodes = ev_malloc(sizeof(ev_quad_tree)*4);
    memset(tree->nodes, 0, sizeof(ev_quad_tree)*4);

    target_dim = tree->dim / 2;

    for( i = 0 ; i < 4 ; ++i ) {
        tree->nodes[i].dim = target_dim;
    }

    tree->nodes[UPPER_LEFT].pos.x = tree->pos.x - target_dim;
    tree->nodes[UPPER_LEFT].pos.y = tree->pos.y - target_dim;

    tree->nodes[UPPER_RIGHT].pos.x = tree->pos.x + target_dim;
    tree->nodes[UPPER_RIGHT].pos.y = tree->pos.y - target_dim;

    tree->nodes[LOWER_RIGHT].pos.x = tree->pos.x + target_dim;
    tree->nodes[LOWER_RIGHT].pos.y = tree->pos.y + target_dim;

    tree->nodes[LOWER_LEFT].pos.x = tree->pos.x - target_dim;
    tree->nodes[LOWER_LEFT].pos.y = tree->pos.y + target_dim;

    for( i = 0 ; i < tree->leaf_cnt ; ++i ) {
        int qq = quadrant_of_point(&tree->pos, &tree->leaves[i]->pos);
        ev_quad_tree_add(tree->nodes + qq, tree->leaves[i]);
        tree->leaves[i] = NULL;
    }
}

ev_quad_tree* ev_quad_tree_add(ev_quad_tree *blob, ev_quad_leaf *t)
{
    assert( t != NULL );

    for(;;) {
        assert( blob != NULL );

        int q = quadrant_of_point(&blob->pos, &t->pos);

        if( !blob->nodes ) {
            /* node hasn't been split yet */
            if( blob->leaf_cnt < MAX_PER_NODE ) {
                blob->leaves[blob->leaf_cnt++] = t;
                return blob;
            } else {
                split_tree(blob);

                return ev_quad_tree_add(blob->nodes + q, t);
            }
        } else {
            /* not in this one so look again in the subdivide */
            blob = &blob->nodes[q];
        }
    }
}
int ev_quad_foreach_in_viewport(ev_quad_tree *t, ev_rect *v, ev_quad_fn fn)
{
    uint32_t cnt = 0;

    if(!t) {
        return 0;
    }

    if( t->nodes ) {
        if( in_viewport(t, v ) ) {
            for( int i = 0 ; i < 4 ; ++i ) {
                cnt += ev_quad_foreach_in_viewport(&t->nodes[i], v, fn);
            }
        }
    } else {
        fn(t, t->leaves, t->leaf_cnt);
        cnt += t->leaf_cnt;
    }
    return cnt;
}
