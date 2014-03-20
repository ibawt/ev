local ffi = require('ffi')

ffi.cdef[[
typedef enum {
    EV_OK = 0,
    EV_FAIL,
    EV_NOMEM
} ev_err_t;

typedef struct ev_app ev_app;
ev_app* ev_app_create(uint32_t w, uint32_t h);

void     ev_app_destroy(ev_app*);
uint32_t ev_app_get_height(ev_app*);
uint32_t ev_app_get_width(ev_app*);
float    ev_app_get_fps(ev_app*);
ev_err_t ev_app_init(ev_app* );
void     ev_app_quit(ev_app*);
ev_err_t ev_app_start(ev_app*);
]]

local C = ffi.C
local app = {}

local methods = {
   show=function(a)
      C.ev_app_start(a)
   end,
   quit=function(a)
      C.ev_app_quit(a)
   end,
   destroy=function(a)
      C.ev_app_destroy(a)
   end
}

local properties = {
   height = function(a)
      return C.ev_app_get_height(a)
   end,
   width = function(a)
      C.ev_app_get_width(a)
   end
}

local mt = {
   __index = function(self, key)
      return methods[key] or properties[key](self)
   end
}

ffi.metatype("ev_app", mt)

app.create = function(width, height)
   local a = ffi.gc(C.ev_app_create(width, height), methods.destroy)
   local r = C.ev_app_init(a)
   return a
end

return app
