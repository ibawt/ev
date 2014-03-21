local ffi = require 'ffi'

ffi.cdef[[
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

ev_sprite*    ev_sprite_create(void);
ev_vec2*      ev_sprite_get_position(ev_sprite*);
void          ev_sprite_set_position(ev_sprite*, float x, float y);
float         ev_sprite_get_rotation(ev_sprite*);
void          ev_sprite_set_rotation(ev_sprite*, float);
void          ev_sprite_set_animation(ev_sprite*, ev_anim*);
ev_anim*      ev_sprite_get_animation(ev_sprite*);
void          ev_sprite_update(ev_sprite*, float);
void          ev_sprite_render(ev_sprite*);
void          ev_sprite_set_body(ev_sprite *, ev_body *);
]]

local M = {}
local C = ffi.C

local setters = {
   animation = function(self, val)
      C.ev_sprite_set_animation(self, val)
   end,
   rotation = function(self, val)
      C.ev_sprite_set_rotation(self, val)
   end
}

local mt = {
   __index = {
      set_position = function(self, x, y)
         C.ev_sprite_set_position(self, x, y)
      end
   },
   __newindex = function(self, key, val)
      setters[key](self, val)
   end,
   __gc = function(self)
      print('sprite gc')
   end
}

ffi.metatype("ev_sprite", mt)

M.create = function()
   return ffi.C.ev_sprite_create()
end

return M
