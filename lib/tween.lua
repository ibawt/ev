local wait = require('wait')

local _M = {}
_M.__index = {}

-- For all easing functions:
-- t = time == how much time has to pass for the tweening to complete
-- b = begin == starting property value
-- c = change == ending - beginning
-- d = duration == running time. How much time has passed *right now*

local pow, sin, cos, pi, sqrt, abs, asin = math.pow, math.sin, math.cos, math.pi, math.sqrt, math.abs, math.asin

-- linear
local function linear(t, b, c, d)
   return c * t / d + b
end

-- quad
local function in_quad(t, b, c, d)
   return c * pow(t / d, 2) + b
end

local function out_quad(t, b, c, d)
  t = t / d
  return -c * t * (t - 2) + b
end

local function in_out_quad(t, b, c, d)
  t = t / d * 2
  if t < 1 then return c / 2 * pow(t, 2) + b end
  return -c / 2 * ((t - 1) * (t - 3) - 1) + b
end
local function out_in_quad(t, b, c, d)
  if t < d / 2 then return out_quad(t * 2, b, c / 2, d) end
  return in_quad((t * 2) - d, b + c / 2, c / 2, d)
end

-- cubic
local function in_cubic (t, b, c, d) return c * pow(t / d, 3) + b end
local function out_cubic(t, b, c, d) return c * (pow(t / d - 1, 3) + 1) + b end
local function in_out_cubic(t, b, c, d)
  t = t / d * 2
  if t < 1 then return c / 2 * t * t * t + b end
  t = t - 2
  return c / 2 * (t * t * t + 2) + b
end
local function out_in_cubic(t, b, c, d)
  if t < d / 2 then return out_cubic(t * 2, b, c / 2, d) end
  return in_cubic((t * 2) - d, b + c / 2, c / 2, d)
end

-- quart
local function in_quart(t, b, c, d) return c * pow(t / d, 4) + b end
local function out_quart(t, b, c, d) return -c * (pow(t / d - 1, 4) - 1) + b end
local function in_out_quart(t, b, c, d)
  t = t / d * 2
  if t < 1 then return c / 2 * pow(t, 4) + b end
  return -c / 2 * (pow(t - 2, 4) - 2) + b
end
local function out_in_quart(t, b, c, d)
  if t < d / 2 then return out_quart(t * 2, b, c / 2, d) end
  return in_quart((t * 2) - d, b + c / 2, c / 2, d)
end

-- quint
local function in_quint(t, b, c, d) return c * pow(t / d, 5) + b end
local function out_quint(t, b, c, d) return c * (pow(t / d - 1, 5) + 1) + b end
local function in_out_quint(t, b, c, d)
  t = t / d * 2
  if t < 1 then return c / 2 * pow(t, 5) + b end
  return c / 2 * (pow(t - 2, 5) + 2) + b
end
local function out_in_quint(t, b, c, d)
  if t < d / 2 then return out_quint(t * 2, b, c / 2, d) end
  return in_quint((t * 2) - d, b + c / 2, c / 2, d)
end

-- sine
local function in_sine(t, b, c, d) return -c * cos(t / d * (pi / 2)) + c + b end
local function out_sine(t, b, c, d) return c * sin(t / d * (pi / 2)) + b end
local function in_out_sine(t, b, c, d) return -c / 2 * (cos(pi * t / d) - 1) + b end
local function out_in_sine(t, b, c, d)
  if t < d / 2 then return out_sine(t * 2, b, c / 2, d) end
  return in_sine((t * 2) -d, b + c / 2, c / 2, d)
end

-- expo
local function in_expo(t, b, c, d)
  if t == 0 then return b end
  return c * pow(2, 10 * (t / d - 1)) + b - c * 0.001
end
local function out_expo(t, b, c, d)
  if t == d then return b + c end
  return c * 1.001 * (-pow(2, -10 * t / d) + 1) + b
end
local function in_out_expo(t, b, c, d)
  if t == 0 then return b end
  if t == d then return b + c end
  t = t / d * 2
  if t < 1 then return c / 2 * pow(2, 10 * (t - 1)) + b - c * 0.0005 end
  return c / 2 * 1.0005 * (-pow(2, -10 * (t - 1)) + 2) + b
end
local function out_in_expo(t, b, c, d)
  if t < d / 2 then return out_expo(t * 2, b, c / 2, d) end
  return in_expo((t * 2) - d, b + c / 2, c / 2, d)
end

-- circ
local function in_circ(t, b, c, d) return(-c * (sqrt(1 - pow(t / d, 2)) - 1) + b) end
local function out_circ(t, b, c, d)  return(c * sqrt(1 - pow(t / d - 1, 2)) + b) end
local function in_out_circ(t, b, c, d)
  t = t / d * 2
  if t < 1 then return -c / 2 * (sqrt(1 - t * t) - 1) + b end
  t = t - 2
  return c / 2 * (sqrt(1 - t * t) + 1) + b
