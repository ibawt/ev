local ffi = require('ffi')

ffi.cdef[[
typedef enum {
    EV_NO_EVENT = 0,
    EV_QUIT,
    EV_KEYUP,
    EV_KEYDOWN
} ev_event_type;

typedef struct {
    ev_event_type type;
    int key;
    int x;
    int y;
} ev_event;

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
int      ev_app_get_ticks(ev_app *app);
void     ev_app_swap_buffers(ev_app*);
ev_event ev_app_poll_event(ev_app*);
]]
ffi.metatype("ev_app", { __gc = function(self) C.ev_app_destroy(self) end })

local C = ffi.C
local App = {}
App.__index = App

function App:get_ticks()
   return C.ev_app_get_ticks(self._ev_app)
end

function App:poll_event()
   return C.ev_app_poll_event(self._ev_app)
end

function App:swap_buffers()
   C.ev_app_swap_buffers(self._ev_app)
end

function App:show()
   local keep_running = true
   local num_frames = 0
   local start_time = self:get_ticks()
   local dt = 0

   while keep_running do
      local t1 = self:get_ticks()
      local has_events = true

      while has_events do
         local event = self:poll_event()

         if event.type == "EV_QUIT" then
            keep_running = false
         elseif event.type == "EV_NO_EVENT" then
            has_events = false
         end
      end

      local actors = {
         'stage','world'
      }

      if self.onupdate then
         self.onupdate(dt)
      end

      for i,v in ipairs(actors) do
         if self[v] then
            self[v]:update(dt)
         end
      end

      for i, v in ipairs(actors) do
         if self[v] then
            self[v]:render()
         end
      end

      self:swap_buffers()

      dt = (self:get_ticks() - t1) / 1000
      num_frames = num_frames + 1

      self.fps = num_frames / (( self:get_ticks() - start_time) / 1000)
   end
end

function App:__newindex(key, val)
   if key == 'stage' then
      C.ev_app_set_stage(self._ev_app, val._ev_stage)
   end
   rawset(self, key, val)
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
