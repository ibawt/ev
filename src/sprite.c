#include "sprite.h"
#include "animation.h"
#include "sprite_sheet.h"

struct _ev_sprite
{
    ev_vec2  position;
    float    rotation;
    float    scale;
    ev_anim *animation;
};

ev_sprite* ev_sprite_create(void)
{
    ev_sprite *s = ev_malloc(sizeof(ev_sprite));
    if( s ) {
        memset(s, 0, sizeof(ev_sprite));
        s->scale = 1.0f;
    }
    return s;
}

void ev_sprite_destroy(ev_sprite* s)
{
    if( s ) {
        ev_free(s);
    }
}

ev_vec2* ev_sprite_get_position(ev_sprite* s)
{
    return s ? &s->position : NULL;
}

void ev_sprite_set_position(ev_sprite *s, float x, float y)
{
    if( s ) {
        s->position.x = x;
        s->position.y = y;
    }
}

float ev_sprite_get_rotation(ev_sprite* s)
{
    return s ? s->rotation : 0.0f;
}

void ev_sprite_set_rotation(ev_sprite* s, float r)
{
    if( s ) {
        s->rotation = r;
    }
}

void ev_sprite_set_animation(ev_sprite* s, ev_anim* a)
{
    if( s ) {
        s->animation = a;
    }
}

ev_anim* ev_sprite_get_animation(ev_sprite* s)
{
    return s ? s->animation : NULL;
}

void ev_sprite_update(ev_sprite* s, float dt)
{
    if( s ) {
        if( s->animation ) {
            ev_anim_update(s->animation, dt);
        }
    }
}

void ev_sprite_render(ev_sprite* s)
{
}

void ev_sprite_fill(ev_sprite* s, ev_bvertex* b)
{
    int i;
    ev_bvertex *src;

    if( s && b ) {
        src = ev_sframe_get_bvertex(ev_anim_get_current_sframe(s->animation));
        if( src ) {
            for( i = 0 ; i < EV_SPRITE_NUM_VERTS ; ++i,b++,src++ ) {
                *b = *src;
                b->scale = s->scale;
                b->rotation = s->rotation;
                b->tx = s->position.x;
                b->ty = s->position.y;
            }
        }
    }
}
