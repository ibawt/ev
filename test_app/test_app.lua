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


local world = ev.world.create()
world:set_dimensions(800, 600)

local system = ev.particle.system.create(world)
local grp = ev.particle.group.create(system)

app.world = world
world.debug_draw = true

sbatch.texture = texture
local bats = {}
for i=1,1,1 do
   local sprite = sbatch:create_sprite( 'bats_fly1.png',
                                        'bats_fly2.png',
                                        'bats_fly3.png')
   sprite.rotation = 0.0
   sprite.body = ev.body.create(world, sprite)
   sprite.body.shape = {
      shape = 'circle',
      radius = 5
   }
   sprite:set_position(math.random(800),math.random(600))
   bats[#bats+1] = sprite
end

local function echo_wave()
   grp:destroy_particles()

   local pos = bats[1].position
   local theta = 0
   local iter = 2048
   local radius = 20
   local inc = 2 * math.pi / iter
   local vel = 200
   for i=1,iter do
      local pos = { x=pos.x + math.cos(theta)*radius, y=pos.y + math.sin(theta)*radius}
      system:spawn_particle( {
            position =pos ,
            vel = { x=math.cos(theta)*vel, y=math.sin(theta)*vel },
            group=grp
      })
      theta = theta + iter
   end
end

local function spawn_light(pos)

end

local function check_wave(dt)
   local contacts = system:get_body_contacts()

   for i,bc in ipairs(contacts) do
      local part = bc.index
      local body = bc.body
      system:destroy_particle(part)
      -- local tag = body.tag
      -- print('collided with a ' .. tag)
      -- spawn_light(body.position)
   end
end

app.onupdate = function(dt)
   check_wave(dt)
end

app.onkeydown = function(key)
   print('keydown: ' .. key )
end

app.onkeyup = function(key)
   print('keydown: ' .. key )

   if key == 'Return' then
      echo_wave()
   end
end

stage:add_sbatch(sbatch)
app.stage = stage
app:show()
