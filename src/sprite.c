#include <assert.h>
#include <stdio.h>

#include "utils.h"
#include "sprite.h"
#include "animation.h"
#include "sprite_sheet.h"
#include "ev_box2d.h"
#include "application.h"

struct _ev_sprite
{
    ev_vec2  position;
    float    rotation;
    float    scale;
    int      visible;
    ev_anim *animation;
    ev_body *body;
};

static void ev_sprite_init(ev_sprite *s)
{
    assert( s != NULL);

    memset(s, 0, sizeof(ev_sprite));
    s->visible = 1;
    s->scale = 1.0f;
}

ev_sprite* ev_sprite_create(void)
{
    ev_sprite *s = ev_malloc(sizeof(ev_sprite));
    if( s ) {
        ev_sprite_init(s);
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
    UNUSED(s);
}

int ev_sprite_get_visiblity(ev_sprite *s)
{
    assert( s != NULL );

    return s->visible;
}

int ev_sprite_fill(ev_sprite* s, ev_bvertex* b)
{
    int i;
    ev_bvertex *src;
    ev_vec2 pos;

    if( s && b && s->visible) {
        if( s->body ) {
            pos = ev_body_get_position(s->body);
        } else {
            pos = s->position;
        }

        src = ev_sframe_get_bvertex(ev_anim_get_current_sframe(s->animation));
        if( src ) {

            for( i = 0 ; i < EV_SPRITE_NUM_VERTS ; ++i,b++,src++ ) {
                *b = *src;
                b->scale = s->scale;
                b->rotation = s->rotation;
                b->tx = pos.x;
                b->ty = pos.y;
            }
        }
        return 1;
    }
    return 0;
}

void ev_sprite_set_body(ev_sprite *s, ev_body *body)
{
    if( !(s && body) )
        return;

    s->body = body;
}
