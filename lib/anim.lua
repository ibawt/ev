local ffi = require 'ffi'

ffi.cdef[[
typedef enum {
    EV_LOOP = 0,
    EV_ONE_SHOT = 1,
    EV_REVERSE = 2,
    EV_PING_PONG = 3
} ev_anim_mode;

typedef struct ev_anim ev_anim;
typedef struct ev_sframe ev_sframe;

ev_anim*     ev_anim_create(void);
void         ev_anim_destroy(ev_anim *);
void         ev_anim_add_sframe(ev_anim*, ev_sframe*);
ev_sframe   *ev_anim_get_current_sframe(ev_anim*);
void         ev_anim_set_mode(ev_anim *, ev_anim_mode);
ev_anim_mode ev_anim_get_mode(ev_anim *);
]]

ffi.metatype("ev_anim", { __gc = function(self) C.ev_anim_destroy(self) end })
local C

local Anim = {}
Anim.__index = Anim

function Anim:add_frame(sframe)
   C.ev_anim_add_sframe(self._ev_anim, sframe)
end

Anim.create = function()
   local ev_anim = ffi.C.ev_anim_create()
   local anim = {}
   setmetatable(anim, Anim)
   anim._ev_anim = ev_anim
   return anim
end

function Anim.init(_ev, lib)
   C = lib
end

return Anim
