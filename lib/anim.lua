local Anim = {}
Anim.__index = Anim

Anim.LOOP      = 0
Anim.ONE_SHOT  = 1
Anim.REVERSE   = 2
Anim.PING_PONG = 3

function Anim:add_frame(sframe)
   self.frames[#self.frames+1] = sframe
end

function Anim:update(dt)
   if self.delay <= 0 then
      return
   end

   self.time = self.time + dt

   while self.time >= self.delay do
      self.time = self.time - self.delay

      if self.mode == Anim.LOOP then
         self.index = self.index + 1
         if self.index > #self.frames then
            self.index = 1
         end
      elseif self.mode == Anim.ONE_SHOT then
         if (self.index + 1) < #self.frames then
            self.index = self.index + 1
         end
      elseif self.mode == Anim.REVERSE then
         if self.index > 1 then
            self.index = self.index - 1
         end
      end
   end
end

function Anim:current_frame()
   return self.frames[self.index]
end

function Anim.create()
   local anim = {}
   setmetatable(anim, Anim)
   anim.frames = {}
   anim.time = 0
   anim.index = 1
   anim.delay = 0.3
   anim.mode = Anim.LOOP
   
   return anim
end

return Anim
