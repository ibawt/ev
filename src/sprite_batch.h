#ifndef EV_SPRITE_BATCH_H_
#define EV_SPRITE_BATCH_H_

#include "evil.h"

ev_sbatch* ev_sbatch_create(void);
void       ev_sbatch_destroy(ev_sbatch*);
void       ev_sbatch_render(ev_sbatch*);
void       ev_sbatch_set_texture(ev_sbatch*, ev_texture*);
void       ev_sbatch_add_sprite(ev_sbatch*, ev_sprite*);
void       ev_sbatch_set_program(ev_sbatch*, ev_program* );
void       ev_sbatch_set_matrix4(ev_sbatch*, ev_matrix4 *);
void       ev_sbatch_set_vbuff(ev_sbatch*, ev_vbuff*);

#endif
