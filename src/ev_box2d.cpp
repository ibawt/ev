#include <assert.h>
#include "Box2D/Box2D.h"
#include "ev_box2d.h"

struct ev_world {
};



ev_world* ev_world_create(void)
{
    b2World *world = new b2World(b2Vec2(0,-9.8f));
    return (ev_world*)world;
}

void ev_world_destroy(ev_world *world)
{
    assert( world != NULL );
    delete (b2World*)world;
}

ev_vec2 ev_world_get_gravity(ev_world *world)
{
    b2World *b2_world;
    ev_vec2  gravity = { 0.0f, 0.0f };
    assert( world != NULL );

    if( world ) {
        b2_world = (b2World*)world;
        b2Vec2 b_gravity = b2_world->GetGravity();
        gravity.x = b_gravity.x;
        gravity.y = b_gravity.y;
    }
    return gravity;
}
