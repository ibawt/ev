#include <assert.h>

#include "tile_map.h"
#include "jansson.h"
#include "utarray.h"

static ev_err_t get_json_int(json_t *dict, const char *key, uint32_t *i)
{
    json_t *value;

    assert( dict != NULL);
    assert( key != NULL );
    assert( i != NULL );

    value = json_object_get(dict, key);

    if( !value ) {
        ev_error("can't find value: %s", key);
        return EV_FAIL;
    }

    if( json_is_integer(value) ) {
        *i = json_integer_value( value);
        return EV_OK;
    }
    return EV_FAIL;
}

static ev_err_t get_json_string(json_t *dict, const char *key, char **s)
{
    json_t *value;

    assert( dict != NULL );
    assert( key != NULL );
    assert( s != NULL );

    value = json_object_get( dict, key );
    if( !value ) {
        ev_error("can't find value for key: %s", key );
        return EV_FAIL;
    }

    if( !json_is_string( value ) ) {
        ev_error("key isn't a string");
        return EV_FAIL;
    }

    *s = ev_strdup( json_string_value(value));

    return EV_OK;
}

typedef struct {
    uint32_t id;
    uint32_t terrain[4];
    float    probability;
} ev_tile;

typedef struct {
    char *name;
    uint32_t tile;
} ev_terrain;

typedef struct {
    uint32_t firstgid;
    uint32_t image_width;
    uint32_t image_height;
    uint32_t tile_width;
    uint32_t tile_height;
    uint32_t spacing;
    uint32_t margin;

    ev_terrain *terrains;
    uint32_t    terrain_len;

    ev_tile    *tiles;
    uint32_t    tile_len;

    char *image;
    char *name;
} ev_tileset;

typedef struct {
    uint16_t *data;
    uint32_t  data_len;
    uint32_t  height;
    uint32_t  width;
    char     *name;
    float     opacity;
    char     *type;
    int       visible;
    float     x;
    float     y;
} ev_layer;

struct _ev_tilemap {
    uint32_t      cols;
    uint32_t      rows;
    ev_map_type_t orientation;
    uint32_t      tile_width;
    uint32_t      tile_height;
    uint32_t      color;
    ev_layer     *layers;
    uint32_t      layer_len;
    ev_tileset   *tilesets;
    uint32_t      tileset_len;
};

static ev_err_t get_orientation(json_t *object, ev_map_type_t *t)
{
    json_t     *value;
    const char *s;

    assert( object != NULL );
    assert( t != NULL );

    value = json_object_get(object, "orientation");
    if( !value )
        return EV_FAIL;

    if( !json_is_string(value))
        return EV_FAIL;

    s = json_string_value(value);

    if( strcmp( s, "orthographic") == 0 ) {
        *t = MAP_ORTHO;
        return EV_OK;
    } else if( strcmp( s, "isometric") == 0 ) {
        *t = MAP_ISOMETRIC;
        return EV_OK;
    } else if( strcmp( s, "staggered" ) == 0 ) {
        *t = MAP_STAGGERED;
        return EV_OK;
    }
    return EV_FAIL;
}

static void tileset_destroy( ev_tileset *t)
{
    uint32_t i;

    assert( t != NULL );

    if( t->image ) {
        ev_free(t->image);
    }
    if( t->name ) {
        ev_free(t->name);
    }
    if( t->terrains ) {
        for( i = 0 ; i < t->terrain_len ; ++i ) {
            if( t->terrains[i].name ) {
                ev_free(t->terrains[i].name);
            }
        }
        ev_free(t->terrains);
    }
    if( t->tiles ) {
        ev_free( t->tiles);
    }
}

ev_tilemap *ev_tilemap_create(void)
{
    ev_tilemap* t = ev_malloc(sizeof(ev_tilemap));

    if( t ) {
        memset( t, 0, sizeof(ev_tilemap));
    }
    return t;

}

void ev_tilemap_destroy(ev_tilemap *e)
{
    uint32_t i;

    if( e ) {
        for( i = 0 ; i < e->tileset_len ; ++i ) {
            tileset_destroy( e->tilesets + i );
        }
        ev_free( e->tilesets );
        ev_free( e );
    }
}

