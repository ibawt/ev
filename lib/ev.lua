function ev.sbatch:create_sprite(...)
  local sprite = ev.sprite.create()
  local anim = ev.anim.create()

  for i, name in ipairs({...}) do
    anim:add_frame(self:get_frame(name))
  end

  sprite:set_animation(anim)
  self:add_sprite(sprite)
  return sprite
end

ev.keys = {
   UP = 82,
   RIGHT = 79,
   DOWN = 81,
   LEFT = 80,
   SPACE = 44,
   ENTER = 40
}
