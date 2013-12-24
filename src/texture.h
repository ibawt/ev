#ifndef EV_TEXTURE_H_
#define EV_TEXTURE_H_

#include "evil.h"

ev_texture* ev_texture_create(void);
void        ev_texture_destroy(ev_texture*);
void        ev_texture_bind(ev_texture*);
ev_err_t    ev_texture_load(ev_texture*, const char*);
uint32_t    ev_texture_get_width(ev_texture*);
uint32_t    ev_texture_get_height(ev_texture*);

#endif
