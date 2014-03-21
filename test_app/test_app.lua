local ev = require 'ev'
local app = ev.app.create(800,600)
local sbatch = {}
local stage = ev.stage.create()

stage:set_ortho(800, 600)
sbatch = ev.sbatch.create()
sbatch:load("bats.json")

texture = ev.texture.create()
texture:load("bats.png")

sbatch.texture = texture

for i=1,512,1 do
   local sprite = ev.sprite.create()
   local anim = ev.anim.create()
   anim:add_frame( sbatch:get_frame("bats_fly1.png"))
   anim:add_frame( sbatch:get_frame("bats_fly2.png"))
   anim:add_frame( sbatch:get_frame("bats_fly3.png"))
   sprite.animation = anim
   sprite.rotation = 0.0
   sprite:set_position(math.random(800),math.random(300))
   sbatch:add_sprite(sprite)
end

stage:add_sbatch(sbatch)
app.stage = stage

app:show()
