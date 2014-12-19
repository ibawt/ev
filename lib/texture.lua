local ffi = require 'ffi'
local C = ffi.C
local ev

ffi.cdef[[
typedef struct {
  int id;
  uint32_t width;
  uint32_t height;
} ev_texture;

ev_texture* ev_texture_create(void);
void        ev_texture_destroy(ev_texture*);
void        ev_texture_bind(ev_texture*);
ev_err_t    ev_texture_load(ev_texture*, const char*);
uint32_t    ev_texture_get_width(ev_texture*);
uint32_t    ev_texture_get_height(ev_texture*);
]]
ffi.metatype("ev_texture", { __gc = function(self) C.ev_texture_destroy(self._ev_texture) end })

local _M = {}
_M.__index = _M

function _M:load(filename)
   return C.ev_texture_load(self._ev_texture, filename)
end

function _M:__index(key)
   if key == 'width' then
      return self._ev_texture.width
   elseif key == 'height' then
      return self._ev_texture.height
   else
      return getmetatable(self)[key] or rawget(self, key)
   end
end

function _M.create()
   local texture = {}
   setmetatable(texture, _M)
   texture._ev_texture = C.ev_texture_create()
   return texture
end

function _M.init(_ev)
   ev = _ev
end

return _M
