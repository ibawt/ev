local ffi = require 'ffi'
local C
local ev

ffi.cdef[[
typedef struct {
  int fb_id;
  ev_texture texture;
  float viewport[4];
} ev_rtex;

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

   -- this should be simpler
   rtex.texture = ev.texture.create()
   rtex.texture._ev_texture.id = rtex._ev_rtex.texture.id
   rtex.texture._ev_texture.width = width
   rtex.texture._ev_texture.height = height
   rtex.sbatch = ev.sbatch.create()
   rtex.sbatch.texture = rtex.texture

   local sprite = ev.sprite.create()
   sprite:set_quad(width, height, 0,0, 1, -1)
   rtex.sbatch:add_sprite(sprite)
   sprite:set_position(width/2, height/2)
   return rtex
end

function RenderTexture:update(dt)
   self.sbatch:update(dt)
end

function RenderTexture:render(fn)
   self:lock()
   fn()
   self:unlock()
end

function RenderTexture:lock()
   C.ev_rtex_bind(self._ev_rtex)
end

function RenderTexture:unlock()
   C.ev_rtex_unbind(self._ev_rtex)
end

function RenderTexture.init(_ev, lib)
   C = lib
   ev = _ev
end

return RenderTexture
