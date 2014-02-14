#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include <stdlib.h>

#include "ev_lua.h"
#include "evil.h"
#include "matrix4.h"

ev_sbatch* ev_sbatch_create(void);
void       ev_sbatch_destroy(ev_sbatch*);
void       ev_sbatch_render(ev_sbatch*);
ev_err_t   ev_sbatch_load(ev_sbatch*, const char*);
ev_sframe* ev_sbatch_get_sframe(ev_sbatch*, const char* );
void       ev_sbatch_set_texture(ev_sbatch*, ev_texture*);
ev_err_t   ev_sbatch_add_sprite(ev_sbatch*, ev_sprite*);
void       ev_sbatch_set_program(ev_sbatch*, ev_program* );
void       ev_sbatch_set_matrix4(ev_sbatch*, ev_matrix4 *);
ev_err_t   ev_sbatch_set_vbuff_capacity(ev_sbatch*, size_t);
void       ev_sbatch_update(ev_sbatch *, float);

ev_sbatch* ev_sbatch_from_lua(lua_State *l, int arg);

#endif
