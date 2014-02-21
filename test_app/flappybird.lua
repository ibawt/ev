function ev.sbatch:create_sprite(...)
   local sprite = ev.sprite.create()
   local anim = ev.anim.create()

   for i,name in ipairs({...}) do
      anim:add_frame(self:get_frame(name))
   end

   sprite:set_animation(anim)

   self:add_sprite(sprite)

   return sprite
end

local app = ev.app.create()

app:set_dimensions(800,600)
app:init()

local stage = ev.stage.create()
local sbatch = ev.sbatch.create()

sbatch:load("flappybird.json")

local texture = ev.texture.create()
texture:load("flappybird.png")

sbatch:set_texture(texture)
local sprite = sbatch:create_sprite('background1')
sprite:set_scale(4)
sprite:set_position(400,300)

stage:add_sbatch(sbatch)

sbatch:set_ortho(800,600)
app:set_stage(stage)

app:show()
