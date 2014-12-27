local ev = require('ev')
local vec2 = ev.vec2

local function float_equals(a,b)
   assert(math.abs(b - a) < 0.0001, string.format('Expected %f, actual %f', a, b))
end

describe('vec2 test cases', function()
   it('equals', function()
      local v1 = vec2.create(0,0)
      local v2 = vec2.create(0,0)
      assert.equals(v1, v2)
      assert.equals(v2, v1)
      v1.x = 5
      v1.y = 5
      v2.x = 5
      v2.y = 5

      assert.equals(v1, v2)
      assert.equals(v2, v1)

      v1.x = 10

      assert.not_equals(v1,v2)
   end)
   it('translate', function()
      local v = vec2.create(0,0)
      v:translate(5,5)

      assert.equals(v, vec2.create(5,5))

      v = vec2.create(0,0)
      v:translate(-5,-5)
      assert.equals(v, vec2.create(-5,-5))
   end)

   it('add', function()
      local v1 = vec2.create(0,0)
      local v2 = vec2.create(5,5)

      local v3 = v1 + v2

      assert.equals(v3, vec2.create(5,5))
   end)

   it('sub', function()
      local v1 = vec2.create(5,5)
      local v2 = vec2.create(4,4)

      local v3 = v1 - v2

      assert.equals(v3, vec2.create(1,1))
   end)

   it('len', function()
      local v1 = vec2.create(3,3)

      float_equals(4.2426, #v1)
   end)

   it('normalize', function()
      local v = vec2.create(0,10)

      v:normalize()

      assert.equals(vec2.create(0,1), v)

      v = vec2.create(10,10)

      v:normalize()
      assert.equals(vec2.create(0.7071, 0.7071), v)
   end)

   it('scale', function()
      local v = vec2.create(0,0)

      v:scale(5,5)

      assert.equals(vec2.create(0,0), v)

      v = vec2.create(2,2)

      v:scale(5,5)

      assert.equals(vec2.create(10,10), v)

      v = vec2.create(2,2)
      v:scale(5)

      assert.equals(vec2.create(10,10), v)
   end)

   it('angle', function()
      local function va(x,y)
         return vec2.create(x,y):angle()
      end
      
      float_equals(0, va(0,1)) -- top
      float_equals(math.pi/4, va(1,1))
      float_equals(math.pi/2, va(1,0))
      float_equals(math.pi*3/4, va(1,-1))
      float_equals(math.pi, va(0,-1))
      float_equals(math.pi*5/4, va(-1,-1))
      float_equals(math.pi*6/4, va(-1,0))
      float_equals(math.pi*7/4, va(-1,1))
   end)

   it('clamp', function()
      local v = vec2.create(5,5)
      v:clamp(1,5)

      float_equals(5, #v)

      v = vec2.create(0,0.2)
      v:clamp(1,5)

      float_equals(1, #v)
   end)
end)
