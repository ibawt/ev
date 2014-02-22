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
