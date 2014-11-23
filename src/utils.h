#ifndef EV_UTILS_H_
#define EV_UTILS_H_

#include <stdlib.h>
#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t cnt;
    float    ms;
} ev_timer;

void ev_timer_start(ev_timer*);
void ev_timer_end(ev_timer*);

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


typedef struct {
    void    *buff;
    void    *current;
#ifndef NDEBUG
    uint32_t size;
    uint32_t count;
#endif
} ev_pool;

void  ev_pool_init(ev_pool *p, void *buff, size_t bytes, uint32_t length);
void *ev_pool_alloc(ev_pool *p);
void  ev_pool_free(ev_pool *p, void *t);

int     ev_rand(void);
float   ev_random_number(float min, float max);
ev_vec2 ev_random_point(ev_rect *bounds);

#ifdef __cplusplus
}
#endif

#endif
