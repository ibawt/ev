local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
typedef struct _ev_graphics ev_graphics;

ev_graphics* ev_graphics_create(void);
void ev_graphics_destroy(ev_graphics*);
void ev_graphics_clear(ev_graphics*, float, float, float, float);
]]

local _M = {}
_M.__index = _M

function _M:clear(r, g, b, a)
   C.ev_graphics_clear(self._ev_graphics, r, g, b, a)
end

function _M.create()
   local g = {}
   setmetatable(g, _M)

   g._ev_graphics = C.ev_graphics_create()

   return g
end

return _M
