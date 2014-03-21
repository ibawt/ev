local ffi = require 'ffi'

ffi.cdef[[
struct ev_texture {
  int id;
  uint32_t width;
  uint32_t height;
};

ev_texture* ev_texture_create(void);
void        ev_texture_destroy(ev_texture*);
void        ev_texture_bind(ev_texture*);
ev_err_t    ev_texture_load(ev_texture*, const char*);
uint32_t    ev_texture_get_width(ev_texture*);
uint32_t    ev_texture_get_height(ev_texture*);
]]

local M = {}
local C = ffi.C

local mt = {
   __index = {
      destroy = function(self)
         C.ev_texture_destroy(self)
      end,
      load = function(self, filename)
         return C.ev_texture_load(self, filename)
      end
   }
}

ffi.metatype("ev_texture", mt)

M.create = function()
   return ffi.gc(C.ev_texture_create(), mt.__index.destroy)
end

return M
