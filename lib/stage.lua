local ffi = require 'ffi'

ffi.cdef[[
typedef struct ev_stage ev_stage;

ev_stage* ev_stage_create(void);
void      ev_stage_destroy(ev_stage *s);
]]
