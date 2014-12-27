local ffi = require('ffi')
local abs = math.abs
local sqrt = math.sqrt
local atan2 = math.atan2

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

function _M.__len(self)
   return sqrt(self.x * self.x + self.y * self.y)
end

function _M:translate(x,y)
   self.x = self.x + x
   self.y = self.y + y
end

function _M.__eq(a, b)
   local function float_cmp(a,b)
      return abs(b-a) < 0.00001
   end
   return float_cmp(a.x, b.x) and float_cmp(a.y, b.y)
end

function _M:clamp(min,max)
   local l = #self
   if l < min then
      self.x = (self.x / l ) * min
      self.y = (self.y / l ) * min
   elseif l > max then
      self.x = (self.x / l ) * max
      self.y = (self.y / l ) * max
   end
end

function _M:normalize()
   local d = #self

   self.x = self.x / d
   self.y = self.y / d
end

function _M:scale(x, y)
   self.x = self.x * x
   self.y = self.y * (y or x)
end

function _M:angle()
   local d = atan2(self.x, self.y)
   if d < 0 then
      d = d + math.pi * 2
   end
   return d
end

ev_vec2 = ffi.metatype("ev_vec2", _M)

function _M.create(x,y)
   return ev_vec2(x or 0, y or 0)
end

return _M
