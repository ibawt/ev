#ifndef EV_QUAD_TREE_H_
#define EV_QUAD_TREE_H_

#include "ev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    ev_vec2 pos;
    void   *data; /* user data */
} ev_qleaf;

typedef struct _ev_qnode {
    ev_qleaf  **leaves; /* if it has children this will be NULL */
    int         leaf_cnt;
    ev_rect     bounds;

    struct _ev_qnode *children[4];
} ev_qnode;

typedef struct {
    int       leaves_per_node;
    float     dim;
    ev_qnode  head;
    int       count;
} ev_qtree;

/* The 4 quardrants  UL == Upper Left */
enum {
    EV_UL  = 0,
    EV_UR,
    EV_LR,
    EV_LL
};

typedef void (*ev_qtree_fn)(ev_qtree *tree, ev_qnode *node);

ev_qtree* ev_qtree_create(int leaves_per_node, float dim);
void      ev_qtree_destroy(ev_qtree *tree, ev_qtree_fn fn);

ev_err_t ev_qtree_add_leaf(ev_qtree *qtree, ev_qleaf *leaf);
ev_err_t ev_qtree_remove_leaf(ev_qtree *qtree, ev_qleaf *leaf);

int      ev_qtree_walk(ev_qtree *qtree, ev_rect *bounds, ev_qtree_fn fn);

#ifdef __cplusplus
}
#endif

#endif
