local ev = require 'ev'
local app = ev.app.create(800,600)

local m = ev.matrix4.create()
m:ortho(0, 800, 600, 0, 1, -1)

local graphics = ev.graphics.create()
graphics.transform = m
local sbatch = ev.sbatch.create()
sbatch:load("flappybird.json")

local texture = ev.texture.create()
texture:load("flappybird.png")

sbatch:set_texture(texture)
local sprite = sbatch:create_sprite('background1')
sprite.scale = 4
sprite:set_position(400,300)
sbatch:add_sprite(sprite)

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

app.on_update = function(dt)
  sbatch:update(dt)
end

app.on_render = function()
  graphics:clear(0, 0, 0, 1)
  sbatch:render(graphics)
end

app:show()
