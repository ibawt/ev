#ifndef EV_UTILS_H_
#define EV_UTILS_H_

#include <stdlib.h>
#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void   *elems;
    size_t  cnt;
    size_t  size;
} ev_array;

void ev_array_create(ev_array *);
void ev_array_push(ev_array *a, void *o);
void ev_array_destroy(ev_array *a);

typedef struct _ev_smap ev_smap;
typedef void (*ev_smap_delete)(void*);

typedef struct {
    void *_node;
} ev_smap_iter;

ev_smap *ev_smap_create(void);
void     ev_smap_destroy(ev_smap*);
void     ev_smap_set_delete(ev_smap*, ev_smap_delete);
void*    ev_smap_get(ev_smap *, const char *);
ev_err_t ev_smap_put(ev_smap *, const char *key, void *opaque);

#define ev_smap_iter_valid( _s ) ( (_s)._node != NULL)

void ev_smap_first(ev_smap *smap, ev_smap_iter *iter);
const char* ev_smap_iter_key(ev_smap_iter *k);
int         ev_smap_iter_next(ev_smap_iter *i);
void ev_log_bvertex(ev_bvertex *b);

#ifdef __cplusplus
}
#endif

#endif
