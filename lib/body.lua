local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
typedef enum {
  EV_FALSE = 0,
  EV_TRUE  = 1
} ev_bool;

typedef struct ev_body ev_body;

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
   return C.ev_body_get_position(self._ev_body)
end

function Body.create(world, shape)
   local body = {}
   setmetatable(body, Body)
   body._ev_body = C.ev_body_create(world, 0)
   C.ev_body_set_shape(shape)
   return body
end

function Body:__index(key)
   local props = {
      velocity = function()
         return C.ev_body_get_linear_velocity(self._ev_body)
      end
   }
   if props[key] then
      props[key]()
   end
end

function Body:__newindex(key, val)
   local props = {
      position = function(val)
         C.ev_body_set_position(self._ev_body, val)
      end,
      gravity_scale = function(val)
         C.ev_body_set_gravity_scale(self._ev_body, val)
      end,
      linear_dampening = function(val)
         C.ev_body_set_linear_dampening(self._ev_body, val)
      end,
      fixed_rotation = function(val)
         C.ev_body_set_fixed_rotation(self._ev_body, val)
      end,
      velocity = function(val)
         C.ev_body_set_linear_velocity(self._ev_body, val)
      end
   }
   if props[key] then
      props[key](val)
   else
      rawset(self, key, val)
   end
end

return Body
