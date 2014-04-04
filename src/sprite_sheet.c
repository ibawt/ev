#include <assert.h>

#include "evil.h"
#include "sprite_sheet.h"
#include "utils.h"
#include "sprite.h"
#include "jansson.h"

#if defined(_MSC_VER)
#define sscanf sscanf_s
#endif

struct _ev_sframe
{
    const char *key;
    ev_size     source_size;
    ev_size     size;
    int         trimmed;
    ev_rect     texture_rect;
    ev_vec2     offset;
    ev_bool     rotated;
    ev_rect     color_rect;
    ev_bvertex  batch_verts[EV_SPRITE_NUM_VERTS];
};

struct _ev_ssheet
{
    ev_smap *frames;
    ev_smap *metadata;
};

static void fill_batch_verts(ev_sframe* frame)
{
    int i;
    ev_bvertex* bv = frame->batch_verts;

    bv[0].x = -frame->size.w/2;
    bv[0].y = -frame->size.h/2;
    bv[0].u = frame->texture_rect.origin.x;
    bv[0].v = frame->texture_rect.origin.y;

    bv[1].x = frame->size.w/2;
    bv[1].y = -frame->size.h/2;
    bv[1].u = frame->texture_rect.origin.x + frame->texture_rect.size.w;
    bv[1].v = frame->texture_rect.origin.y;

    bv[2].x = frame->size.w/2;
    bv[2].y = frame->size.h/2;
    bv[2].u = frame->texture_rect.origin.x + frame->texture_rect.size.w;
    bv[2].v = frame->texture_rect.origin.y + frame->texture_rect.size.h;

    bv[3] = bv[2];

    bv[4].x = -frame->size.w/2;
    bv[4].y = frame->size.h/2;
    bv[4].u = frame->texture_rect.origin.x;
    bv[4].v = frame->texture_rect.origin.y + frame->texture_rect.size.h;

    bv[5] = bv[0];

    for( i = 0 ; i < 6 ; ++i ) {
        bv[i].scale = 1.0f;
        bv[i].rotation = 0.0f;
        bv[i].tx = 0.0f;
        bv[i].ty = 0.0f;
    }
}

ev_sframe* ev_sframe_create_quad(float w, float h, float left, float top,float right,float bottom)
{
    ev_sframe* sframe = ev_malloc(sizeof(ev_sframe));
    if( sframe ) {
        sframe->size.w = w;
        sframe->size.h = h;
        sframe->texture_rect.origin.x = left;
        sframe->texture_rect.origin.y = top;
        sframe->texture_rect.size.w = right;
        sframe->texture_rect.size.h = bottom;
        fill_batch_verts(sframe);
    }
    return sframe;
}

static ev_err_t parse_size(json_t *o, const char *key, ev_size *size)
{
    int w,h;
    json_t *value;

    if( !o )
        return EV_FAIL;

    assert( key != NULL );
    assert( size != NULL );

    value = json_object_get(o, key );
    if( !value || !json_is_string(value) )
        return EV_FAIL;

    if( sscanf( json_string_value(value), "{%d, %d}", &w, &h ) < 2 ) {
        return EV_FAIL;
    }

    size->w = (float)w;
    size->h = (float)h;

    return EV_OK;
}

static ev_err_t parse_rect(json_t *o, const char *key, ev_rect* rect)
{
    int x,y,w,h;
    json_t *value;

    if( !o )
        return EV_FAIL;

    assert( key != NULL );
    assert( rect != NULL );

    value = json_object_get(o, key);

    if( !value || !json_is_string(value))
        return EV_FAIL;

    if( sscanf(json_string_value(value), "{{%d, %d}, {%d, %d}}",
               &x, &y, &w, &h) < 4 ) {
        return EV_FAIL;
    }

    rect->origin.x = (float)x;
    rect->origin.y = (float)y;
    rect->size.w = (float)w;
    rect->size.h = (float)h;

    return EV_OK;
}

static ev_err_t parse_vec2(json_t *o, const char *key, ev_vec2* vec)
{
    int x,y;
    json_t *value;

    assert( key != NULL);
    assert( vec != NULL );

    value = json_object_get(o, key);
    if( value && json_is_string(value) ) {
        if( sscanf(json_string_value(value),"{%d, %d}", &x, &y) == 2 ) {
            vec->x = (float)x;
            vec->y = (float)y;
            return EV_OK;
        }
    }
    return EV_FAIL;
}

