#include <assert.h>
#include <stdio.h>

#include "utils.h"
#include "sprite.h"
#include "animation.h"
#include "sprite_sheet.h"
#include "ev_box2d.h"
#include "application.h"
#include "vector2.h"

void ev_sprite_set_quad(ev_sprite *sprite, float w, float h, float left, float top, float right, float bottom)
{
    ev_sframe *frame;

    frame = ev_sframe_create_quad(w, h, left, top, right, bottom);
    if(!sprite->animation) {
        sprite->animation = ev_anim_create();
    }
    ev_anim_add_sframe(sprite->animation, frame);
}

void ev_sprite_init(ev_sprite *s)
{
    assert( s != NULL );
    
    memset(s, 0, sizeof(ev_sprite));

    s->visible = EV_TRUE;
    s->scale = 1.0f;
    s->opacity = 1.0f;
}

ev_vec2* ev_sprite_get_position(ev_sprite* s)
{
    if( s->body ) {
        s->position = ev_body_get_position(s->body);
    }
    return &s->position;
}

void ev_sprite_set_position(ev_sprite *s, float x, float y)
{
    s->position.x = x;
    s->position.y = y;
    
    if( s->body ) {
        ev_body_set_position(s->body, s->position);
    }
}

float ev_sprite_get_rotation(ev_sprite* s)
{
    return s->rotation;
}

void ev_sprite_set_rotation(ev_sprite* s, float r)
{
    s->rotation = r;
}

void ev_sprite_set_animation(ev_sprite* s, ev_anim* a)
{
    s->animation = a;
}

ev_anim* ev_sprite_get_animation(ev_sprite* s)
{
    return s->animation;
}

void ev_sprite_update(ev_sprite* s, float dt)
{
    if( s->animation ) {
        ev_anim_update(s->animation, dt);
    }
}

void ev_sprite_render(ev_sprite* s)
{
    UNUSED(s);
}

ev_bool ev_sprite_get_visiblity(ev_sprite *s)
{
    assert( s != NULL );

    return s->visible;
}

void ev_sprite_set_visibilty(ev_sprite *s, ev_bool v)
{
    assert( s != NULL );

    s->visible = v;
}


int ev_sprite_fill(ev_sprite* s, ev_bvertex* b)
{
    int i;
    ev_bvertex *src;
    ev_vec2 pos;

    assert( s != NULL );
    assert( b != NULL );
    
    if(s->visible) {
        if( s->body ) {
            pos = ev_body_get_position(s->body);
        } else {
            pos = s->position;
        }

        src = ev_sframe_get_bvertex(ev_anim_get_current_sframe(s->animation));
        assert( src != NULL );
        for( i = 0 ; i < EV_SPRITE_NUM_VERTS ; ++i,b++,src++ ) {
            *b = *src;
            b->scale = s->scale;
            b->rotation = s->rotation;
            b->tx = pos.x;
            b->ty = pos.y;
            b->opacity = s->opacity;
        }
        return EV_SPRITE_NUM_VERTS;
    }
    return 0;
}

void ev_sprite_set_body(ev_sprite *s, ev_body *body)
{
    s->body = body;
}