end
local function out_in_circ(t, b, c, d)
  if t < d / 2 then return out_circ(t * 2, b, c / 2, d) end
  return in_circ((t * 2) - d, b + c / 2, c / 2, d)
end

-- elastic
local function calculate_pas(p,a,c,d)
  p, a = p or d * 0.3, a or 0
  if a < abs(c) then return p, c, p / 4 end -- p, a, s
  return p, a, p / (2 * pi) * asin(c/a) -- p,a,s
end
local function in_elastic(t, b, c, d, a, p)
  local s
  if t == 0 then return b end
  t = t / d
  if t == 1  then return b + c end
  p,a,s = calculate_pas(p,a,c,d)
  t = t - 1
  return -(a * pow(2, 10 * t) * sin((t * d - s) * (2 * pi) / p)) + b
end
local function out_elastic(t, b, c, d, a, p)
  local s
  if t == 0 then return b end
  t = t / d
  if t == 1 then return b + c end
  p,a,s = calculate_pas(p,a,c,d)
  return a * pow(2, -10 * t) * sin((t * d - s) * (2 * pi) / p) + c + b
end
local function in_out_elastic(t, b, c, d, a, p)
  local s
  if t == 0 then return b end
  t = t / d * 2
  if t == 2 then return b + c end
  p,a,s = calculate_pas(p,a,c,d)
  t = t - 1
  if t < 0 then return -0.5 * (a * pow(2, 10 * t) * sin((t * d - s) * (2 * pi) / p)) + b end
  return a * pow(2, -10 * t) * sin((t * d - s) * (2 * pi) / p ) * 0.5 + c + b
end
local function out_in_elastic(t, b, c, d, a, p)
  if t < d / 2 then return out_elastic(t * 2, b, c / 2, d, a, p) end
  return in_elastic((t * 2) - d, b + c / 2, c / 2, d, a, p)
end

-- back
local function in_back(t, b, c, d, s)
  s = s or 1.70158
  t = t / d
  return c * t * t * ((s + 1) * t - s) + b
end
local function out_back(t, b, c, d, s)
  s = s or 1.70158
  t = t / d - 1
  return c * (t * t * ((s + 1) * t + s) + 1) + b
end
local function in_out_back(t, b, c, d, s)
  s = (s or 1.70158) * 1.525
  t = t / d * 2
  if t < 1 then return c / 2 * (t * t * ((s + 1) * t - s)) + b end
  t = t - 2
  return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b
end
local function out_in_back(t, b, c, d, s)
  if t < d / 2 then return out_back(t * 2, b, c / 2, d, s) end
  return in_back((t * 2) - d, b + c / 2, c / 2, d, s)
end

-- bounce
local function out_bounce(t, b, c, d)
  t = t / d
  if t < 1 / 2.75 then return c * (7.5625 * t * t) + b end
  if t < 2 / 2.75 then
    t = t - (1.5 / 2.75)
    return c * (7.5625 * t * t + 0.75) + b
  elseif t < 2.5 / 2.75 then
    t = t - (2.25 / 2.75)
    return c * (7.5625 * t * t + 0.9375) + b
  end
  t = t - (2.625 / 2.75)
  return c * (7.5625 * t * t + 0.984375) + b
end
local function in_bounce(t, b, c, d) return c - out_bounce(d - t, 0, c, d) + b end
local function in_out_bounce(t, b, c, d)
  if t < d / 2 then return in_bounce(t * 2, 0, c, d) * 0.5 + b end
  return out_bounce(t * 2 - d, 0, c, d) * 0.5 + c * .5 + b
end
local function out_in_bounce(t, b, c, d)
  if t < d / 2 then return out_bounce(t * 2, b, c / 2, d) end
  return in_bounce((t * 2) - d, b + c / 2, c / 2, d)
end

local tween_fns = {
   linear = linear, in_quad = in_quad, out_quad = out_quad,in_out_quad=in_out_quad,
}

local tweens = {}
tweens.count = 0

function _M.tween(options)
   tweens.count = tweens.count + 1
   tweens[tweens.count] = options
   options.time = 0
   options.begin = options.begin or 0
   options.value = tween_fns[options.curve]
end

function _M.update(dt)
   local i = 1
   while i <= tweens.count do
      local tw = tweens[i]
      tw.time = tw.time + dt
      tw.target[tw.property] = tw.value(tw.duration, tw.begin, tw.ending - tw.begin, tw.time)

      if tw.time > tw.duration then
         if tw.on_complete then
            tw.on_complete()
         end

         tweens[i] = tweens[tweens.count]
         tweens[tweens.count] = nil
         tweens.count = tweens.count - 1
      else
         i = i + 1
      end
   end
end

return _M
