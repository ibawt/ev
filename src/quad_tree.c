#include <assert.h>

#include "quad_tree.h"

static int quadrant_of_point(ev_vec2 *center, ev_vec2 *p)
{
    float dx = p->x - center->x;
    float dy = p->y - center->y;

    if( dx < 0 && dy < 0 ) {
        return EV_UL;
    } else if( dx > 0 && dy < 0 ) {
        return EV_UR;
    } else if( dx > 0 && dy > 0 ) {
        return EV_LR;
    } else {
        return EV_LL;
    }
}

ev_qtree* ev_qtree_create(int leaves_per_node, float dim)
{
    ev_qtree *qtree = ev_malloc(sizeof(ev_qtree));

    if( qtree ) {
        memset(qtree, 0, sizeof(ev_qtree));

        qtree->head.bounds.size.w = dim;
        qtree->head.bounds.size.h = dim;
        qtree->head.leaves = ev_malloc(sizeof(ev_qleaf)*leaves_per_node);
        if( !qtree->head.leaves) {
            ev_free(qtree);
            return NULL;
        }
        memset(qtree->head.leaves, 0, sizeof(ev_qleaf)*leaves_per_node);

        qtree->dim = dim;
        qtree->leaves_per_node = leaves_per_node;
    }
    return qtree;
}

/** TODO make this not recursive */
static void destroy_node(ev_qtree *tree, ev_qnode* node, ev_qtree_fn fn)
{
    int i;

    assert( tree != NULL );
    assert( node != NULL );

    if( fn ) {
        fn(tree, node);
    }

    if( node->leaves ) {
        if(!fn) {
            /* default delete function if nothing specified */
            for(i = 0 ; i < node->leaf_cnt ; ++i) {
                ev_free(node->leaves[i]);
            }
        }
        ev_free(node->leaves);
    }

    for(i = 0 ; i < 4 ; ++i) {
        if( node->children[i] ) {
            destroy_node(tree, node->children[i], fn);
            ev_free(node->children[i]);
        }
    }
}

void ev_qtree_destroy(ev_qtree *tree, ev_qtree_fn fn)
{
    assert(tree != NULL);

    destroy_node(tree, &tree->head, fn);
    ev_free(tree);
}

static int in_viewport(ev_qnode *b, ev_rect *viewport)
{
    return ev_rect_intersects(&b->bounds, viewport);
}

static ev_err_t split_node(ev_qtree *tree, ev_qnode *node)
{
    float target_dim;
    int i;

    assert(node!= NULL );

    target_dim = node->bounds.size.w / 2;

    assert(target_dim >= 1.0f );

    for(i = 0 ; i < 4 ; ++i ) {
        ev_qnode *n = ev_malloc(sizeof(ev_qnode));
        memset(n, 0, sizeof(ev_qnode));
        node->children[i] = n;

        n->leaves = ev_malloc(sizeof(ev_qleaf*)*tree->leaves_per_node);
        if(!n->leaves) {
            return EV_NOMEM;
        }
        memset(n->leaves, 0, sizeof(ev_qleaf*)*tree->leaves_per_node);

        n->bounds.size.w = target_dim;
        n->bounds.size.h = target_dim;
    }

    node->children[EV_UL]->bounds.origin.x = node->bounds.origin.x - target_dim;
    node->children[EV_UL]->bounds.origin.y = node->bounds.origin.y - target_dim;

    node->children[EV_UR]->bounds.origin.x = node->bounds.origin.x + target_dim;
    node->children[EV_UR]->bounds.origin.y = node->bounds.origin.y - target_dim;

    node->children[EV_LR]->bounds.origin.x = node->bounds.origin.x + target_dim;
    node->children[EV_LR]->bounds.origin.y = node->bounds.origin.y + target_dim;

    node->children[EV_LL]->bounds.origin.x = node->bounds.origin.x - target_dim;
    node->children[EV_LL]->bounds.origin.y = node->bounds.origin.y + target_dim;

    for(i = 0 ; i < node->leaf_cnt ; ++i ) {
        int qq = quadrant_of_point(&node->bounds.origin, &node->leaves[i]->pos);

        node->children[qq]->leaves[node->children[qq]->leaf_cnt++] = node->leaves[i];
    }
    ev_free(node->leaves);

    node->leaves = NULL;
    node->leaf_cnt = 0;

    return EV_OK;
}

ev_err_t ev_qtree_remove_leaf(ev_qtree *tree, ev_qleaf *t)
{
    return EV_FAIL;
}

ev_err_t ev_qtree_add_leaf(ev_qtree *tree, ev_qleaf *t)
{
    ev_qnode *node;

    assert( tree != NULL );
    assert( t != NULL );

    node = &tree->head;

    while(node) {
        int q = quadrant_of_point(&node->bounds.origin, &t->pos);

        if( node->leaves ) {
            ev_err_t err;
            /* node hasn't been split yet */
            if( node->leaf_cnt < tree->leaves_per_node ) {
                node->leaves[node->leaf_cnt++] = t;
                return EV_OK;
            }

            if( node->bounds.size.w < 1.0f) {
                return EV_FAIL;
            }

            if( (err = split_node(tree, node)) ) {
                return err;
            }
        }

        node = node->children[q];
    }

    return EV_FAIL;
}

static int node_in_viewport(ev_qnode *node, ev_rect *bounds)
{
    if(!bounds) {
        return 1;
    }
    return ev_rect_intersects(&node->bounds, bounds);
}

static int node_walk(ev_qtree *tree, ev_qnode *node, ev_rect *bounds, ev_qtree_fn fn)
{
    int items = 0;

    if(node && node_in_viewport(node, bounds)) {
        int i;

        if( node->leaves ) {
            fn(tree, node);
            items += node->leaf_cnt;
        }
        for( i = 0 ; i < 4 ; ++i ) {
            items += node_walk(tree, node->children[i], bounds, fn);
        }
    }
    return items;
}

int ev_qtree_walk(ev_qtree *tree, ev_rect *bounds, ev_qtree_fn fn)
{
    assert( tree != NULL );
    assert( fn != NULL );

    return node_walk(tree, &tree->head, bounds, fn);
}
