local ffi = require('ffi')
local sin = math.sin
local cos = math.cos
local sqrt = math.sqrt

local _M = {}
_M.__index = _M

local M00=0
local M01=4
local M02=8
local M03=12
local M10=1
local M11=5
local M12=9
local M13=13
local M20=2
local M21=6
local M22=10
local M23=14
local M30=3
local M31=7
local M32=11
local M33=15

local ev_matrix4

function _M:identity()
   local m = self.m
   m[M00] = 1
   m[M10] = 0
   m[M20] = 0
   m[M30] = 0
   m[M01] = 0
   m[M11] = 1
   m[M21] = 0
   m[M31] = 0
   m[M02] = 0
   m[M12] = 0
   m[M22] = 1
   m[M32] = 0
   m[M03] = 0
   m[M13] = 0
   m[M23] = 0
   m[M33] = 1
end

function _M:multiply(other)
   for i=0,3 do
      self.m[i*4] =
         self.m[i*4]   * other.m[0] +
         self.m[i*4+1] * other.m[1*4] +
         self.m[i*4+2] * other.m[2*4] +
         self.m[i*4+3] * other.m[3*4]

      self.m[i*4+1] =
         self.m[i*4]   * other.m[1] +
         self.m[i*4+1] * other.m[1*4 + 1] +
         self.m[i*4+2] * other.m[2*4 + 1] +
         self.m[i*4+3] * other.m[3*4 + 1]

      self.m[i*4+2] =
         self.m[i*4]   * other.m[2] +
         self.m[i*4+1] * other.m[1*4 + 2] +
         self.m[i*4+2] * other.m[2*4 + 2] +
         self.m[i*4+3] * other.m[3*4 + 2]

      self.m[i*4+3] =
         self.m[i*4]   * other.m[3] +
         self.m[i*4+1] * other.m[1*4 + 3] +
         self.m[i*4+2] * other.m[2*4 + 3] +
         self.m[i*4+3] * other.m[3*4 + 3]
   end
end

function _M:translate(x, y, z)
   local m = ev.matrix.create()
   m:set_translation(x,y,z)

   self:multiply(m)
end

function _M:set_scale(x, y, z)
   self:identity()

   y = y or x
   z = z or x
   
   self.m00 = x
   self.m11 = y
   self.m22 = z
end

function _M:set_rotation(angle, x, y, z)
   local c = cos( angle )
   local s = sin( angle )

   local len = sqrt( x * x + y * y + z * z)

   x = x / len
   y = y / len
   z = z / len

   self:identity()

   self.m00 = x * x * (1 - c) + c
   self.m10 = y * x * (1 - c) - z*s
   self.m20 = z * x * (1 - c) + y*s

   self.m01 = x * y * (1 - c) + z *s
   self.m11 = y * y * (1 - c) + c
   self.m21 = z * y * (1 - c) - x*s

   self.m02 = x * z * (1 -c) - y*s
   self.m12 = y * z * (1 -c) + x*s
   self.m22 = z * z * (1 -c ) + c
end

function _M:rotate(angle, x, y, z )
   local m = ev.matrix.create()
   m:set_rotation(angle, x, y, z)
   self:multiply(m)
end

function _M:scale(x, y, z)
   local m = ev.matrix.create()

   m:set_scale(x,y, z)

   self:multiply(m)
end

function _M:set_translation(x,y,z)
   self.m30 = x
   self.m31 = y
   self.m32 = z
end

function _M:ortho(left, right, bottom, top, n, f)
   local m = self.m
   local x_orth = 2 / ( right - left )
   local y_orth = 2 / ( top - bottom )
   local z_orth = -2 / (f - n )
   
   local tx = -(right + left) / ( right - left )
   local ty = -(top + bottom) / ( top - bottom )
   local tz = -(f + n ) / ( f - n )
   
   self.m00 = x_orth
   self.m10 = 0
   self.m20 = 0
   self.m30 = 0
   self.m01 = 0
   self.m11 = y_orth
   self.m21 = 0
   self.m31 = 0
   self.m02 = 0
   self.m12 = 0
   self.m22 = z_orth
   self.m32 = 0
   self.m03 = tx
   self.m13 = ty
   self.m23 = tz
   self.m33 = 1
end

ev_matrix4 = ffi.metatype("ev_matrix4", _M)

function _M.create()
   local m = ev_matrix4()
   m:identity()

   return m
end

return _M
