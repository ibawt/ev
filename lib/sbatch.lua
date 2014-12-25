local ffi = require 'ffi'
local C = ffi.C
local ev

ffi.cdef[[
typedef enum {
GL_ZERO = 0,
GL_ONE = 1,
GL_SRC_COLOR = 0x0300,
GL_ONE_MINUS_SRC_COLOR = 0x0301,
GL_SRC_ALPHA = 0x0302,
GL_ONE_MINUS_SRC_ALPHA = 0x0303,
GL_DST_ALPHA = 0x0304,
GL_ONE_MINUS_DST_ALPHA = 0x0305,
GL_DST_COLOR =  0x0306,
GL_ONE_MINUS_DST_COLOR = 0x0307,
GL_SRC_ALPHA_SATURATE = 0x0308
} blend;

typedef struct {
  int src;
  int dst;
} ev_blend_func;

typedef struct ev_sbatch ev_sbatch;
typedef struct ev_sprite ev_sprite;
typedef struct ev_program ev_program;
typedef struct ev_matrix4 ev_matrix4;
typedef struct ev_sframe ev_sframe;
typedef struct ev_texture ev_texture;

ev_sbatch* ev_sbatch_create(void);
void       ev_sbatch_destroy(ev_sbatch*);
void       ev_sbatch_render(ev_sbatch*, ev_matrix4* );
ev_err_t   ev_sbatch_load(ev_sbatch*, const char*);
ev_sframe* ev_sbatch_get_sframe(ev_sbatch*, const char* );
void       ev_sbatch_set_texture(ev_sbatch*, ev_texture*);
ev_err_t   ev_sbatch_add_sprite(ev_sbatch*, ev_sprite*);
void       ev_sbatch_destroy_sprite(ev_sbatch *s, int );
void       ev_sbatch_set_program(ev_sbatch*, ev_program* );
void       ev_sbatch_set_matrix4(ev_sbatch*, ev_matrix4 *);
ev_err_t   ev_sbatch_set_vbuff_capacity(ev_sbatch*, size_t);
int        ev_sbatch_get_vbuff_capacity(ev_sbatch*);
void       ev_sbatch_update(ev_sbatch *, float);
int        ev_sbatch_num_filled_sprites(ev_sbatch *s);
void       ev_sbatch_set_blend_func(ev_sbatch *s, blend dst, blend src);
ev_bvertex* ev_sbatch_lock(ev_sbatch *batch);
void       ev_sbatch_unlock(ev_sbatch*, int);
]]
ffi.metatype("ev_sbatch", { __gc = function(self) C.ev_sbatch_destroy(self) end })

local _M = {}
_M.__index = _M
local ev

function _M:load(filename)
   return C.ev_sbatch_load(self._ev_sbatch, filename)
end

function _M:get_frame(frame_name)
   return C.ev_sbatch_get_sframe(self._ev_sbatch, frame_name)
end

function _M:set_blend_func(dst, src)
   C.ev_sbatch_set_blend_func(self._ev_sbatch, dst, src)
end

function _M:update(dt)
   for _, sprite in ipairs(self.sprites) do
      sprite:update(dt)
   end
end

function _M:set_texture(texture)
   C.ev_sbatch_set_texture(self._ev_sbatch, texture._ev_texture)
end

function _M:destroy_sprite(index)
   table.remove(self.sprites, index)
end

function _M:add_sprite(sprite)
   local cap = C.ev_sbatch_get_vbuff_capacity(self._ev_sbatch)
   if cap <= #self.sprites + 1 then
      local err = C.ev_sbatch_set_vbuff_capacity(self._ev_sbatch, math.floor(cap * 3 / 2))
      if err then
         return err
      end
   end
   self.sprites[#self.sprites+1] = sprite
end

function _M:set_capacity(num_sprites)
   C.ev_sbatch_set_vbuff_capacity(self._ev_sbatch, num_sprites)
end

function _M:lock()
   return C.ev_sbatch_lock(self._ev_sbatch)
end

function _M:unlock(n)
   C.ev_sbatch_unlock(self._ev_sbatch, n/6)
end

function _M:render(g)
   local verts = self:lock()
   local n = 0
   for _, sprite in ipairs(self.sprites) do
      n = n + sprite:fill(verts + n)
   end
   
   self:unlock(n)
   
   self:draw(g)
end

function _M:render_iter(g, iter)
   local verts = self:lock()
   local n = 0
   for sprite in iter() do
      n = n + sprite:fill(verts + n)
   end
   self:unlock(n)

   self:draw(g)
end

function _M:draw(g)
   C.ev_sbatch_render(self._ev_sbatch, g.transform)
end

function _M:filled_sprites()
   return C.ev_sbatch_num_filled_sprites(self._ev_sbatch)
end

function _M:create_sprite(...)
   local sprite = ev.sprite.create()
   local anim = ev.anim.create()
   for i,v in ipairs({...}) do
      anim:add_frame(self:get_frame(v))
   end
   sprite.animation = anim
   
   return sprite
end

function _M:__newindex(k, v)
   if k == 'texture' then
      C.ev_sbatch_set_texture(self._ev_sbatch, v._ev_texture)
   else
      rawset(self, k, v)
   end
end

function _M.create()
   local sbatch = {}
   setmetatable(sbatch, _M)
   sbatch._ev_sbatch = C.ev_sbatch_create()
   sbatch.sprites = {}
   sbatch.name = "Sprite Batch"
   return sbatch
end

function _M.init(e)
   ev = e
end

return _M
