#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "utils.h"
#include "uthash.h"
#include "evil.h"
#include "rect.h"

struct _ev_dir {
    DIR *dir;
};

ev_dir* ev_dir_open(const char *name)
{
    ev_dir *d = ev_malloc(sizeof(ev_dir));

    d->dir = opendir(name);
    if(!d->dir) {
        ev_free(d);
        return NULL;
    }
    return d;
}
void ev_dir_close(ev_dir *d)
{
    assert(d);

    if( d->dir ) {
        closedir(d->dir);
        d->dir = NULL;
    }
    ev_free(d);
}

const char *ev_dir_next_entry(ev_dir* d)
{
    struct dirent *entry;

    assert(d);
    if( !d->dir )
        return NULL;

    entry = readdir(d->dir);

    return entry ? entry->d_name : NULL;
}

#define MAX_STR_LEN 1024*16

/* undefine the defaults */
#undef uthash_malloc
#undef uthash_free

/* re-define, specifying alternate functions */
#define uthash_malloc(sz) ev_malloc(sz)
#define uthash_free(ptr,sz) ev_free(ptr)

void ev_timer_start(ev_timer *t)
{
    t->cnt = SDL_GetPerformanceCounter();
    t->ms = 0;
}

void ev_timer_end(ev_timer *t)
{
    t->cnt = SDL_GetPerformanceCounter() - t->cnt;
    t->ms = (float)(t->cnt / (double)SDL_GetPerformanceFrequency());
}

void ev_timer_end(ev_timer*);

typedef struct {
    char          *key;
    void          *value;
    UT_hash_handle hh;
} node;

struct _ev_smap
{
    node* head;
    ev_smap_delete deleter;
};

void ev_error(const char *fmt, ... )
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);
}

void ev_logger(ev_log_level level, const char *fmt, ... )
{
    va_list args;

    UNUSED(level);

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    fflush(stdout);
}

void *ev_realloc(void *p, size_t size)
{
    return realloc(p,size);
}

void* ev_malloc(size_t size)
{
    void *p = malloc(size);
    assert(p != NULL);
#ifndef NDEBUG
    memset(p, 0, size);
#endif

    return p;
}

void ev_free( void *p )
{
    assert( p != NULL);
    free(p);
}

ev_smap *ev_smap_create(void)
{
    ev_smap* smap = ev_malloc( sizeof(ev_smap));
    if( !smap )
        return NULL;

    smap->head = NULL;
    smap->deleter = NULL;

    return smap;
}

void ev_smap_set_delete(ev_smap* map, ev_smap_delete fn)
{
    if( map ) {
        map->deleter = fn;
    }
}

void *ev_smap_get(ev_smap* map, const char *key)
{
    node *node;
    if( !map || !key )
        return NULL;

    HASH_FIND_STR( map->head, key, node);

    if( !node )
        return NULL;

    return node->value;
}

ev_err_t ev_smap_put(ev_smap* map, const char *key, void *val)
{
    if( map && key ) {
        node *n = ev_malloc(sizeof(node));
        if( !n )
            return EV_FAIL;

        memset(n, 0, sizeof(node));
        n->key = strdup(key);
        n->value = val;
        HASH_ADD_KEYPTR( hh, map->head, n->key, strlen(n->key), n);
        return EV_OK;
    }
    return EV_FAIL;
}

void ev_smap_first(ev_smap *smap, ev_smap_iter *iter)
{
    if( smap && iter ) {
        iter->_node = smap->head;
    }
}

const char *ev_smap_iter_key( ev_smap_iter *i )
{
    if( i ) {
        return ((node*)i->_node)->key;
    }
    return NULL;
}

void *ev_smap_iter_value( ev_smap_iter *i)
{
    if( i ) {
        return ((node*)i->_node)->value;
    }
    return NULL;
}

int ev_smap_iter_next(ev_smap_iter *i)
{
    node* node;

    if( i ) {
        node = i->_node;
        if( node ) {
            i->_node = node->hh.next;
            return 1;
        }
    }
    return 0;
}

void ev_smap_destroy(ev_smap* map)
{
    node *node, *tmp;

    HASH_ITER( hh, map->head, node, tmp) {
        HASH_DEL( map->head, node );
        if( map->deleter ) {
            map->deleter(node->value);
        }
        ev_free(node->key);
        ev_free(node);
    }
}

char *ev_strdup(const char *src)
{
    if( src ) {
        size_t len = strnlen(src, MAX_STR_LEN);
        char *dst = ev_malloc( len + 1 );
        memcpy(dst, src, len);
        dst[len] = '\0';
        return dst;
    }
    return NULL;
}

void ev_log_bvertex(ev_bvertex *b)
{
    assert( b != NULL );

    ev_log("[x: %.2f, y: %.2f, u: %.2f, v: %.2f]", b->x, b->y, b->u, b->v );
    ev_log("[scale: %.2f, rotation: %.2f, tx: %.2f, ty: %.2f]", b->scale, b->rotation, b->tx, b->ty );
}

/**
 * O(n) watchout
 */
void  ev_pool_init(ev_pool *p, void *buff, size_t bytes, uint32_t length)
{
    uint32_t i;

    assert( p != NULL );
    assert( buff != NULL );

    p->buff = buff;

    *((void**)buff) = NULL;
    for( i = bytes ; i < (length * bytes) ; i += bytes) {
        void *pp = ((char *)buff) + i;
        *((void **)pp) = ((char*)buff) + (i - bytes);
    }

    p->buff = buff;
    p->current = ((char *)buff) + (length-1)*bytes;

#ifndef NDEBUG
    p->size = length;
    p->count = length;
#endif
}

void *ev_pool_alloc(ev_pool *p)
{
    void *pp;

    assert( p != NULL );
    assert( p->current != NULL );
    assert( p->count > 0 );

    pp = p->current;
    p->current = *((void **)pp);

#ifndef NDEBUG
    p->count--;
#endif

    return pp;
}

void ev_pool_free(ev_pool *p, void *t)
{
    assert( p != NULL );
    assert( t != NULL );
    assert( p->count < p->size );

    *((void **)t) = p->current;
    p->current = t;

#ifndef NDEBUG
    p->count++;
#endif
}

int ev_rand(void)
{
    return rand();
}

float ev_random_number(float min, float max)
{
    return min + ev_rand() / (RAND_MAX / (max - min + 1) + 1);
}

ev_vec2 ev_random_point(ev_rect *bounds)
{
    ev_vec2 p;

    assert( bounds != NULL );

    p.x = ev_random_number(EV_RECT_X1(bounds), EV_RECT_X2(bounds));
    p.y = ev_random_number(EV_RECT_Y1(bounds), EV_RECT_Y2(bounds));

    return p;
}
