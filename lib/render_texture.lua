local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
typedef struct ev_rtex ev_rtex;

ev_rtex* ev_rtex_create(int width, int height);
void     ev_rtex_destroy(ev_rtex*);
void     ev_rtex_bind(ev_rtex*);
void     ev_rtex_unbind(ev_rtex*);
]]

local RenderTexture = {}
RenderTexture.__index = RenderTexture

function RenderTexture.create(width, height)
	 local rtex = {}
	 setmetatable(rtex, RenderTexture)
	 rtex._ev_rtex = C.ev_rtex_create(width, height)
	 return rtex
end

function RenderTexture:update(dt)
end

function RenderTexture:render()
end

function RenderTexture:lock()
	 C.ev_rtex_bind(self._ev_rtex)
end

function RenderTexture:unlock()
	 C.ev_rtex_unbind(self._ev_tex)
end

return RenderTexture
