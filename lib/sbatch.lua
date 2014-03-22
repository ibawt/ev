local ffi = require 'ffi'
local C = ffi.C

ffi.cdef[[
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
void       ev_sbatch_set_program(ev_sbatch*, ev_program* );
void       ev_sbatch_set_matrix4(ev_sbatch*, ev_matrix4 *);
ev_err_t   ev_sbatch_set_vbuff_capacity(ev_sbatch*, size_t);
void       ev_sbatch_update(ev_sbatch *, float);
]]
ffi.metatype("ev_sbatch", { __gc = function(self) C.ev_sbatch_destroy(self) end })

local SpriteBatch = {}
SpriteBatch.__index = SpriteBatch
local ev

function SpriteBatch:load(filename)
   return C.ev_sbatch_load(self._ev_sbatch, filename)
end

function SpriteBatch:get_frame(frame_name)
   return C.ev_sbatch_get_sframe(self._ev_sbatch, frame_name)
end

function SpriteBatch:set_texture(texture)
   C.ev_sbatch_set_texture(self._ev_sbatch, texture._ev_texture)
end

function SpriteBatch:add_sprite(sprite)
   return C.ev_sbatch_add_sprite(self._ev_sbatch, sprite._ev_sprite)
end

function SpriteBatch:create_sprite(...)
   local sprite = ev.sprite.create()
   local anim = ev.anim.create()
   for i,v in ipairs({...}) do
      anim:add_frame(self:get_frame(v))
   end
   sprite.animation = anim
   self:add_sprite(sprite)
   return sprite
end

function SpriteBatch:__newindex(k, v)
   if k == 'texture' then
      C.ev_sbatch_set_texture(self._ev_sbatch, v._ev_texture)
   else
      rawset(self, k, v)
   end
end

function SpriteBatch.create()
   local sbatch = {}
   setmetatable(sbatch, SpriteBatch)
   sbatch._ev_sbatch = C.ev_sbatch_create()
   return sbatch
end

function SpriteBatch.init(e)
   ev = e
   return SpriteBatch
end

return SpriteBatch
