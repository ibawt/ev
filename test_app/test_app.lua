local app = ev.app.create()

local sbatch = {}

app:set_dimensions(800,600)
app:init()

sbatch = ev.sbatch.create()
sbatch:load("bats.json")

local texture = ev.texture.create()
texture:load("bats.png")

sbatch:set_texture(texture)
for i=1,10,1 do
   local sprite = ev.sprite.create()
   local anim = ev.anim.create()
   anim:add_frame( sbatch:get_frame("bats_fly1.png"))
   anim:add_frame( sbatch:get_frame("bats_fly2.png"))
   anim:add_frame( sbatch:get_frame("bats_fly3.png"))
   sprite.animation = anim
   sprite.rotation = 0.0
   sprite.x = 400
   sprite.y = 300
   sbatch:add_sprite(sprite)
end

app:show()
