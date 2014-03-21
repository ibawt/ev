#ifndef EV_TEXTURE_H_
#define EV_TEXTURE_H_

#include "ev_lua.h"
#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

EV_API ev_texture* ev_texture_create(void);
EV_API void        ev_texture_destroy(ev_texture*);
EV_API void        ev_texture_bind(ev_texture*);
EV_API ev_err_t    ev_texture_load(ev_texture*, const char*);
EV_API uint32_t    ev_texture_get_width(ev_texture*);
EV_API uint32_t    ev_texture_get_height(ev_texture*);

#ifdef __cplusplus
}
#endif

#endif
