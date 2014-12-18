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
    ev_bvertex  batch_verts[4];
} ev_sframe;
]]

local ev_vec2 = ffi.metatype("ev_vec2", {})
local Sprite = {}
Sprite.__index = Sprite

function Sprite:fill(dst)
   if self.opacity <= 0 or not self.visible then
      return 0
   end
   -- need to force it to a pointer
   local src = self.animation:current_frame().batch_verts + 0

   local r = self.rotation
   local s = self.scale
   local p = self.position
   local o = self.opacity

   for i=0,3 do
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

   return 4
end

function Sprite:update(dt)
   if self.animation then
      self.animation:update(dt)
   end

   if self.body then
      self.position = self.body:get_position()
   end
end

function Sprite:set_position(x,y)
   self.position.x = x
   self.position.y = y

   if self.body then
      self.body.position = self.position
   end
end

function Sprite.create()
   local sprite = {}
   setmetatable(sprite, Sprite)
   sprite.visible = true
   sprite.opacity = 1
   sprite.rotation = 0
   sprite.scale = 1
   sprite.position = ev_vec2()
   return sprite
end

local function fill_batch_verts(frame)
   local bv = frame.batch_verts + 0
   -- ordered for a triangle strip
   -- top left
   bv[0].x = -frame.size.w/2
   bv[0].y = -frame.size.h/2
   bv[0].u = frame.texture_rect.origin.x
   bv[0].v = frame.texture_rect.origin.y

   -- bottom left
   bv[1].x = -frame.size.w/2;
   bv[1].y = frame.size.h/2;
   bv[1].u = frame.texture_rect.origin.x;
   bv[1].v = frame.texture_rect.origin.y + frame.texture_rect.size.h;

   -- top right
   bv[2].x = frame.size.w/2;
   bv[2].y = -frame.size.h/2;
   bv[2].u = frame.texture_rect.origin.x + frame.texture_rect.size.w;
   bv[2].v = frame.texture_rect.origin.y;

   -- bottom right
   bv[3].x = frame.size.w/2;
   bv[3].y = frame.size.h/2;
   bv[3].u = frame.texture_rect.origin.x + frame.texture_rect.size.w;
   bv[3].v = frame.texture_rect.origin.y + frame.texture_rect.size.h;
end

function Sprite:set_quad(w, h, left, top, right, bottom)
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

function Sprite.init(_ev)
   ev = _ev
end

return Sprite
