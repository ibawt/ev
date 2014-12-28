local ev = require('ev')

describe('the dir function', function()
   setup(function()
      os.execute('mkdir -p /tmp/foobar/bar/foo')
      os.execute('mkdir -p /tmp/foobar/baz')
      os.execute('touch /tmp/foobar/foo.txt')
   end)
   
   it('dir will give back an iterator to list the files', function()
      local exepcted = { '.', '..', 'bar', 'baz', 'foo.txt' }

      local actual = {}
      for v in ev.utils.dir('/tmp/foobar') do
         actual[#actual+1] = v
      end

      assert.are.same(table.sort(exepcted), table.sort(actual))
   end)

   it('will give back an empty list for a directory that doesnt exist', function()
      for dir in ev.utils.dir('/fdsfdsfd') do
         assert.is_true(false)
      end
   end)
end)

