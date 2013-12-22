#include <stdlib.h>

#include "sprite_sheet.h"
#include "jansson.h"
#include "evil.h"
#include "utils.h"

using namespace std;

namespace evil {

static void dump(json_t* t)
{
    if( t ) {
        char *json = json_dumps(t, JSON_INDENT(1));
        if( json ) {
            log("%s", json);
            free(json);
        }
    }
}

void SpriteSheet::fillBuffer(shared_ptr<SpriteFrame> frame)
{
    auto textureRect = frame->textureRect;

    auto bv = &frame->verts[0];

    bv->x = -frame->size.w/2;
    bv->y = -frame->size.h/2;
    bv->u = textureRect.x;
    bv->v = textureRect.y;

    bv[1].x = frame->size.w/2;
    bv[1].y = -frame->size.h/2;
    bv[1].u = textureRect.x + textureRect.w;
    bv[1].v = textureRect.y;

    bv[2].x = frame->size.w/2;
    bv[2].y = frame->size.h/2;
    bv[2].u = textureRect.x + textureRect.w;
    bv[2].v = textureRect.y + textureRect.h;

    bv[3] = bv[2];

    bv[4].x = -frame->size.w/2;
    bv[4].y = frame->size.h/2;
    bv[4].u = textureRect.x;
    bv[4].v = textureRect.y + textureRect.h;

    bv[5] = bv[0];

    for( int i = 0 ; i < 6 ; ++i ) {
        bv[i].scale = 1.0f;
        bv[i].rotation = 0.0f;
        bv[i].tx = 0.0f;
        bv[i].ty = 0.0f;
    }
}

bool SpriteSheet::parseRect(json_t *obj, const string& key, Rect& r)
{
    if( obj ) {
        json_t *value = json_object_get(obj,key.c_str());
        if( value && json_is_string(value)) {
            const char *data = json_string_value(value);
            int x,y,w,h;

            if( sscanf(data, "{{%d, %d}, {%d, %d}}", &x, &y, &w, &h ) != 4 )
                return false;

            r.x = (float)x;
            r.y = (float)y;
            r.w = (float)w;
            r.h = (float)h;

            return true;
        }
    }
    return false;
}

bool SpriteSheet::parseSize(json_t *obj, const string& key, Rect& r)
{
    if( obj ) {
        json_t *value = json_object_get(obj, key.c_str());
        if( value && json_is_string(value) ) {
            const char *data = json_string_value(value);
            int w,h;
            if (sscanf(data, "{%d, %d}", &w, &h) < 2 ) {
                return false;
            }
            r.w = (float)w;
            r.h = (float)h;

            return true;
        }
    }
    return false;
}

bool SpriteSheet::parseVector(json_t *obj, const string& key, Vector2& v ) {
    Rect r;
    if( parseSize(obj, key, r) ) {
        v.x = r.w;
        v.y = r.h;

        return true;
    }
    return false;
}

bool SpriteSheet::load(const string& name)
{
    frames.clear();

    json_t *root;
    json_error_t err;

    root = json_load_file( name.c_str(), 0, &err );

    if( !root ) {
        error("File didn't load");
        return false;
    }

    auto dl = MakeScopeExit([&root]() { json_decref(root); root = nullptr; });

    json_t* json_frames = json_object_get(root, "frames");

    if( !json_frames ) {
        return false;
    }

    json_t* meta = json_object_get(root, "metadata");
    if( meta ) {
        if( !parseSize(meta,"size", textureSize) )
            return false;
    }

    const char *key;
    json_t *value;


    json_object_foreach( json_frames, key, value ) {
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
        normalizeTextureRect(frame->textureRect);
        fillBuffer(frame);
        frames[key] = frame;
    }
    return true;
}

void SpriteSheet::normalizeTextureRect(Rect&r)
{
    r.x /= textureSize.w;
    r.y /= textureSize.h;
    r.w /= textureSize.w;
    r.h /= textureSize.h;
}

}
