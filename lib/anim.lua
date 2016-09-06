local _M = {}
_M.__index = _M

_M.LOOP      = 0
_M.ONE_SHOT  = 1
_M.REVERSE   = 2
_M.PING_PONG = 3

function _M:add_frame(sframe)
   self.frames[#self.frames+1] = sframe
end

function _M:update(dt)
   if self.delay <= 0 then
      return
   end

   self.time = self.time + dt

   while self.time >= self.delay do
      self.time = self.time - self.delay

      if self.mode == _M.LOOP then
         self.index = self.index + 1
         if self.index > #self.frames then
            self.index = 1
         end
      elseif self.mode == _M.ONE_SHOT then
         if (self.index + 1) < #self.frames then
            self.index = self.index + 1
         end
      elseif self.mode == _M.REVERSE then
         if self.index > 1 then
            self.index = self.index - 1
         end
      end
   end
end

function _M:current_frame()
   return self.frames[self.index]
end

function _M.create()
   local anim = {}
   setmetatable(anim, _M)
   anim.frames = {}
   anim.time = 0
   anim.index = 1
   anim.delay = 0.3
   anim.mode = _M.LOOP

   return anim
end

return _M
