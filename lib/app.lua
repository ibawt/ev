local ffi = require('ffi')
local C = ffi.C

ffi.cdef[[
typedef enum {
    EV_NO_EVENT = 0,
    EV_QUIT,
    EV_KEYUP,
    EV_KEYDOWN
} ev_event_type;

typedef struct {
    ev_event_type type;
    uint32_t key;
    const char *key_name;
    int x;
    int y;
} ev_event;

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
int      ev_app_get_ticks(ev_app *app);
void     ev_app_swap_buffers(ev_app*);
bool     ev_app_poll_event(ev_app*, ev_event *);
]]
ffi.metatype("ev_app", { __gc = function(self) C.ev_app_destroy(self) end })

local App = {}
App.__index = App

function App:get_ticks()
   return C.ev_app_get_ticks(self._ev_app)
end

function App:poll_event(event)
   return C.ev_app_poll_event(self._ev_app, event)
end

function App:swap_buffers()
   C.ev_app_swap_buffers(self._ev_app)
end

function App:keyup(event)
   assert(event.key_name, "key_name is NULL")
   local key = ffi.string(event.key_name)
   self.key_state[key] = false

   if self.onkeyup then
      self.onkeyup(key)
   end
end

function App:keydown(event)
   assert(event.key_name, "key_name is NULL")
   local key = ffi.string(event.key_name)
   self.key_state[key] = true
   if self.onkeydown then
      self.onkeydown(key)
   end
end

function App:quit()
   self.keep_running = false
end

function App:show()
   self.keep_running = true
   local num_frames = 0
   local start_time = self:get_ticks()
   local dt = 0
   local event = ffi.new("ev_event")
   
   local tick_list = {}
   for i=1,101 do
      tick_list[i] = 0
   end
   local tick_index = 1
   local tick_sum = 0
   
   while self.keep_running do
      local t1 = self:get_ticks()

      while self:poll_event(event) do
         -- can't do a table index with an enum with ffi :(
         if event.type == "EV_QUIT" then
            keep_running = false
         elseif event.type == "EV_KEYUP" then
            self:keyup(event)
         elseif event.type == "EV_KEYDOWN" then
            self:keydown(event)
         end
      end

      if self.onupdate then
         self.onupdate(dt)
      end

      if self.onrender then
         self.onrender(self.graphics)
      end

      ev.wait.update(dt)

      self:swap_buffers()

      dt = (self:get_ticks() - t1)
      num_frames = num_frames + 1

      -- dt ms per frame
      tick_sum = tick_sum - tick_list[tick_index] + dt
      tick_list[tick_index] = dt
      tick_index = (tick_index + 1)
      if tick_index > 101 then
         tick_index = 1
      end

      self.fps = (1 / (tick_sum / 101)) * 1000

      dt = dt / 1000
   end
   
   C.ev_app_quit(self._ev_app)
end

function App.create(width,height)
   local ev_app = C.ev_app_create(width, height)
   C.ev_app_init(ev_app)

   local app = {}
   setmetatable(app, App)
   app._ev_app = ev_app
   app.width = width
   app.height = height

   app.graphics = ev.graphics.create()

   app.key_state = {}

   return app
end

function App.init(_ev)
   ev = _ev
end

return App
