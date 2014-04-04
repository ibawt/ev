#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include <stdlib.h>
#include "ev_lua.h"
#include "evil.h"
#include "texture.h"
#include "matrix4.h"

#ifdef __cplusplus
extern "C" {
#endif

EV_API ev_sbatch* ev_sbatch_create(void);
EV_API void       ev_sbatch_destroy(ev_sbatch*);
EV_API void       ev_sbatch_render(ev_sbatch*, ev_matrix4 *);
EV_API ev_err_t   ev_sbatch_load(ev_sbatch*, const char*);
EV_API ev_sframe* ev_sbatch_get_sframe(ev_sbatch*, const char* );
EV_API void       ev_sbatch_set_texture(ev_sbatch*, ev_texture*);
EV_API ev_err_t   ev_sbatch_add_sprite(ev_sbatch*, ev_sprite*);
EV_API void       ev_sbatch_set_program(ev_sbatch*, ev_program* );
EV_API void       ev_sbatch_set_matrix4(ev_sbatch*, ev_matrix4 *);
EV_API ev_err_t   ev_sbatch_set_vbuff_capacity(ev_sbatch*, size_t);
EV_API void       ev_sbatch_update(ev_sbatch *, float);
EV_API int        ev_sbatch_num_filled_sprites(ev_sbatch*);
EV_API void       ev_sbatch_set_blend_func(ev_sbatch *s, int dst, int src);
EV_API void       ev_sbatch_destroy_sprite(ev_sbatch *s, int );
#ifdef __cplusplus
}
#endif

#endif
