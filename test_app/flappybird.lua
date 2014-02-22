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

local up_pipes = {}
for i=1,12,1 do
   up_pipes[i] = sbatch:create_sprite('green_pipe_up')
   up_pipes[i]:set_visiblity(false)
end

local down_pipes = {}
for i=1,12,1 do
   down_pipes[i] = sbatch:create_sprite('green_pipe_down')
   down_pipes[i]:set_visiblity(false)
end


app.update = function(dt)
   print("update: " .. dt)
end

stage:add_sbatch(sbatch)

sbatch:set_ortho(800,600)
app:set_stage(stage)

app:show()
