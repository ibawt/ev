local ffi = require 'ffi'
local C = ffi.C
local ev

ffi.cdef[[
typedef struct ev_particle_system ev_particle_system;
typedef struct ev_particle_group ev_particle_group;
typedef struct {
    int     index;
    void   *body;
    void   *fixture;
    float   weight;
    ev_vec2 normal;
    float   mass;
    ev_vec2 position;
} ev_particle_body_contact;

ev_particle_system* ev_particle_system_create(ev_world*);
void                ev_particle_system_destroy(ev_particle_system*);
ev_particle_group*  ev_particle_group_create(ev_particle_system*);
void                ev_particle_group_destroy(ev_particle_group*);
int                 ev_particle_create(ev_particle_system*, ev_particle_group *, float, float, float, float);
void                ev_particle_group_destroy_particles(ev_particle_group *grp);
void                ev_particle_system_update(ev_particle_system* sys, float);
int                 ev_particle_system_body_contact_count(ev_particle_system *sys);
int                 ev_particle_system_body_contact_at(ev_particle_system *s, int index, ev_particle_body_contact *bc);
void                ev_particle_system_destroy_particle(ev_particle_system *s, int i);
int ev_particle_system_get_length(ev_particle_system *sys);
ev_vec2* ev_particle_system_get_position_buffer(ev_particle_system *sys);
ev_vec2* ev_particle_system_get_velocity_buffer(ev_particle_system *sys);

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

function System:destroy_particle(index)
   C.ev_particle_system_destroy_particle(self._ev_system, index)
end

function System:get_body_contacts()
   local n = C.ev_particle_system_body_contact_count(self._ev_system)   
   local contacts = {}
   for i=0,n-1 do
      local bc = ffi.new("ev_particle_body_contact")
      C.ev_particle_system_body_contact_at(self._ev_system, i, bc)
      contacts[#contacts+1] = bc
   end

   return contacts
end

function System:get_count()
   return C.ev_particle_system_get_length(self._ev_system)
end

function System:get_positions()
   return C.ev_particle_system_get_position_buffer(self._ev_system)
end

function System:spawn_particle(options)
   return C.ev_particle_create(self._ev_system,
                               options.group._ev_group,
                               options.position.x,
                               options.position.y,
                               options.vel.x,
                               options.vel.y )
end

function System:update(dt)
   C.ev_particle_system_update(C._ev_system, dt)
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

function M.init(_ev)
   ev = _ev
end

return M
