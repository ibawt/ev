local ev = require 'ev'
local app = ev.app.create(800,600)

local stage = ev.stage.create()
local sbatch = ev.sbatch.create()

sbatch:load("flappybird.json")

local texture = ev.texture.create()
texture:load("flappybird.png")

sbatch:set_texture(texture)
local sprite = sbatch:create_sprite('background1')
sprite.scale = 4
sprite:set_position(400,300)

local up_pipes = {}
for i=1,12,1 do
   up_pipes[i] = sbatch:create_sprite('green_pipe_up')
   up_pipes[i].visible = 0
end

local down_pipes = {}
for i=1,12,1 do
   down_pipes[i] = sbatch:create_sprite('green_pipe_down')
   down_pipes[i].visible = 0
end


app.update = function(dt)
   print("update: " .. dt)
end

stage:add_sbatch(sbatch)

stage:set_ortho(800,600)
app.stage = stage

app:show()
