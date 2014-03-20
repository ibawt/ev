local ffi = require('ffi')

ffi.cdef[[
typedef struct ev_app ev_app;
ev_app* ev_app_create(uint32_t w, uint32_t h);

void     ev_app_destroy(ev_app*);
uint32_t ev_app_get_height(ev_app*);
uint32_t ev_app_get_width(ev_app*);
float    ev_app_get_fps(ev_app*);
int ev_app_init(ev_app* );
void     ev_app_quit(ev_app*);
int ev_app_start(ev_app*);
]]

local app = {}

app.__index = {
   show=function(a)
      ffi.C.ev_app_start(a)
   end
}

ffi.metatype("ev_app", app)

app.create = function(width, height)
   print(width,height)
   local a = ffi.C.ev_app_create(width, height)
   ffi.C.ev_app_init(a)
   return a
end

return app
