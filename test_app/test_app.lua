local app = ev.app.create()
local sbatch = {}
local stage = ev.stage.create()

app:set_dimensions(800,600)
app:init()

app.render = function()
   print( 'this is a function')
end

sbatch = ev.sbatch.create()
sbatch:load("bats.json")

texture = ev.texture.create()
texture:load("bats.png")

sbatch:set_texture(texture)
sbatch.texture = texture
for i=1,1,1 do
   local sprite = ev.sprite.create()
   local anim = ev.anim.create()
   anim:add_frame( sbatch:get_frame("bats_fly1.png"))
   anim:add_frame( sbatch:get_frame("bats_fly2.png"))
   anim:add_frame( sbatch:get_frame("bats_fly3.png"))
   sprite:set_animation( anim )
   sprite:set_rotation( 0.0 )
   sprite:set_position(400,300)
   sbatch:add_sprite(sprite)
end

if sbatch.sprites ~= nil then
   print('foo')
   for k,v in pairs(sbatch.sprites) do
      print(k,v)
   end
end

sbatch:set_ortho(800,600)
stage:add_sbatch(sbatch)
app:set_stage(stage)

app:show()
