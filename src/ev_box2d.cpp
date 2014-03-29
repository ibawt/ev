#include <assert.h>
#include "Box2D/Box2D.h"
#include "ev_box2d.h"
#include "vector2.h"
#include "debug_draw.h"
#include <search.h>
#include "uthash.h"

typedef struct {
    b2Contact *contact;
    UT_hash_handle hh;
} contact;

class ev_contact_listener : public b2ContactListener
{
public:
    ev_contact_listener() : head(NULL) {
    }

    ~ev_contact_listener() {
        contact *c;
        contact *tmp;

        HASH_ITER(hh, head, c, tmp) {
            HASH_DEL(head, c);
            ev_free(c);
        }
    }

    virtual void BeginContact(b2Contact *b) {
        void *key = b;
        contact *c;
        HASH_FIND_PTR(head, &key, c);
        if( c == NULL ) {
            c = (contact *)ev_malloc(sizeof(contact));
            c->contact = b;
            HASH_ADD_PTR(head, contact, c);
        } else {
            assert(true);
        }
    }
    virtual void EndContact(b2Contact *b) {
        void *key = b;
        contact *c;
        HASH_FIND_PTR(head, &key, c);

        if( c ) {
            HASH_DEL( head, c);
            ev_free(c);
        } else {
            assert(true);
        }

    }
    contact *head;
};

struct ev_world {
    /* TODO these shouldn't be constants some how */
    ev_world() : world(b2Vec2(0, 5.0f)), debug_draw(NULL) { }
    float ptm_ratio;
    ev_contact_listener listener;
    b2World world;
    b2Body *world_box;
    b2DebugDraw *debug_draw;

};

struct ev_body {
    b2Body           *body;
    ev_world         *world;
};

struct ev_particle_group {
    b2ParticleGroup *group;
};

struct ev_particle_system {
    b2ParticleSystem *system;
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
int ev_world_get_contacts(ev_world *world, ev_contact *contacts, int max)
{
    int num = 0;
    contact *c;
    contact *tmp;

    HASH_ITER(hh, world->listener.head, c, tmp) {
        contacts->a = c->contact->GetFixtureA()->GetBody()->GetUserData();
        contacts->b = c->contact->GetFixtureB()->GetBody()->GetUserData();
        contacts++;
        num++;

        if( num >= max ) {
            break;
        }
    }
    return num;
}

void ev_world_render(ev_world *world, ev_matrix4 *t)
{
    world->debug_draw->SetTransform(t);
    world->world.DrawDebugData();
}

void ev_world_set_debug_draw(ev_world* world, ev_bool b)
{
    if( b ) {
        world->debug_draw = new b2DebugDraw(world->ptm_ratio);
    } else {
        delete world->debug_draw;
    }
    world->world.SetDebugDraw( world->debug_draw);
}

void ev_world_destroy(ev_world *world)
{
    assert( world != NULL );
    delete world->debug_draw;
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

    groundBodyDef.position.Set(0, 0);

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

ev_body* ev_body_create(ev_world *world, void *opaque)
{
    ev_body *b;
    b2BodyDef bodyDef;

    if(!world) {
        return NULL;
    }

    b = (ev_body*)ev_malloc(sizeof(ev_body));
    memset(b, 0, sizeof(ev_body));
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0,0);

    bodyDef.userData = b;

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
        polyShape.SetAsBox( shape->size.w / body->world->ptm_ratio,
                            shape->size.h / body->world->ptm_ratio);
        fixtureDef.shape = &polyShape;
        break;
    case EV_SHAPE_CIRCLE:
        circleShape.m_radius = shape->radius / body->world->ptm_ratio;
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

ev_vec2 ev_body_get_linear_velocity(ev_body *body)
{
    ev_vec2 v;
    b2Vec2  bv;

    bv = body->body->GetLinearVelocity();

    v.x = bv.x * body->world->ptm_ratio;
    v.y = bv.y * body->world->ptm_ratio;

    return v;
}

ev_particle_system* ev_particle_system_create(ev_world* world)
{
    b2ParticleSystemDef systemDef;

    ev_particle_system *system = new (ev_malloc(sizeof(ev_particle_system))) ev_particle_system;
    system->system = world->world.CreateParticleSystem(&systemDef);

    return system;
}
void                ev_particle_system_destroy(ev_particle_system* sys)
{
}

ev_particle_group*  ev_particle_group_create(ev_particle_system *sys)
{

    b2ParticleGroupDef groupDef;
    ev_particle_group *grp = new (ev_malloc(sizeof(ev_particle_group))) ev_particle_group;
    grp->group = sys->system->CreateParticleGroup(groupDef);

    return grp;
}
void                ev_particle_group_destroy(ev_particle_group*);


int ev_particle_create(ev_particle_system *system, float x, float y)
{
    b2ParticleDef def;
    int index;

    def.position.Set(x/32,y/32);

    def.color = b2ParticleColor(255,255,255,255);

    index = system->system->CreateParticle(def);

    return index;
}
