local ffi = require('ffi')

ffi.cdef[[
typedef enum {
    EV_OK = 0,
    EV_FAIL,
    EV_NOMEM
} ev_err_t;

typedef struct ev_app ev_app;
typedef struct ev_stage ev_stage;

ev_app* ev_app_create(uint32_t w, uint32_t h);
void ev_app_set_stage(ev_app *app, ev_stage *s);
void     ev_app_destroy(ev_app*);
uint32_t ev_app_get_height(ev_app*);
uint32_t ev_app_get_width(ev_app*);
float    ev_app_get_fps(ev_app*);
ev_err_t ev_app_init(ev_app* );
void     ev_app_quit(ev_app*);
ev_err_t ev_app_start(ev_app*);
]]
ffi.metatype("ev_app", { __gc = function(self) C.ev_app_destroy(self) end })

local C = ffi.C
local App = {}
App.__index = App


function App:show()
   C.ev_app_start(self._ev_app)
end

function App:__newindex(key, val)
   if key == 'stage' then
      C.ev_app_set_stage(self._ev_app, val._ev_stage)
   else
      rawset(self, key, val)
   end
end

function App.create(width,height)
   local ev_app = C.ev_app_create(width, height)
   C.ev_app_init(ev_app)

   local app = {}
   setmetatable(app, App)
   app._ev_app = ev_app
   return app
end

return App