static ev_err_t parse_tileset( ev_tileset *t, json_t *object)
{
    json_t *value;
    unsigned i;
    unsigned len;

    assert( t != NULL );
    assert( object != NULL );

    if( get_json_int(object, "firstgid", &t->firstgid) ||
        get_json_int(object, "imagewidth", &t->image_width) ||
        get_json_int(object, "imageheight", &t->image_height) ||
        get_json_int(object, "margin", &t->margin ) ||
        get_json_int(object, "tilewidth", &t->tile_width) ||
        get_json_int(object, "tileheight", &t->tile_height )) {
        return EV_FAIL;
    }

    if( get_json_string( object, "name", &t->name ) ) {
        return EV_FAIL;
    }

    if(  get_json_int( object, "spacing", &t->spacing) ) {
        return EV_FAIL;
    }

    value = json_object_get(object, "terrains");
    if( !value || !json_is_array(value)) {
        return EV_FAIL;
    }

    len = json_array_size(value);
    t->terrains = ev_malloc(sizeof(ev_terrain)* len );
    t->terrain_len = len;
    if( t->terrains ) {
        for( i = 0 ; i < len ; ++i ) {
            get_json_string(json_array_get(value, i), "name", &t->terrains[i].name);
            get_json_int( json_array_get(value, i), "tile", &t->terrains[i].tile);
        }
    }

    value = json_object_get(object, "tiles");
    len = json_object_size(value);
    t->tiles = ev_malloc(sizeof(ev_tile)* len );
    t->tile_len = len;
    if( t->tiles ) {
        char buff[32];
        json_t *tt;
        unsigned j;

        for( i = 0 ; i < len ; ++i ) {
            snprintf(buff, sizeof(buff), "%d", i);

            tt = json_object_get( json_object_get(value, buff), "terrain");

            for( j = 0 ; j < 4 ; ++j ) {
                t->tiles[i].terrain[j] = json_integer_value( json_array_get( tt, j ) );
            }
        }
    }
    return EV_OK;
}

static ev_err_t parse_layer( ev_layer *l, json_t *object)
{
    json_t  *value;
    unsigned i;
    unsigned len;

    assert( l != NULL );
    assert( object != NULL );

    value = json_object_get(object, "data");
    if( !value || !json_is_array(value))
        return EV_FAIL;

    len = json_array_size( value );

    for( i = 0 ; i < len ; ++i ) {
        int tile_index;
        tile_index = json_integer_value( json_array_get( value, i));
        l->data[i] = tile_index;
    }
    l->data_len = len;

    l->height = json_integer_value(json_object_get(value, "height"));
    l->width  = json_integer_value(json_object_get(value, "width"));
    l->opacity = (float)json_real_value(json_object_get(value, "opacity"));
    l->type = ev_strdup( json_string_value( json_object_get(value, "type")));

    l->visible = (int)json_is_true(json_object_get(value, "visible"));
    l->x = (float)json_real_value(json_object_get(value, "x"));
    l->y = (float)json_real_value(json_object_get(value, "y"));
    l->name = ev_strdup(json_string_value(json_object_get(value, "name")));
    return EV_OK;
}

ev_err_t ev_tilemap_load(ev_tilemap *t, const char *file)
{
    json_t *root;
    json_error_t json_err;
    json_t *value;
    unsigned i;
    unsigned len;

    if( !t || !file )
        return EV_FAIL;

    root = json_load_file( file, 0, &json_err);

    if( get_json_int( root, "height", &t->rows ) ||
        get_json_int( root, "width",  &t->cols ) ) {
        json_decref(root);
        return EV_FAIL;
    }

    if( get_orientation(root, &t->orientation ) ) {
        json_decref(root);
        return EV_FAIL;
    }

    value = json_object_get( root, "layers");
    if( value && json_is_array(value) ) {
        len = json_array_size( value );

        t->layers = ev_malloc( sizeof( ev_layer ) * len );
        t->layer_len = len;
        if( t->layers ) {
            memset( t->layers, 0, sizeof(ev_layer ) * len );

            for( i = 0 ; i < len ; ++i ) {
                parse_layer(t->layers + i, json_array_get( value, i ) );
            }
        }
    } else {
        ev_error("no layers? not fatal");
    }

    value = json_object_get(root, "tilesets");
    if( value && json_is_array(value)) {
        len = json_array_size(value);
        t->tilesets = ev_malloc(sizeof(ev_tileset)*len);
        t->tileset_len = len;
        if( t->tilesets ) {
            memset( t->tilesets, 0, sizeof(ev_tileset)*len);
            for( i = 0 ; i < len ; ++i ) {
                parse_tileset( t->tilesets + i, json_array_get(value, i));
            }
        }
    }

    if( get_json_int( root, "tilewidth", &t->tile_width ) ||
        get_json_int( root, "tileheight", &t->tile_height) ) {
        json_decref( root );
        return EV_FAIL;
    }

    if( !root ) {
        return EV_FAIL;
    }

    if( root ) {
        json_decref(root);
    }
    return EV_OK;
}
