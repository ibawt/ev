local ev = require 'ev'
local app = ev.app.create(800,600)
local sbatch = {}
local stage = ev.stage.create()

stage:set_ortho(800, 600)
sbatch = ev.sbatch.create()
sbatch:load("bats.json")

texture = ev.texture.create()

if texture:load("bats.png") == 'EV_FAIL' then
   print("error in texture load")
   return
end

sbatch.texture = texture

for i=1,512,1 do
   local sprite = sbatch:create_sprite( 'bats_fly1.png',
                                        'bats_fly2.png',
                                        'bats_fly3.png')
   sprite.rotation = 0.0
   sprite:set_position(math.random(800),math.random(600))
end

stage:add_sbatch(sbatch)
app.stage = stage

app:show()
