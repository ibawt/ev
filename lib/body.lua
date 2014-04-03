local ffi = require 'ffi'
local C
local ev

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
		box    = 0,
		circle = 1
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

	ev_body*  ev_body_create(ev_world *, void *);
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

Body.__gc = function(self)
	 self.world.body_keys[self.key] = nil
end
function Body:get_position(x, y)
	 return C.ev_body_get_position(self._ev_body)
end

function Body.create(world)
	 local body = {}
	 setmetatable(body, Body)
	 body._ev_body = C.ev_body_create(world._ev_world, nil)
	 body.key = tonumber(ffi.cast("int", body.ev_body))
	 body.world= world
	 world.body_keys[ body.key ] = body
	 return body
end

function Body:__index(key)
	 local props = {
			velocity = function()
				 return C.ev_body_get_linear_velocity(self._ev_body)
			end
	 }
	 if props[key] then
			return props[key]()
	 else
			return getmetatable(self)[key] or rawget(self, key)
	 end
end

local function body_shape_default()
	 local bit = require 'bit'
	 local bs = ffi.new("ev_body_shape")
	 bs.shape = "circle"
	 bs.radius = 1
	 bs.density = 1
	 bs.friction = 0
	 bs.restitution = 1
	 bs.category_bits = 1
	 bs.mask_bits = 0xFFFF
	 return bs
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
			end,
			shape = function(val)
				 local bs = body_shape_default()
				 for k,v in pairs(val) do
						bs[k] = v
				 end
				 C.ev_body_set_shape(self._ev_body, bs)
			end
	 }
	 if props[key] then
			props[key](val)
	 else
			rawset(self, key, val)
	 end
end

function Body.init(_ev, lib)
	 ev = _ev
	 C = lib
end

return Body
