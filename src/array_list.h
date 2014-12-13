#ifndef EV_ARRAY_LIST_H_
#define EV_ARRAY_LIST_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Shitty array list for size < a pointer
 * TODO:
 *  - allow for value types (like ev_vec2)
 *  - benchmark
 */
typedef struct {
    size_t   len;
    size_t   top;
    void   **buff;
} ev_arraylist;

#define  ev_arraylist_len(_p) (_p)->top

ev_err_t ev_arraylist_init(ev_arraylist *al, size_t initial);
#define  ev_arraylist_destroy(_p) ev_free((_p)->buff)

ev_err_t ev_arraylist_push_ex(ev_arraylist *al, void *d);

#define  ev_arraylist_push(_p, _d) do { if((_p)->top >= (_p)->len) {      \
            ev_arraylist_push_ex((_p),(_d));                            \
        } else { (_p)->buff[(_p)->top++] = (_d); } } while(0)

#define  ev_arraylist_pop(_p) ( (_p)->top ? (_p)->buff[(_p)->top-- - 1] : NULL )

void     ev_arraylist_insert(ev_arraylist *al, size_t index, void *d);
void     ev_arraylist_remove(ev_arraylist *al, size_t index);

#define  ev_arraylist_clear(_p) (_p)->top = 0

ev_err_t ev_arraylist_reserve(ev_arraylist *al, size_t amt);

#ifdef __cplusplus
}
#endif

#endif
