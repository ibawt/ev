#include "utils.h"
#include "uthash.h"
#include "evil.h"

typedef struct {
    const char    *key;
    void          *opaque;
    UT_hash_handle hh;
} node;

struct _ev_smap
{
    node* head;
    ev_smap_delete deleter;
};


void* ev_malloc(size_t size)
{
    return malloc(size);
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

    HASH_FIND_STR( map->map, key, &node);

    if( !node )
        return NULL;

    return node->opaque;
}

ev_err_t ev_smap_put(ev_smap* map, const char *key, void *val)
{
    if( map && key ) {
        node *node = ev_malloc(sizeof(node));
        if( !node )
            return EV_FAIL;

        node->key = strdup(key);
        node->value = val;
        HASH_ADD_KEYPTR( hh, map->head, node->key, strlen(key), node);
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
