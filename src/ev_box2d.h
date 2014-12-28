#ifndef EV_BOX2D_H_
#define EV_BOX2D_H_

#include "evil.h"
#include "matrix4.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EV_DYNAMIC = 0,
    EV_STATIC = 1
} ev_body_type;

typedef enum {
    EV_SHAPE_BOX    = 0,
    EV_SHAPE_CIRCLE = 1
} ev_body_shape_type;

typedef struct {
    ev_body_shape_type shape;
    float         radius; /* CIRCLE */
    ev_size       size;   /* BOX */
    float         density;
    float         friction;
    float         restitution;
    unsigned int  category_bits;
    unsigned int  mask_bits;
} ev_body_shape;

typedef struct {
    void *a; /* user data a */
    void *b; /* user data b */
} ev_contact;

EV_API void      ev_world_set_debug_draw(ev_world*, ev_bool);
EV_API ev_world* ev_world_create(void);
EV_API void      ev_world_destroy(ev_world*);
EV_API ev_vec2   ev_world_get_gravity(ev_world*);
EV_API void      ev_world_set_gravity(ev_vec2);
EV_API void      ev_world_set_dimensions(ev_world*, float w, float h);
EV_API ev_bool   ev_world_intersects(ev_world *, ev_vec2 point, ev_size size);
EV_API void      ev_world_update(ev_world *, float);
EV_API void      ev_world_render(ev_world *, ev_matrix4 *t);
EV_API int       ev_world_get_contacts(ev_world *, ev_contact*, int max);

EV_API ev_body*  ev_body_create(ev_world *, void *);
EV_API void      ev_body_destroy(ev_body* );
EV_API ev_vec2   ev_body_get_position(ev_body*);
EV_API void      ev_body_set_position(ev_body*, ev_vec2);
EV_API void      ev_body_set_shape(ev_body*, ev_body_shape *);
EV_API void      ev_body_set_gravity_scale(ev_body*, float);
EV_API void      ev_body_set_linear_damping(ev_body*, float);
EV_API void      ev_body_set_fixed_rotation(ev_body*, ev_bool);
EV_API void      ev_body_set_linear_velocity(ev_body*, ev_vec2);
EV_API ev_vec2   ev_body_get_linear_velocity(ev_body*);
EV_API void      ev_body_set_rotation(ev_body*, float);
EV_API void      ev_body_apply_linear_impluse(ev_body *, ev_vec2);

typedef struct {
    int     index;
    void   *body;
    void   *fixture;
    float   weight;
    ev_vec2 normal;
    float   mass;
    ev_vec2 position;
} ev_particle_body_contact;


EV_API ev_particle_system* ev_particle_system_create(ev_world*);
EV_API void                ev_particle_system_destroy(ev_particle_system*);

EV_API void ev_particle_group_destroy_particles(ev_particle_group *grp);

EV_API int ev_particle_create(ev_particle_system *particle, ev_particle_group *, float x, float y, float, float);

EV_API ev_particle_group*  ev_particle_group_create(ev_particle_system*);
EV_API void                ev_particle_group_destroy(ev_particle_group*);

EV_API int ev_particle_system_body_contact_count(ev_particle_system *sys);
EV_API int ev_particle_system_body_contact_at(ev_particle_system *s, int index, ev_particle_body_contact *bc);
EV_API void ev_particle_system_destroy_particle(ev_particle_system *s, int i);
EV_API int ev_particle_system_get_length(ev_particle_system *sys);
EV_API ev_vec2* ev_particle_system_get_position_buffer(ev_particle_system *sys);
EV_API ev_vec2* ev_particle_system_get_velocity_buffer(ev_particle_system *sys);

#ifdef __cplusplus
}
#endif

#endif
