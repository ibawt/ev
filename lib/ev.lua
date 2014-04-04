local ffi = require 'ffi'
local library = ffi.C

ffi.cdef[[
typedef struct {
 float m[16];
} ev_matrix4;

void ev_matrix4_set_ortho(ev_matrix4 *,
                          float left, float right, float bottom, float top,
                          float near, float far);
void ev_matrix4_identity(ev_matrix4 *);
]]

local ev = {}

local modules = {
   'app',
   'texture',
   'sprite',
   'anim',
   'sbatch',
   'world',
   'body',
   'particle',
   'render_texture',
   'graphics'
}

for i,v in ipairs(modules) do
   ev[v] = require(v)
   if ev[v].init then
      ev[v].init(ev, library)
   end
end

return ev
