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
typedef struct ev_font_system ev_font_system;
ev_app* ev_app_create(uint32_t w, uint32_t h);
void     ev_app_destroy(ev_app*);
uint32_t ev_app_get_height(ev_app*);
uint32_t ev_app_get_width(ev_app*);
float    ev_app_get_fps(ev_app*);
ev_err_t ev_app_init(ev_app* , const char *);
void     ev_app_quit(ev_app*);
ev_err_t ev_app_start(ev_app*);
int      ev_app_get_ticks(ev_app *app);
void     ev_app_swap_buffers(ev_app*);
bool     ev_app_poll_event(ev_app*, ev_event *);
]]
ffi.metatype("ev_app", { __gc = function(self) C.ev_app_destroy(self) end })

local _M = {}
_M.__index = _M

function _M:get_ticks()
   return C.ev_app_get_ticks(self._ev_app)
end

function _M:poll_event(event)
   return C.ev_app_poll_event(self._ev_app, event)
end

function _M:swap_buffers()
   C.ev_app_swap_buffers(self._ev_app)
end

function _M:keyup(event)
   assert(event.key_name, "key_name is NULL")
   local key = ffi.string(event.key_name)
   self.key_state[key] = false

   if self.on_keyup then
      self.on_keyup(key)
   end
end

function _M:keydown(event)
   assert(event.key_name, "key_name is NULL")
   local key = ffi.string(event.key_name)
   self.key_state[key] = true
   if self.on_keydown then
      self.on_keydown(key)
   end
end

function _M:quit()
   self.keep_running = false
end

function _M:show()
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
         if event.type == "EV_QUIT" then
            self.keep_running = false
         elseif event.type == "EV_KEYUP" then
            self:keyup(event)
         elseif event.type == "EV_KEYDOWN" then
            self:keydown(event)
         end
      end

      if self.on_update then
         self.on_update(dt)
      end

      if self.on_render then
         self.on_render(self.graphics)
      end

      ev.wait.update(dt)
      ev.tween.update_all(dt)

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

      self.fps = 101 * 1000 / tick_sum

      dt = dt / 1000
   end
   ev.wait.clear()
   C.ev_app_quit(self._ev_app)
end

function _M.create(name, width,height)
   local ev_app = C.ev_app_create(width, height)
   C.ev_app_init(ev_app, name)

   local app = {}
   setmetatable(app, _M)
   app._ev_app = ev_app
   app.width = width
   app.height = height
   app.graphics = ev.graphics.create()
   app.key_state = {}

   return app
end

function _M.init(_ev)
   ev = _ev
end

return _M
