#include <assert.h>

#include "array_list.h"

ev_err_t ev_arraylist_init(ev_arraylist *al, size_t initial)
{
    assert(al);

    al->len = initial;
    al->top = 0;
    al->buff = ev_malloc(sizeof(void*)*initial);

    if( !al->buff ) {
        return EV_NOMEM;
    }

    return EV_OK;
}

static ev_err_t grow_list(ev_arraylist *al)
{
    void   *b;
    size_t  new_size = (al->len * 3) / 2;

    b = ev_malloc(new_size * sizeof(void*));
    if( !b )
        return EV_NOMEM;

    memcpy(b, al->buff, al->top * sizeof(void*));

    ev_free(al->buff);

    al->buff = b;

    al->len = new_size;

    return EV_OK;
}
ev_err_t ev_arraylist_push_ex(ev_arraylist *al, void *d)
{
    ev_err_t err;

    assert(al);

    if(al->top >= al->len) {
        err = grow_list(al);
        if(err) {
            return err;
        }
    }

    al->buff[al->top] = d;

    al->top++;

    return EV_OK;
}

void ev_arraylist_insert(ev_arraylist *al, size_t index, void *d)
{
    assert(index < (al->top+1));

    if( al->top >= al->len ) {
        grow_list(al);
    }

    memmove(al->buff + index + 1, al->buff + index, (al->top - index) * sizeof(void*));

    al->buff[index] = d;
    al->top++;
}

void ev_arraylist_remove(ev_arraylist *al, size_t index)
{
    memmove(al->buff + index, al->buff + index + 1, (al->top - index - 1)*sizeof(void*));
    al->top--;
}

ev_err_t ev_arraylist_reserve(ev_arraylist *al, size_t s)
{
    void **p = ev_malloc(s * sizeof(void*));
    if( !p ) {
        return EV_NOMEM;
    }

    memcpy(p, al->buff, (al->top-1) * sizeof(void*));

    ev_free(al->buff);

    al->buff = p;
    al->len = s;

    return EV_OK;
}
