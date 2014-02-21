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
