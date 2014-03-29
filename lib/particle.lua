local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
typedef struct ev_particle_system ev_particle_system;
typedef struct ev_particle_group ev_particle_group;

ev_particle_system* ev_particle_system_create(ev_world*);
void                ev_particle_system_destroy(ev_particle_system*);

ev_particle_group*  ev_particle_group_create(ev_particle_system*);
void                ev_particle_group_destroy(ev_particle_group*);
int ev_particle_create(ev_particle_system*, ev_particle_group *, float, float, float, float);
void ev_particle_group_destroy_particles(ev_particle_group *grp);
]]
local M = {}
M.__index = M

local System = {}
System.__index = System

function System.create(world)
   local sys = {}
   setmetatable(sys, System)

   sys._ev_system = C.ev_particle_system_create(world._ev_world)

   return sys
end

function System:spawn_particle(options)
   return C.ev_particle_create(self._ev_system,
                               options.group._ev_group,
                               options.position.x,
                               options.position.y,
                               options.vel.x,
                               options.vel.y )
end

M.system = System

local Group = {}
Group.__index = Group

function Group:destroy_particles()
   C.ev_particle_group_destroy_particles(self._ev_group)
end

function Group.create(system)
   local grp = {}
   setmetatable(grp, Group)

   grp._ev_group = C.ev_particle_group_create(system._ev_system)

   return grp
end

M.group = Group

return M
