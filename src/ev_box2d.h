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
EV_API int       ev_world_get_contacts(ev_world *, ev_contact*, int max);
#ifdef __cplusplus
}
#endif

#endif
