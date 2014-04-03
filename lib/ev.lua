local ffi = require 'ffi'
local library = ffi.C

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
	 'particle',
	 'render_texture'
}

for i,v in ipairs(modules) do
	 ev[v] = require(v)
	 if ev[v].init then
			ev[v].init(ev, library)
	 end
end

return ev
