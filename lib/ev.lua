local ffi = require 'ffi'

ffi.cdef[[
typedef int ev_bool;
typedef union {
   float m[16];
    struct {
        float m00, m10, m20, m30;
        float m01, m11, m21, m31;
        float m02, m12, m22, m32;
        float m03, m13, m23, m33;
    };
} ev_matrix4;

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
   'wait',
   'vec2',
   'matrix4',
   'font',
   'utils',
   'tween',
   'list'
}

for i,v in ipairs(modules) do
   ev[v] = require(v)
   if type(ev[v]) == 'table' and ev[v].init then
      ev[v].init(ev)
   end
end

return ev
