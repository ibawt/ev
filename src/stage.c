#include "stage.h"
#include "sprite_batch.h"

typedef struct {
    void           *opaque;
    ev_stage_render_fn render_fn;
    ev_stage_update_fn update_fn;
} node;

struct _ev_stage {
    node* nodes;
    size_t node_len;
    size_t node_cnt;
    ev_matrix4 transform;

    int    lua_ref;
};

static void stage_init(ev_stage *stage)
{
    memset(stage, 0, sizeof(ev_stage));

    stage->nodes = ev_malloc(sizeof(node)*16);
    stage->node_len = 16;
    stage->node_cnt = 0;
}

ev_stage* ev_stage_create(void)
{
    ev_stage* stage = ev_malloc(sizeof(ev_stage));

    stage_init(stage);

    return stage;
}

void ev_stage_set_transform(ev_stage *s, const ev_matrix4 *m)
{
    if( s ) {
        memcpy(&s->transform, m, sizeof(ev_matrix4));
    }
}

void ev_stage_free(ev_stage *s)
{
    if( s ) {
        if( s->nodes ) {
            ev_free(s->nodes);
        }
        ev_free(s);
    }
}

void ev_stage_render(ev_stage *s)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for( i = s->node_cnt - 1 ; i >= 0 ; --i ) {
        s->nodes[i].render_fn( s->nodes[i].opaque, &s->transform);
    }
}

void ev_stage_add_actor(ev_stage *stage, ev_stage_render_fn render_fn, ev_stage_update_fn update_fn, void *d)
{
    node *n;

    if( stage->node_cnt == stage->node_len ) {
        stage->node_len += 16;
        stage->nodes = ev_realloc( stage->nodes, (stage->node_len)*sizeof(node));
    }

    n = stage->nodes + stage->node_cnt;

    n->opaque = d;
    n->render_fn = render_fn;
    n->update_fn = update_fn;

    stage->node_cnt++;
}

void ev_stage_update(ev_stage *stage, float dt)
{
    unsigned int i;

    for( i = 0 ; i < stage->node_cnt ; ++i ) {
        stage->nodes[i].update_fn(stage->nodes[i].opaque, dt);
    }
}
