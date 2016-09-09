local ev = require 'ev'
local scale = 2
local width = 144 * scale
local height = 256 * scale
local app = ev.app.create(width, height)

local m = ev.matrix4.create()
m:ortho(0, width, height, 0, 1, -1)

local graphics = ev.graphics.create()
graphics.transform = m
local sbatch = ev.sbatch.create()
sbatch:load("flappybird.json")

local texture = ev.texture.create()
texture:load("flappybird.png")

sbatch:set_texture(texture)
local sprite = sbatch:create_sprite('background1')
sprite.scale = scale
sprite:set_position(width/2, height/2)
sbatch:add_sprite(sprite)

-- local up_pipes = {}
-- for i=1,12,1 do
--    up_pipes[i] = sbatch:create_sprite('green_pipe_up')
--    up_pipes[i].visible = false
-- end

-- local down_pipes = {}
-- for i=1,12,1 do
--    down_pipes[i] = sbatch:create_sprite('green_pipe_down')
--    down_pipes[i].visible = false
--    down_pipes[i].scale = scale
-- end

-- down_pipes[1].visible = true
-- sbatch:add_sprite(down_pipes[1])


local function generate_map()
  local spacing = 100
  local pipes = {}
  local x = 0
  local y = 0
  local n = 50
  for v = 1, n do
    local s = sbatch:create_sprite('green_pipe_down')
    s.visible = true
    s.scale = scale
    s:set_position(x + (v * spacing), y)
    -- print("setting position to: " .. s.position.x .. "," .. s.position.y)
    sbatch:add_sprite(s)
    pipes[#pipes+1] = s

    s = sbatch:create_sprite('green_pipe_up')
    s.scale = scale
    s.visible = true
    s:set_position(x + v*spacing, y + height)

    sbatch:add_sprite(s)
    pipes[#pipes+1] = s
  end

  pipes.update = function(self, dt)
    for i, v in ipairs(pipes) do
      local pos = v.position
      if pos.x < -width/2 then
        pos.x = pos.x + 50 * spacing
      else
        pos.x = pos.x - 15 * dt
      end
    end
  end
  return pipes
end


local pipes = generate_map()

local bird = sbatch:create_sprite('blue_bird_2',
                                  'blue_bird_1',
                                  'blue_bird_3')
bird.visible = true
sbatch:add_sprite(bird)
bird.position.x = width/2
bird.position.y = height/2
bird.scale = scale
bird.animation.mode = bird.animation.PING_PONG
bird.y_vel = 0
bird.animation.delay = 0.1

local ground = sbatch:create_sprite('ground')
ground.scale = scale
ground.position.x = 0
ground.position.y = height - 56
sbatch:add_sprite(ground)

local ground2 = sbatch:create_sprite('ground')
ground2.scale = scale
ground2.position.x = 168
ground2.position.y = height - 56
sbatch:add_sprite(ground2)

local function update_ground(dt)
  local speed = 20
  ground.position.x = ground.position.x - speed * dt
  ground2.position.x = ground2.position.x - speed * dt

  if ground.position.x < 0 then
    ground.position.x = width
  end

  if ground2.position.x < 0 then
    ground2.position.x = width
  end
end

local function update_bird(dt)
  bird.position.y = bird.position.y + bird.y_vel * dt

  bird.y_vel = bird.y_vel + 450*dt

  if bird.y_vel < 0 then
    bird.rotation = 45
  elseif bird.y_vel > 0 and not bird.tween then
      bird.tween = ev.tween.tween(bird, 'rotation', 0, -2, 1)
  end
end

app.on_update = function(dt)
  sbatch:update(dt)
  pipes:update(dt)

  update_ground(dt)
  update_bird(dt)
end

app.on_render = function()
  graphics:clear(0, 0, 0, 1)
  sbatch:render(graphics)
end

app.on_keydown = function(key)
  if key == "Space" then
    bird.y_vel = -250
    if bird.tween then
      bird.tween:cancel()
      bird.tween = nil
    end
  end
end

app:show()
