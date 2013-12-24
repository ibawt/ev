#include "evil.h"
#include "application.h"
#include "texture.h"
#include "sprite_batch.h"

ev_app     *app = NULL;
ev_texture *texture = NULL;
ev_sbatch  *sprite_batch = NULL;

void render(ev_app* app)
{

}

void update(ev_app* app, float dt)
{
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

    err = ev_app_start(app);

    ev_sbatch_destroy(sprite_batch);
    ev_texture_destroy(texture);

    ev_app_destroy(app);

    return err;
}
