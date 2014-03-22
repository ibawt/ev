local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
typedef struct ev_world ev_world;

void      ev_world_set_debug_draw(ev_world*, bool);
ev_world* ev_world_create(void);
void      ev_world_destroy(ev_world*);
ev_vec2   ev_world_get_gravity(ev_world*);
void      ev_world_set_gravity(ev_vec2);
void      ev_world_set_dimensions(ev_world*, float w, float h);
bool   ev_world_intersects(ev_world *, ev_vec2 point, ev_size size);
void      ev_world_update(ev_world *, float);
void      ev_world_render(ev_world *, ev_matrix4*);
]]
ffi.metatype("ev_world", { __gc = function(self) C.ev_world_destroy(self._ev_world) end })

local World = {}
World.__index = World

function World.create()
   local world = {}
   setmetatable(world, World)
   world._ev_world = C.ev_world_create()
   return world
end

function World:set_dimensions(width,height)
   C.ev_app_set_dimensions(self._ev_world, width, height)
end

function World:render(transform)
   C.ev_app_render(self._ev_world, transform)
end

function World:update(dt)
   C.ev_app_update(self._ev_world, dt)
end

function World:__index(key, val)
   local props = {
      gravity = function(val)
         return C.ev_world_get_gravity(self._ev_world)
      end
   }
   if props[key] then
      return props[key](val)
   else
      return rawget(self, key, val)
   end
end

function World:__newindex(key, val)
   local props = {
      debug_draw = function(val)
         C.ev_world_set_debug_draw(self._ev_world, val)
      end
   }
   if props[key] then
      props[key](val)
   end
   rawset(self, key, val)
end

return World
