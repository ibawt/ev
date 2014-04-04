local ffi = require 'ffi'
local C
local ev

ffi.cdef[[
typedef struct {
	 float x;
	 float y;
} ev_vec2;

typedef struct ev_anim ev_anim;
typedef struct {
		float w;
		float h;
} ev_size;

typedef struct {
		float x,y;
} ev_vec2;

typedef struct {
		ev_vec2 origin;
		ev_size size;
} ev_rect;

typedef struct ev_body ev_body;

typedef struct {
	ev_vec2  position;
	float    rotation;
	float    scale;
	int      visible;
	float    opacity;
	ev_anim *animation;
	ev_body *body;
} ev_sprite;

ev_sprite*    ev_sprite_create(void);
ev_vec2*      ev_sprite_get_position(ev_sprite*);
void          ev_sprite_set_position(ev_sprite*, float x, float y);
float         ev_sprite_get_rotation(ev_sprite*);
void          ev_sprite_set_rotation(ev_sprite*, float);
void          ev_sprite_set_animation(ev_sprite*, ev_anim*);
ev_anim*      ev_sprite_get_animation(ev_sprite*);
void          ev_sprite_update(ev_sprite*, float);
void          ev_sprite_render(ev_sprite*);
void          ev_sprite_set_body(ev_sprite *, ev_body *);
]]
ffi.metatype("ev_sprite", { __gc = function(self) C.ev_sprite_destroy(self) end})

local Sprite = {}
Sprite.__index = Sprite

local setters = {
	 animation = function(self, val)
			C.ev_sprite_set_animation(self._ev_sprite, val._ev_anim)
	 end,
	 rotation = function(self, val)
			C.ev_sprite_set_rotation(self._ev_sprite, val)
	 end,
	 body = function(self, val)
		C.ev_sprite_set_body(self._ev_sprite, val._ev_body)
		rawset(self, 'body', val)
	 end,
	 scale = function(self, val)
			self._ev_sprite.scale = val
	 end,
	 opacity = function(self, val)
			self._ev_sprite.opacity = val
	 end
}

function Sprite:__index(key)
	 if key == 'position' then
			return C.ev_sprite_get_position(self._ev_sprite)
	 elseif key == 'opacity' then
			return self._ev_sprite.opacity
	 else
			return getmetatable(self)[key] or rawget(self, key)
	 end
end

function Sprite:set_position(x,y)
	 C.ev_sprite_set_position(self._ev_sprite, x, y)
end

function Sprite:__newindex(key, val)
	 if setters[key] then
			setters[key](self,val)
	 else
			rawset(self, key, val)
	 end
end

function Sprite.create()
	 local ev_sprite = C.ev_sprite_create()
	 local sprite = {}
	 setmetatable(sprite, Sprite)

	 sprite._ev_sprite = ev_sprite
	 return sprite
end

function Sprite.init(_ev, lib)
	 ev = _ev
	 C = lib
end

return Sprite
