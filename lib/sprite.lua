local ffi = require 'ffi'
local C = ffi.C
local ev

ffi.cdef[[
typedef struct ev_body ev_body;
typedef struct _ev_sframe
{
    const char *key;
    ev_size     source_size;
    ev_size     size;
    int         trimmed;
    ev_rect     texture_rect;
    ev_vec2     offset;
    ev_bool     rotated;
    ev_rect     color_rect;
    ev_bvertex  batch_verts[6];
} ev_sframe;
]]

local _M = {}
_M.__index = _M

function _M:fill(dst)
   if self.opacity <= 0 or not self.visible then
      return 0
   end
   -- need to force it to a pointer
   local src = self.animation:current_frame().batch_verts + 0

   local r = self.rotation
   local s = self.scale
   local p = self.position
   local o = self.opacity

   for i=1,6 do
      dst.x = src.x
      dst.y = src.y
      dst.u = src.u
      dst.v = src.v
      dst.rotation = r
      dst.scale = s
      dst.tx = p.x
      dst.ty = p.y
      dst.opacity = o

      dst = dst + 1
      src = src + 1
   end

   return 6
end

function _M:update(dt)
   if self.animation then
      self.animation:update(dt)
   end

   if self.body then
      self.position = self.body:get_position()
   end
end

function _M:set_position(x,y)
   self.position.x = x
   self.position.y = y

   if self.body then
      self.body.position = self.position
   end
end

function _M.create()
   local sprite = {}
   setmetatable(sprite, _M)
   sprite.visible = true
   sprite.opacity = 1
   sprite.rotation = 0
   sprite.scale = 1
   sprite.position = ev.vec2.create()
   return sprite
end

local function fill_batch_verts(frame)
   local bv = frame.batch_verts + 0
   -- top left
   bv[0].x = -frame.size.w/2
   bv[0].y = -frame.size.h/2
   bv[0].u = frame.texture_rect.origin.x
   bv[0].v = frame.texture_rect.origin.y

   -- top right
   bv[1].x = frame.size.w/2
   bv[1].y = -frame.size.h/2
   bv[1].u = frame.texture_rect.origin.x + frame.texture_rect.size.w
   bv[1].v = frame.texture_rect.origin.y

   -- bottom right
   bv[2].x = frame.size.w/2
   bv[2].y = frame.size.h/2
   bv[2].u = frame.texture_rect.origin.x + frame.texture_rect.size.w
   bv[2].v = frame.texture_rect.origin.y + frame.texture_rect.size.h

   
   bv[3].x = frame.size.w/2
   bv[3].y = frame.size.h/2
   bv[3].u = frame.texture_rect.origin.x + frame.texture_rect.size.w
   bv[3].v = frame.texture_rect.origin.y + frame.texture_rect.size.h
   
   -- bottom left
   bv[4].x = -frame.size.w/2
   bv[4].y = frame.size.h/2
   bv[4].u = frame.texture_rect.origin.x
   bv[4].v = frame.texture_rect.origin.y + frame.texture_rect.size.h
   
   bv[5].x = -frame.size.w/2
   bv[5].y = -frame.size.h/2
   bv[5].u = frame.texture_rect.origin.x
   bv[5].v = frame.texture_rect.origin.y
end

function _M:set_quad(w, h, left, top, right, bottom)
   if not self.animation then
      self.animation = ev.anim.create()
   end

   local sframe

   if #self.animation.frames > 0 then
      sframe = self.animation.frames[1]
   else
      sframe = ffi.new('ev_sframe')
      self.animation:add_frame(sframe)
   end

   sframe.size.w = w
   sframe.size.h = h
   sframe.texture_rect.origin.x = left
   sframe.texture_rect.origin.y = top
   sframe.texture_rect.size.w = right
   sframe.texture_rect.size.h = bottom

   fill_batch_verts(sframe)
end

function _M.init(_ev)
   ev = _ev
end

return _M
