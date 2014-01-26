#include <assert.h>
#include <stdio.h>

#include "utils.h"
#include "uthash.h"
#include "evil.h"

#define MAX_STR_LEN 1024*16

/* undefine the defaults */
#undef uthash_malloc
#undef uthash_free

/* re-define, specifying alternate functions */
#define uthash_malloc(sz) ev_malloc(sz)
#define uthash_free(ptr,sz) ev_free(ptr)

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
		va_start(args, fmt);
		vfprintf(stdout, fmt, args);
		va_end(args);
}

void *ev_realloc(void *p, size_t size)
{
		return realloc(p,size);
}


void* ev_malloc(size_t size)
{
		void *p = malloc(size);

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
