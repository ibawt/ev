#ifndef EV_QUAD_TREE_H_
#define EV_QUAD_TREE_H_

#include "ev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    ev_vec2 pos;
    void   *data;
} ev_quad_leaf;

#define MAX_PER_NODE 4

typedef struct _ev_quad_tree {
    ev_quad_leaf *leaves[MAX_PER_NODE];
    uint32_t      leaf_cnt;
    uint32_t      leaves_per_node;

    float   dim;
    ev_vec2 pos;
    struct _ev_quad_tree *nodes;
} ev_quad_tree;

enum {
    UPPER_LEFT  = 0,
    UPPER_RIGHT = 2,
    LOWER_RIGHT = 4,
    LOWER_LEFT  = 8
};

typedef void (*ev_quad_fn)(ev_quad_tree *this, ev_quad_leaf **l, int num_leaves);

void          ev_quad_tree_init(ev_quad_tree *t, float dim, uint32_t leaves_per_node);
void          ev_quad_tree_destroy(ev_quad_tree *t);
ev_quad_tree *ev_quad_tree_add(ev_quad_tree *t, ev_quad_leaf *l);
int           ev_quad_foreach_in_viewport(ev_quad_tree *t, ev_rect *v, ev_quad_fn fn);
ev_quad_tree *ev_quad_at_point(ev_quad_tree *t, ev_vec2 *p);

#ifdef __cplusplus
}
#endif

#endif
