local ffi = require('ffi')

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

function _M:ortho(left, right, bottom, top, n, f)
   local m = self.m
   local x_orth = 2 / ( right - left )
   local y_orth = 2 / ( top - bottom )
   local z_orth = -2 / (f - n )
   
   local tx = -(right + left) / ( right - left )
   local ty = -(top + bottom) / ( top - bottom )
   local tz = -(f + n ) / ( f - n )
   
   m[M00] = x_orth
   m[M10] = 0
   m[M20] = 0
   m[M30] = 0
   m[M01] = 0
   m[M11] = y_orth
   m[M21] = 0
   m[M31] = 0
   m[M02] = 0
   m[M12] = 0
   m[M22] = z_orth
   m[M32] = 0
   m[M03] = tx
   m[M13] = ty
   m[M23] = tz
   m[M33] = 1
end

ev_matrix4 = ffi.metatype("ev_matrix4", _M)

function _M.create()
   return ev_matrix4()
end

return _M
