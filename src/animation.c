#include <assert.h>
#include "ev_lua.h"
#include "animation.h"

#define FRAME_GROW_FAC 16

static void grow_array(ev_anim *a)
{
    void *p;

    assert( a != NULL );

    p = a->frames;
    a->frames = ev_malloc(sizeof(ev_sframe*)*(a->frame_size + FRAME_GROW_FAC));
    a->frame_size += FRAME_GROW_FAC;
    memcpy(a->frames, p, sizeof(ev_sframe*)*a->frame_cnt);

    ev_free(p);
}

static void init_ev_anim(ev_anim* a)
{
    assert( a != NULL );

    a->time = 0.0f;
    a->mode = EV_LOOP;
    a->frames = ev_malloc(sizeof(ev_sframe*)*FRAME_GROW_FAC);
    memset(a->frames, 0, sizeof(ev_sframe*)*FRAME_GROW_FAC);
    a->frame_cnt = 0;
    a->delay = 0.3f;
    a->frame_size = FRAME_GROW_FAC;
    a->index = 0;
}

ev_anim* ev_anim_create(void)
{
    ev_anim * a = ev_malloc(sizeof(ev_anim));
    if( !a )
        return NULL;

    init_ev_anim(a);

    return a;
}

void ev_anim_destroy(ev_anim *a)
{
    if( a ) {
        ev_free(a->frames);
        a->frames = NULL;
        ev_free(a);
    }
}

void ev_anim_update(ev_anim *a, float dt)
{
    if( a && a->delay > 0.0f ) {
        a->time += dt;

        while( a->time >= a->delay ) {
            a->time -= a->delay;
            switch( a->mode ) {
            case EV_LOOP:
                a->index++;
                if( a->index >= a->frame_cnt ) {
                    a->index = 0;
                }
                break;
            case EV_ONE_SHOT:
                a->index++;
                if( a->index >= a->frame_cnt ) {
                    a->index = a->frame_cnt - 1;
                }
                break;
            case EV_REVERSE:
                if( a->index > 0 ) {
                    a->index--;
                }
                break;
            case EV_PING_PONG:
                break;
            }
        }
    }
}

void ev_anim_add_sframe(ev_anim* a, ev_sframe *s)
{
    assert(a != NULL);

    if( a && s ) {
        if( a->frame_cnt >= a->frame_size ) {
            grow_array(a);
        }

        a->frames[a->frame_cnt++] = s;
    }
}

ev_sframe* ev_anim_get_current_sframe(ev_anim *a)
{
    assert( a->frames[a->index] != NULL );

    return a ? a->frames[a->index] : NULL;
}

void ev_anim_set_mode(ev_anim *a, ev_anim_mode m)
{
    if( a ) {
        a->mode = m;
    }
}
ev_anim_mode ev_anim_get_mode(ev_anim *a)
{
    if( a ) {
        return a->mode;
    }
    return EV_LOOP;
}
