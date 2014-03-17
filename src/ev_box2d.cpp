#include <assert.h>
#include "Box2D/Box2D.h"
#include "ev_box2d.h"


class ev_contact_listener : public b2ContactListener
{
public:
    virtual void BeginContact(b2Contact *b) {
    }
    virtual void EndContact(b2Contact *b) {
    }
};


struct ev_world {
    ev_world() : world(b2Vec2(0,-9.8)) { }
    ev_contact_listener listener;
    b2World world;
};


ev_world* ev_world_create(void)
{
    ev_world *world;

    world = new (ev_malloc(sizeof(ev_world))) ev_world;

    return (ev_world*)world;
}

void ev_world_destroy(ev_world *world)
{
    assert( world != NULL );
    delete world;
}

ev_vec2 ev_world_get_gravity(ev_world *world)
{
    ev_vec2  gravity = { 0.0f, 0.0f };
    assert( world != NULL );

    if( world ) {
        b2Vec2 b_gravity = world->world.GetGravity();
        gravity.x = b_gravity.x;
        gravity.y = b_gravity.y;
    }
    return gravity;
}
