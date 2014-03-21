#ifndef EV_SHADER_H_
#define EV_SHADER_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

EV_API ev_shader* ev_shader_create(void);
EV_API ev_err_t   ev_shader_compile(ev_shader *, GLenum type, const char *);
EV_API void       ev_shader_destroy(ev_shader*);

EV_API ev_program* ev_program_create(void);
EV_API void        ev_program_use(ev_program*);
EV_API ev_err_t    ev_program_compile(ev_program*);
EV_API void        ev_program_set_shader(ev_program*, ev_shader*, GLenum type);
EV_API GLint       ev_program_get_attrib_loc(ev_program*, const char *);
EV_API GLint       ev_program_get_uniform_loc(ev_program*, const char *);
EV_API void        ev_program_destroy(ev_program*);

#ifdef __cplusplus
}
#endif

#endif
