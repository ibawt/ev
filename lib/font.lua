local ffi = require('ffi')
local C = ffi.C

ffi.cdef[[
typedef struct _ev_font ev_font;

ev_font* ev_font_create(const char *name, float pt_size);
void     ev_font_destroy(ev_font*);
void     ev_font_set_position(ev_font*, ev_vec2);
ev_vec2  ev_font_get_position(ev_font*);
float    ev_font_set_text(ev_font *font, const char *text, int len);
void     ev_font_render(ev_font *, ev_matrix4 *);
void     ev_font_set_colour(ev_font *font, float r, float g, float b, float a);
]]

ffi.metatype("ev_font", { __gc = function(self) C.ev_font_destroy(self) end })

local _M = {}
_M.__index = _M

function _M.create(filename, size)
   local font = {}
   local ev_font = C.ev_font_create(filename, size)
   
   font._ev_font = ev_font

   setmetatable(font, _M)

   return font
end

function _M:set_colour(r,g,b,a)
   C.ev_font_set_colour(self._ev_font, r, g, b, a)
end

function _M:set_position(x, y)
   local p = ev.vec2.create()
   p.x = x
   p.y = y
   C.ev_font_set_position(self._ev_font, p)
end

function _M:get_position()
   C.ev_font_get_position(self._ev_font)
end

function _M:set_text(text)
   self.width = C.ev_font_set_text(self._ev_font, text, #text)
end

function _M:render(g)
   C.ev_font_render(self._ev_font, self.transform)
end

return _M
