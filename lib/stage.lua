local ffi = require 'ffi'
local C = ffi.C

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
void      ev_stage_update(ev_stage *s, float dt);

void      ev_stage_set_transform(ev_stage *, ev_matrix4*);
void      ev_stage_render(ev_stage *s);
]]
ffi.metatype("ev_stage", { __gc = function(self) C.ev_stage_destroy(self) end })

local Stage = {}
Stage.__index = Stage

function Stage:update(dt)
   for _,v in ipairs(self.actors) do
      v:update(dt)
   end
end

function Stage:render()
   C.ev_stage_render(self._ev_stage)

   for _,v in ipairs(self.actors) do
      v:render(self.transform)
   end
end

function Stage:add_sbatch(actor)
   self.actors[#self.actors+1] = actor
end

function Stage:set_ortho(width, height)
   local m = ffi.new("ev_matrix4")
   C.ev_matrix4_set_ortho(m, 0, width, height, 1, -1, 1 )
   C.ev_stage_set_transform(self._ev_stage, m)
   self.transform = m
end

function Stage.create()
   local stage = {}
   setmetatable(stage, Stage)
   stage._ev_stage = C.ev_stage_create()
   stage.actors = {}
   return stage
end

return Stage
