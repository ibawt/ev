local ffi = require('ffi')
local C = ffi.C

ffi.cdef[[
typedef struct _ev_dir ev_dir;

ev_dir*     ev_dir_open(const char *name);
void        ev_dir_close(ev_dir *);
const char *ev_dir_next_entry(ev_dir*);
]]

local _M = {}
_M.__index = _M

function _M.dir(name)
   local d = C.ev_dir_open(name)

   if d == nil then
      return function() end
   end
   
   local entries = {}

   repeat
      local c_str = C.ev_dir_next_entry(d)
      if c_str ~= nil then
         entries[#entries+1] = ffi.string(c_str)
      end
   until c_str == nil

   C.ev_dir_close(d)
   
   local i = 0

   return function()
      i = i + 1
      if i <= #entries then
         return entries[i]
      end
   end
end

return _M