static void normalize_texture_rect(ev_sframe* frame, ev_size* size)
{
    assert( frame != NULL);
    assert( size != NULL );
    assert( size->w != 0 );
    assert( size->h != 0 );

    frame->texture_rect.origin.x /= size->w;
    frame->texture_rect.origin.y /= size->h;
    frame->texture_rect.size.w /= size->w;
    frame->texture_rect.size.h /= size->h;
}

static ev_sframe* parse_sframe(json_t *frame)
{
    ev_sframe* sframe;

    assert( frame != NULL );

    sframe = ev_malloc(sizeof(ev_sframe));
    if( !sframe )
        return NULL;

    memset(sframe, 0, sizeof(ev_sframe));

    sframe->trimmed = json_is_true( json_object_get(frame, "spriteTrimmed"));
    sframe->rotated = json_is_true( json_object_get(frame, "textureRotated"));

    if( parse_size( frame, "spriteSourceSize", &sframe->source_size) ||
        parse_size( frame, "spriteSize", &sframe->size ) ||
        parse_rect( frame, "textureRect", &sframe->texture_rect) ||
        parse_vec2( frame, "spriteOffset",&sframe->offset) ||
        parse_rect( frame, "spriteColorRect", &sframe->color_rect) ) {
        ev_free(sframe);
        return NULL;
    }
    return sframe;
}

static ev_smap* parse_metadata(json_t *object)
{
    ev_smap *map;
    ev_size *size;

    if( !object )
        return NULL;

    map = ev_smap_create();
    if( !map )
        return NULL;

    ev_smap_set_delete( map, ev_free);

    size = ev_malloc(sizeof(ev_size));
    if( !size ) {
        ev_smap_destroy(map);
        return NULL;
    }

    if( parse_size( object,"size", size ) )  {
        ev_smap_destroy(map);
        return NULL;
    }

    ev_smap_put(map, "size", size );

    return map;
}

static ev_smap* parse_frames(json_t *json,  ev_size *tex_size)
{
    const char *k;
    json_t     *v;
    ev_smap    *frames;

    if( !json ) {
        return NULL;
    }

    frames = ev_smap_create();
    if( !frames )
        return NULL;

    ev_smap_set_delete(frames, ev_free);

    json_object_foreach( json, k, v) {
        assert( k != NULL);
        assert( v != NULL);

        ev_sframe* frame = parse_sframe(v);

        if( !frame )  {
            goto CLEAN_UP;
        }

        if( tex_size ) {
            normalize_texture_rect(frame, tex_size);
        }
        fill_batch_verts(frame);
        ev_smap_put( frames, k, frame);
    }

    return frames;
 CLEAN_UP:
    if( frames ) {
        ev_smap_destroy(frames);
    }
    return NULL;
}

ev_ssheet* ev_ssheet_create(void)
{
    ev_ssheet *s = ev_malloc(sizeof(ev_ssheet));
    memset( s, 0, sizeof(ev_ssheet));
    return s;
}

void ev_ssheet_destroy(ev_ssheet *s)
{
    if( s ) {
        if( s->frames ) {
            ev_smap_destroy(s->frames);
            s->frames = NULL;
        }
        if( s->metadata ) {
            ev_smap_destroy(s->metadata);
            s->metadata = NULL;
        }
        ev_free(s);
    }
}

ev_err_t ev_ssheet_load_file(ev_ssheet *sheet, const char *path)
{
    json_t      *root = NULL;
    json_error_t json_err;

    if( !path || !sheet)
        return EV_FAIL;

    root = json_load_file(path, 0, &json_err);
    if(!root) {
        ev_log("failed loading file");
        return EV_FAIL;
    }

    sheet->metadata = parse_metadata(json_object_get(root, "metadata"));
    if( !sheet->metadata ) {
        ev_log("metadata parsing failed");
        json_decref(root);
        return EV_FAIL;
    }

    sheet->frames = parse_frames(json_object_get(root, "frames"), ev_smap_get(sheet->metadata, "size") );
    if( !sheet->frames ) {
        ev_log("frame parsging failed");
        json_decref(root);
        ev_smap_destroy(sheet->metadata);
        sheet->metadata = NULL;
        return EV_FAIL;
    }

    if( root ) {
        json_decref(root);
    }
    return EV_OK;
}

ev_bvertex* ev_sframe_get_bvertex(ev_sframe* sframe)
{
    return sframe ? &sframe->batch_verts[0] : NULL;
}

ev_sframe* ev_ssheet_get_sframe(ev_ssheet* sheet, const char *name)
{
    if( sheet && name ) {
        return ev_smap_get(sheet->frames, name);
    }
    return NULL;
}

ev_size ev_sframe_get_size(ev_sframe *s)
{
    assert( s != NULL );

    return s->size;
}
