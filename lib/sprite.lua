local ffi = require 'ffi'
local C
local ev

ffi.cdef[[
typedef int ev_bool;
      
typedef struct {
    float x;
    float y;
    float u;
    float v;
    float rotation;
    float scale;
    float tx;
    float ty;
    float opacity;
} ev_bvertex;

typedef struct {
   float x;
   float y;
} ev_vec2;

typedef struct ev_anim ev_anim;
typedef struct {
    float w;
    float h;
} ev_size;

typedef struct {
    float x,y;
} ev_vec2;

typedef struct {
    ev_vec2 origin;
    ev_size size;
} ev_rect;

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

typedef struct {
  ev_vec2  position;
  float    rotation;
  float    scale;
  int      visible;
  float    opacity;
  ev_anim *animation;
  ev_body *body;
               } ev_sprite;

ev_sframe *   ev_sframe_create_quad(float w, float h, float left,float  top,float right,float  bottom);
void          ev_sprite_init(ev_sprite *);
ev_vec2*      ev_sprite_get_position(ev_sprite*);
void          ev_sprite_set_position(ev_sprite*, float x, float y);
float         ev_sprite_get_rotation(ev_sprite*);
void          ev_sprite_set_rotation(ev_sprite*, float);
void          ev_sprite_set_animation(ev_sprite*, ev_anim*);
ev_anim*      ev_sprite_get_animation(ev_sprite*);
void          ev_sprite_update(ev_sprite*, float);
void          ev_sprite_render(ev_sprite*);
void          ev_sprite_set_body(ev_sprite *, ev_body *);
void          ev_sprite_set_quad(ev_sprite *sprite, float w, float h, float left, float top, float right, float bottom);
int           ev_sprite_fill(ev_sprite*, ev_bvertex*);
]]
local ev_sprite = ffi.metatype("ev_sprite", {})

local Sprite = {}
Sprite.__index = Sprite

local setters = {
   body = function(self, val)
    C.ev_sprite_set_body(self._ev_sprite, val._ev_body)
    rawset(self, 'body', val)
   end,
}

function Sprite:fill(dst)
   -- need to force it to a pointer
   local src = self.animation:current_frame().batch_verts + 0
   
   local r = self.rotation or 0
   local s = self.scale or 1
   local p = self.position
   local o = self.opacity or 1
   
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
   C.ev_sprite_update(self._ev_sprite, dt)

   if self.animation then
      self.animation:update(dt)
   end
end

function Sprite:__index(key)
   if key == 'position' then
      return C.ev_sprite_get_position(self._ev_sprite)
   else
      return getmetatable(self)[key] or rawget(self, key)
   end
end

function Sprite:set_position(x,y)
   C.ev_sprite_set_position(self._ev_sprite, x, y)
end

function Sprite:__newindex(key, val)
   if setters[key] then
      setters[key](self,val)
   else
      rawset(self, key, val)
   end
end

function Sprite.create()
   local ev_sprite = ffi.new('ev_sprite')
   C.ev_sprite_init(ev_sprite)
   local sprite = {}
   setmetatable(sprite, Sprite)
   sprite.visible = true
   sprite.opacity = 1
   sprite.rotation = 0
   sprite.scale = 1
   sprite._ev_sprite = ev_sprite
   return sprite
end

function Sprite:set_quad(w, h, left, top, right, bottom)
   if not self.animation then
      self.animation = ev.anim.create()
   end
   self.animation:add_frame(C.ev_sframe_create_quad(w,h,left,top,right,bottom))
end

function Sprite.init(_ev, lib)
   ev = _ev
   C = lib
end

return Sprite

