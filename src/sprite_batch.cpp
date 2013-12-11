#include "evil.h"
#include "sprite_batch.h"
#include "sprite.h"
#include "jansson.h"

#include <stdio.h>

namespace evil {

static Rect parseRect(json_t *obj, const char *key)
{
    Rect r;
    if( obj ) {
        json_t *value = json_object_get(obj,key);
        if( value && json_is_string(value)) {
            const char *data = json_string_value(value);
            int x,y,w,h;

            sscanf(data, "{{%d, %d}, {%d, %d}}", &x, &y, &w, &h );

            r.x = x;
            r.y = y;
            r.w = w;
            r.h = h;
        }
    }
    return r;
}


static Rect parseSize(json_t *obj, const char *key)
{
    Rect r;
    if( obj ) {
        json_t *value = json_object_get(obj, key );
        if( value && json_is_string(value) ) {
            const char *data = json_string_value(value);
            int w,h;
            sscanf(data, "{%d, %d}", &w, &h);
            r.w = w;
            r.h = h;
        }
    }
    return r;
}

static Vector2 parseVector(json_t *obj, const char *key ) {
    Rect r = parseSize(obj, key);
    return Vector2( r.w, r.h );
}

bool SpriteBatch::load(string json)
{
    sheet.frames.clear();
    
    json_t *root;
    json_error_t err;

    root = json_load_file( json.c_str(), 0, &err );

    if( !root ) {
        error("File didn't load");
        return false;
    }
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
        SpriteFrame frame;
        frame.key = string(key);

        frame.sourceSize = parseSize(value, "spriteSourceSize");
        frame.size = parseSize(value, "spriteSize");

        frame.trimmed = json_is_true(json_object_get(value, "spriteTrimmed"));
        frame.textureRect = parseRect(value, "textureRect");
        frame.offset = parseVector(value, "spriteOffset");
        frame.rotated = json_is_true(json_object_get(value, "textureRotated"));
        frame.colorRect = parseRect(value, "spriteColorRect");
        sheet.frames[key] = frame;
    }
    
    json_decref( root );

    return true;
}

shared_ptr<Sprite> SpriteBatch::get(string name)
{
    return make_shared<Sprite>();
}

void SpriteBatch::render()
{
}

}
