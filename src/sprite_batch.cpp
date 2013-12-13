#include "evil.h"
#include "sprite_batch.h"
#include "sprite.h"
#include "jansson.h"
#include "utils.h"

#include <stdio.h>

namespace evil {

static bool parseRect(json_t *obj, const char *key, Rect& r)
{
		if( obj ) {
				json_t *value = json_object_get(obj,key);
				if( value && json_is_string(value)) {
						const char *data = json_string_value(value);
						int x,y,w,h;

						if( sscanf(data, "{{%d, %d}, {%d, %d}}", &x, &y, &w, &h ) != 4 )
								return false;

						r.x = x;
						r.y = y;
						r.w = w;
						r.h = h;

						return true;
				}
		}
		return false;
}


static bool parseSize(json_t *obj, const char *key, Rect& r)
{
		if( obj ) {
				json_t *value = json_object_get(obj, key );
				if( value && json_is_string(value) ) {
						const char *data = json_string_value(value);
						int w,h;
						if (sscanf(data, "{%d, %d}", &w, &h) < 2 ) {
								return false;
						}
						r.w = w;
						r.h = h;

						return true;
				}
		}
		return false;
}

static bool parseVector(json_t *obj, const char *key, Vector2& v ) {
		Rect r;
		if( parseSize(obj, key, r) ) {
				v.x = r.w;
				v.y = r.h;

				return true;
		}
		return false;
}


void SpriteBatch::setTexture( shared_ptr<Texture>& t )
{
		texture = t;

		for( auto& frame : sheet.frames ) {
				frame.second->textureRect.w /= t->getWidth();
				frame.second->textureRect.h /= t->getHeight();
		}
}

bool SpriteBatch::load(const string& json)
{
		sheet.frames.clear();

		json_t *root;
		json_error_t err;

		root = json_load_file( json.c_str(), 0, &err );

		if( !root ) {
				error("File didn't load");
				return false;
		}

		auto dl = MakeScopeExit([&root]() { json_decref(root); root = nullptr; });

#if 0
		char *output = json_dumps(root, JSON_INDENT(1) );
		if( !output ) {
				json_decref(root);
				return false;
		}

		printf("%s\n", output);
		free( output );
#endif

		json_t* frames = json_object_get(root, "frames");

		if( !frames ) {
				json_decref(root);
				return false;
		}

		const char *key;
		json_t *value;

		json_object_foreach( frames, key, value ) {
				auto frame = make_shared<SpriteFrame>();

				frame->key = string(key);
				frame->trimmed = json_is_true(json_object_get(value, "spriteTrimmed"));
				frame->rotated = json_is_true(json_object_get(value, "textureRotated"));

				if( !(parseSize(value, "spriteSourceSize", frame->sourceSize ) &&
							parseSize(value, "spriteSize", frame->size ) &&
							parseRect(value, "textureRect", frame->textureRect) &&
							parseVector(value, "spriteOffset", frame->offset) &&
							parseRect(value, "spriteColorRect", frame->colorRect)) ) {
						return false;
				}
				sheet.frames[key] = frame;
		}

		return true;
}

shared_ptr<Sprite> SpriteBatch::get(const string& name)
{
		auto s = make_shared<Sprite>();

		auto& frame = sheet.frames[name];

		s->setFrame(frame);
		return s;
}

void SpriteBatch::render()
{
}

}
