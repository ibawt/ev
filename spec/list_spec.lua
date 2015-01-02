describe('list tests', function()
   local list

   before_each(function()
      list = ev.list.create()
   end)

   it('add deletion and iteration', function()
      for i=1,10 do
         list:push(i)
      end
      
      local it = list:iter()
      for i,v in it.each do
         if v % 2 == 0 then
            it.delete()
         end
      end

      assert.are.same(5, list:len())

      local t = { 1, 3, 5, 7, 9 }

      local i = 1

      it = list:iter()
      local actual = {}
      local ii = 0
      for i,v in it.each do
         table.insert(actual, v)
         ii = ii + 1
         assert.are.same(ii, i)
      end

      table.sort(actual)

      assert.are.same(t, actual)
   end)
end)
