local _M = {}
_M.__index = _M

function _M:push(v)
   self.count = self.count + 1
   self.items[self.count] = v
end

function _M:pop()
   if self.count > 0 then
      local v = self.items[self.count]
      self.count = self.count - 1
      return v
   end
end

function _M:len()
   return self.count
end

function _M:iter()
   local i = 0

   return {
      each=function()
         if (i+1) <= self.count then
            i = i + 1
            return i, self.items[i]
         end
      end,
      delete=function()
         if i <= self.count then
            self.items[i] = self.items[self.count]
            i = i - 1
            self.items[self.count] = nil
            self.count = self.count - 1
         end
      end
   }
end

function _M:clear()
   self.count = 0
   self.items = {}
end

function _M.create()
   local l = {
      items={},
      count=0
   }
   setmetatable(l, _M)
   return l
end

return _M
