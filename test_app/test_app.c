#include "evil.h"
#include "application.h"
#include "texture.h"
#include "sprite_batch.h"
#include "matrix4.h"
#include "sprite.h"
#include "animation.h"

#define NUM_SPRITES 1

ev_app     *app = NULL;
ev_texture *texture = NULL;
ev_sbatch  *sprite_batch = NULL;
ev_matrix4  transform = {{0}};

int init(void)
{
    int i;

    texture = ev_texture_create();
    if( ev_texture_load(texture, "../test_app/bats.png") ) {
        ev_error("texture failed to load");
        return 1;
    }

    sprite_batch = ev_sbatch_create();

    if( ev_sbatch_load(sprite_batch, "../test_app/bats.json") ) {
        ev_error("sprite batch failed to load");
        return 1;
    }
    ev_sbatch_set_texture( sprite_batch, texture );

    for( i = 0 ; i < NUM_SPRITES ; ++i ) {
        ev_sprite *s = ev_sprite_create();
        ev_anim   *a = ev_anim_create();
        ev_anim_add_sframe(a, ev_sbatch_get_sframe(sprite_batch, "bats_fly1.png"));
        ev_anim_add_sframe(a, ev_sbatch_get_sframe(sprite_batch, "bats_fly2.png"));
        ev_anim_add_sframe(a, ev_sbatch_get_sframe(sprite_batch, "bats_fly3.png"));
        ev_sprite_set_animation(s, a);
        ev_sprite_set_rotation(s, 0.0f);
        ev_sprite_set_position(s, 400, 300);
        ev_sbatch_add_sprite(sprite_batch, s);
    }
    ev_sbatch_set_vbuff_capacity(sprite_batch, NUM_SPRITES);

    ev_matrix4_set_ortho( &transform, 0, ev_app_get_width(app), ev_app_get_height(app), 1, -1, 1);
    ev_sbatch_set_matrix4(sprite_batch, &transform);

    return EV_OK;
}

void render(ev_app* app)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ev_sbatch_render(sprite_batch);
}

void update(ev_app* app, float dt)
{
    ev_sbatch_update(sprite_batch, dt);
}

int main(int argc, char** argv)
{
    ev_err_t err;

    app = ev_app_create(800,600);
    if(!app) {
        return 1;
    }
    ev_app_set_render(app, render);
    ev_app_set_update(app, update);

    err = ev_app_init(app);
    if( err ) {
        ev_app_destroy(app);
        return 1;
    }

    if( init() ) {
        ev_error("init failed aborting");
        ev_app_destroy(app);
        return 1;
    }

    err = ev_app_start(app);

    ev_sbatch_destroy(sprite_batch);
    ev_texture_destroy(texture);

    ev_app_destroy(app);

    return err;
}
