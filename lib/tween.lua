local Tween = {}
Tween.__index = Tween

local tweens = {}

function Tween:update(dt)
  self.elapsed = self.elapsed + dt

  local v = (self.e - self.s)
  local vv = v * (self.elapsed / self.time )

  self.obj[self.field] = vv

  if self.elapsed > self.time then
    self.obj[self.field] = self.e
    tweens[self] = nil
  end
end

function Tween:cancel()
  tweens[self] = nil
end

function Tween.tween(obj, field, s, e, time)
  local t = { obj=obj, field=field, s=s, e=e, time=time, elapsed = 0 }
  setmetatable(t, Tween)

  tweens[t] = t

  return t
end

function Tween.update_all(dt)
  for _,v in pairs(tweens) do
    v:update(dt)
  end
end

return Tween
