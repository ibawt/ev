local Tween = {}
Tween.__index = Tween

local tweens = {}

function Tween.lerp(x1, x2, ct, tt)
  return (x2 - x1) * (ct / tt )
end

function Tween.quad_ease_in(x1, x2, ct, tt)
  local t = ct / tt
  return (x2 - x1) * t * t + x1
end

function Tween.quad_ease_out(x1, x2, ct, tt)
  local t = ct / tt
  return -(x2 - x1) * t * (t - 2) + x1
end

function Tween:update(dt)
  self.elapsed = self.elapsed + dt

  self.object[self.field] = self[self.tween](self.s, self.e, self.elapsed, self.time)

  if self.elapsed > self.time then
    self.object[self.field] = self.e
    tweens[self] = nil
  end
end

function Tween:cancel()
  tweens[self] = nil
end

function Tween.tween(t)
  t.elapsed = 0
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
