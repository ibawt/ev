local ev = {}

local modules = {
   'app',
   'stage',
   'sbatch',
   'texture',
   'sprite',
   'anim'
}

for i,v in ipairs(modules) do
   ev[v] = require(v)
   if ev[v].init then
      ev[v].init(ev)
   end
end

return ev
