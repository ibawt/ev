local ffi = require 'ffi'

ffi.cdef[[
typedef struct {
  float m[16];
} ev_matrix4;

void ev_matrix4_set_ortho(ev_matrix4 *, float left, float right, float bottom, float top, float near, float far);
void ev_matrix4_identity(ev_matrix4 *);
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
]]
local ev = {}
ev.__index = ev

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
   'graphics',
   'wait'
}

for i,v in ipairs(modules) do
   ev[v] = require(v)
   if ev[v].init then
      ev[v].init(ev)
   end
end

return ev
