local ev = {}

local modules = {
   'app',
   'texture',
   'stage',
   'sprite',
   'anim',
   'sbatch',
   'world',
   'body',
   'particle'
}

for i,v in ipairs(modules) do
   ev[v] = require(v)
   if ev[v].init then
      ev[v].init(ev)
   end
end

return ev
