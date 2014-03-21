local ffi = require 'ffi'

ffi.cdef[[
typedef enum {
    EV_LOOP = 0,
    EV_ONE_SHOT = 1,
    EV_REVERSE = 2,
    EV_PING_PONG = 3
} ev_anim_mode;

ev_anim*     ev_anim_create(void);
void         ev_anim_destroy(ev_anim *);
void         ev_anim_add_sframe(ev_anim*, ev_sframe*);
ev_sframe   *ev_anim_get_current_sframe(ev_anim*);
void         ev_anim_set_mode(ev_anim *, ev_anim_mode);
ev_anim_mode ev_anim_get_mode(ev_anim *);
]]

local M = {}
local C = ffi.C

local mt = {
   __index = function(self, key)
      if key == 'add_frame' then
         return C.ev_anim_add_sframe
      end
   end,
   __gc = function(self)
      print 'anim gc'
   end
}

ffi.metatype("ev_anim", mt)

M.create = function()
   return ffi.C.ev_anim_create()
end

return M
