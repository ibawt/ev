local Wait = {}
Wait.__index = Wait

local waiting = {}
local signals = {}

local time = 0

function Wait.wait_for(seconds)
   local co = coroutine.running()

   if not co then
      assert('not a coroutine!')
   end

   waiting[co] = time + seconds

   co.yield()
end

function Wait.wait_for_signal(signal)
   local co = coroutine.running()

   if not signals[signal] then
      signals[signal] = {}
   end

   local slot = signals[signal]

   slot[#slot+1] = co

   co.yield()
end

function Wait.wrap(fn)
   local co = courtine.create(fn)
   return coroutine.resume(co)
end

function Wait.signal(signal)
   local slot = signals[signal]

   if slot then
      for k,v in pairs(slot) do
         v.resume()
      end
   end
   signals[slot] = nil
end

function Wait.update(dt)
   time  = time + dt

   for co,t in pairs(waiting) do
      if t >= time then
         waiting[co] = nil
         co.resume()
      end
   end
end

return Wait
