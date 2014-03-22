local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
typedef enum {
  EV_FALSE = 0,
  EV_TRUE  = 1
} ev_bool;

typedef struct ev_body ev_body;
typedef struct ev_world ev_world;

typedef enum {
    EV_DYNAMIC = 0,
    EV_STATIC = 1
} ev_body_type;

typedef enum {
    EV_SHAPE_BOX    = 0,
    EV_SHAPE_CIRCLE = 1
} ev_body_shape_type;

typedef struct ev_body_user_data ev_body_user_data;

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
 void      ev_world_set_debug_draw(ev_world*, ev_bool, float width, float height);
 ev_world* ev_world_create(void);
 void      ev_world_destroy(ev_world*);
 ev_vec2   ev_world_get_gravity(ev_world*);
 void      ev_world_set_gravity(ev_vec2);
 void      ev_world_set_dimensions(ev_world*, float w, float h);
 ev_bool   ev_world_intersects(ev_world *, ev_vec2 point, ev_size size);
 void      ev_world_update(ev_world *, float);
 void      ev_world_render(ev_world *);
 ev_body*  ev_body_create(ev_world *, ev_body_user_data );
 void      ev_body_destroy(ev_body* );
 ev_vec2   ev_body_get_position(ev_body*);
 void      ev_body_set_position(ev_body*, ev_vec2);
 void      ev_body_set_shape(ev_body*, ev_body_shape *);
 void      ev_body_set_gravity_scale(ev_body*, float);
 void      ev_body_set_linear_damping(ev_body*, float);
 void      ev_body_set_fixed_rotation(ev_body*, ev_bool);
 void      ev_body_set_linear_velocity(ev_body*, ev_vec2);
 ev_vec2   ev_body_get_linear_velocity(ev_body*);
 void      ev_body_set_rotation(ev_body*, float);
]]
ffi.metatype("ev_body", { __gc = function(self) C.ev_body_destroy(self) end })

local Body = {}
Body.__index = Body

function Body:get_position(x, y)
   local v = C.ev_body_get_position(self._ev_body)
   return v
end

function Body.create()
   local body = {}
   setmetatable(body, Body)
   body._ev_body = C.ev_body_create()
   return body
end

return Body
