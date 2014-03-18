#include <assert.h>
#include "Box2D/Box2D.h"
#include "ev_box2d.h"
#include "vector2.h"

class ev_contact_listener : public b2ContactListener
{
public:
    virtual void BeginContact(b2Contact *b) {
        ev_log("begin contact");
    }
    virtual void EndContact(b2Contact *b) {
        ev_log("end contact");
    }
};

struct ev_world {
    ev_world() : world(b2Vec2(0, 9.8f)) { }
    float ptm_ratio;
    ev_contact_listener listener;
    b2World world;
    b2Body *world_box;
};

struct ev_body {
    b2Body           *body;
    ev_world         *world;
    ev_body_user_data user_data;
};

ev_world* ev_world_create(void)
{
    ev_world *world;

    world = new (ev_malloc(sizeof(ev_world))) ev_world;

    world->world.SetContactListener(&world->listener);
    world->world.SetAllowSleeping(true);
    world->world.SetContinuousPhysics(true);

    world->ptm_ratio = 32.0f;

    return world;
}

void ev_world_destroy(ev_world *world)
{
    assert( world != NULL );
    world->~ev_world();

    ev_free(world);
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

void ev_world_set_dimensions(ev_world *world, float w, float h)
{
    b2BodyDef groundBodyDef;

    w /= world->ptm_ratio;
    h /= world->ptm_ratio;

    groundBodyDef.position.Set(0.0f, 0.0f);

    world->world_box = world->world.CreateBody(&groundBodyDef);

    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0,0), b2Vec2(w,0));
    world->world_box->CreateFixture(&groundBox, 0);

    groundBox.Set(b2Vec2(0, h), b2Vec2(w,h));
    world->world_box->CreateFixture(&groundBox, 0);

    groundBox.Set(b2Vec2(0,h), b2Vec2(0, 0));
    world->world_box->CreateFixture(&groundBox, 0);

    groundBox.Set(b2Vec2(w,h), b2Vec2(w, 0));
    world->world_box->CreateFixture(&groundBox, 0);
}

void ev_world_update(ev_world *world, float dt)
{
    if( world ) {
        world->world.Step(dt, 8, 1);
    }
}

ev_body* ev_body_create(ev_world *world, ev_body_user_data data)
{
    ev_body *b;
    b2BodyDef bodyDef;

    if(!world) {
        return NULL;
    }

    b = (ev_body*)ev_malloc(sizeof(ev_body));
    memset(b, 0, sizeof(ev_body));
    b->user_data = data;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0,0);
    bodyDef.userData = &b->user_data;

    b->body = world->world.CreateBody(&bodyDef);
    b->world = world;

    return b;
}

void ev_body_destroy(ev_body *body)
{
    if( body ) {
        body->world->world.DestroyBody(body->body);
        body->body = NULL;
        ev_free(body);
    }
}
ev_vec2 ev_body_get_position(ev_body* body)
{
    ev_vec2 pos;
    b2Vec2 b_vec;

    b_vec = body->body->GetPosition();

    pos.x = b_vec.x * body->world->ptm_ratio;
    pos.y = b_vec.y * body->world->ptm_ratio;

    return pos;
}

void ev_body_set_position(ev_body *body, ev_vec2 v)
{
    body->body->SetTransform(b2Vec2(v.x /body->world->ptm_ratio,
                                    v.y / body->world->ptm_ratio ), 0.0f );
}

void ev_body_set_shape(ev_body *body, ev_body_shape *shape)
{
    b2PolygonShape polyShape;
    b2CircleShape circleShape;
    b2FixtureDef fixtureDef;

    switch(shape->shape) {
    case EV_SHAPE_BOX:
        polyShape.SetAsBox( shape->size.w, shape->size.h);
        fixtureDef.shape = &polyShape;
        break;
    case EV_SHAPE_CIRCLE:
        circleShape.m_radius = shape->radius;
        fixtureDef.shape = &circleShape;
        break;
    }

    fixtureDef.density = shape->density;
    fixtureDef.friction = shape->friction;
    fixtureDef.restitution = shape->restitution;
    fixtureDef.filter.categoryBits = shape->category_bits;
    fixtureDef.filter.maskBits = shape->mask_bits;

    body->body->CreateFixture( &fixtureDef );
}


void ev_body_set_gravity_scale(ev_body *body, float scale)
{
    if( body ) {
        body->body->SetGravityScale(scale);
    }
}
void ev_body_set_linear_damping(ev_body *body, float dampening)
{
    if( body ) {
        body->body->SetLinearDamping(dampening);
    }
}

void ev_body_set_fixed_rotation(ev_body *body, ev_bool b)
{
    if( body ) {
        body->body->SetFixedRotation(b);
    }
}

void ev_body_set_linear_velocity(ev_body *body, ev_vec2 v)
{
    if( body ) {
        v = ev_vec2_scale(v, 1 / body->world->ptm_ratio);
        body->body->SetLinearVelocity(b2Vec2(v.x, v.y));
    }
}
