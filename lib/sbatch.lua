local ffi = require 'ffi'

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

local M = {}
local C = ffi.C
local mt = {
   __index = {
      destroy = function(self)
         C.ev_sbatch_destroy(self)
      end,
      render = function(self)
         C.ev_sbatch_render(self)
      end,
      load = function(self, filename)
         return C.ev_sbatch_load(self, filename)
      end,
      get_frame = function(self, frame_name)
         return C.ev_sbatch_get_sframe(self, frame_name)
      end,
      set_texture = function(self, texture)
         C.ev_sbatch_set_texture(self, texture)
      end,
      add_sprite = function(self, sprite)
         return C.ev_sbatch_add_sprite(self, sprite)
      end,
      set_transform = function(self, transform)
         C.ev_sbatch_set_matrix4(self, transform)
      end,
      add_sprite = function(self, sprite)
         C.ev_sbatch_add_sprite(self, sprite)
      end
   },
   __newindex = function(self, k, v)
      if k == 'texture' then
         C.ev_sbatch_set_texture(self, v)
      end
   end
}

ffi.metatype("ev_sbatch", mt)

M.create = function()
   local s = ffi.gc(C.ev_sbatch_create(), mt.__index.destroy)
   return s
end

return M
