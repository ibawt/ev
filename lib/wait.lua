local _M = {}
_M.__index = _M

local waiting = {}
local signals = {}

local time = 0

function _M.wait_for(seconds)
   local co = coroutine.running()

   assert(co, 'not a coroutine!')

   waiting[co] = time + seconds

   coroutine.yield()
end

function _M.wait_for_signal(signal)
   local co = coroutine.running()

   if not signals[signal] then
      signals[signal] = {}
   end

   local slot = signals[signal]

   slot[#slot+1] = co

   coroutine.yield()
end

function _M.wrap(fn)
   local co = coroutine.create(fn)
   return coroutine.resume(co)
end

function _M.signal(signal)
   local slot = signals[signal]

   if slot then
      for k,v in pairs(slot) do
         v.resume()
      end
   end
   signals[slot] = nil
end

-- calls fn after to seconds
function _M.set_timeout(to, fn)
   local to_fn = function()
      _M.wait_for(to)
      fn()
   end
   _M.wrap(to_fn)
end

-- calls fn every interval seconds
-- return: a function to close the interval
-- as well if fn returns false then close the interval
function _M.set_interval(interval, fn)
   local keep_running = true
   local to_fn = function()
      while keep_running do
         _M.wait_for(interval)
         local r = fn()
         if r == false then
            keep_running = false
         end
      end
   end

   _M.wrap(to_fn)

   return function()
      keep_running = false
   end
end

function _M.update(dt)
   time = time + dt

   for co,t in pairs(waiting) do
      if time >= t then
         waiting[co] = nil
         coroutine.resume(co)
      end
   end
end

function _M.clear()
   time = 0
   waiting = {}
   signals = {}
end

return _M
