local ffi = require('ffi')
local C = ffi.C

ffi.cdef[[
typedef struct {
    float x,y;
} ev_vec2;
]]

local _M = {}
_M.__index = _M

local ev_vec2

function _M.__add (a, b)
   return ev_vec2( a.x + b.x, a.y + b.y)
end

function _M.__sub(a, b)
   return ev_vec2( a.x - b.x, a.y - b.y)
end

function _M:len()
   local x = self.x * self.x
   local y = self.y * self.y

   local d = math.sqrt(x * x + y * y)

   return d
end

function _M:translate(x,y)
   self.x = self.x + x
   self.y = self.y + y
end

function _M:normalize()
   local d = self:len()

   self.x = self.x / d
   self.y = self.y / d
end

function _M:scale(x, y)
   self.x = self.x * x
   self.y = self.y * (x or y)
end

ev_vec2 = ffi.metatype("ev_vec2", _M)

function _M.create()
   return ev_vec2()
end

return _M
