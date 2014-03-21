local ffi = require 'ffi'

ffi.cdef[[
typedef struct {
    float m[16];
} ev_matrix4;

void ev_matrix4_set_ortho( ev_matrix4 *, float left, float right, float bottom, float top, float near, float far);
typedef void (*ev_stage_render_fn)(void *d, ev_matrix4 *);
typedef void (*ev_stage_update_fn)(void *, float);

typedef struct ev_stage ev_stage;

ev_stage* ev_stage_create(void);
void      ev_stage_destroy(ev_stage *s);
void      ev_stage_add_actor(ev_stage *s, ev_stage_render_fn func, ev_stage_update_fn update_fn, void *d);

void      ev_stage_set_transform(ev_stage *, ev_matrix4*);
]]

local _M = {}
local C = ffi.C
local mt = {
   __index = {
      destroy=function(a)
         C.ev_stage_destroy(a)
      end,
      add_sbatch=function(self, sbatch)
         C.ev_stage_add_actor(self, C.ev_sbatch_render, C.ev_sbatch_update, sbatch)
      end,
      set_ortho = function(self, width, height)
         local m = ffi.new("ev_matrix4")
         C.ev_matrix4_set_ortho(m, 0, width, height, 1, -1, 1 )
         C.ev_stage_set_transform(self, m)
      end
   }
}

ffi.metatype("ev_stage", mt)

_M.create = function()
   local stage = ffi.gc(C.ev_stage_create(), mt.__index.destroy)
   return stage
end

return _M
