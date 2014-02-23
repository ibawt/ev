#ifndef EV_SHADER_H_
#define EV_SHADER_H_

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

ev_shader* ev_shader_create(void);
ev_err_t   ev_shader_compile(ev_shader *, GLenum type, const char *);
void       ev_shader_destroy(ev_shader*);

ev_program* ev_program_create(void);
void        ev_program_use(ev_program*);
ev_err_t    ev_program_compile(ev_program*);
void        ev_program_set_shader(ev_program*, ev_shader*, GLenum type);
GLint       ev_program_get_attrib_loc(ev_program*, const char *);
GLint       ev_program_get_uniform_loc(ev_program*, const char *);
void        ev_program_destroy(ev_program*);

#ifdef __cplusplus
}
#endif

#endif
