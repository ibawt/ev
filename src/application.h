#ifndef EV_APPLICATION_H_
#define EV_APPLICATION_H_

#include <stdint.h>

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ev_app ev_app;

typedef void (*ev_app_render)(ev_app*);
typedef void (*ev_app_update)(ev_app*, float);
typedef void (*ev_app_key_event)(ev_app*, ev_key_event *);
typedef void (*ev_app_mouse_event)(ev_app*, ev_mouse_event*);

 __attribute__ ((visibility ("default"))) ev_app*  ev_app_create(uint32_t width, uint32_t height);
void     ev_app_destroy(ev_app*);
uint32_t ev_app_get_height(ev_app*);
uint32_t ev_app_get_width(ev_app*);
float    ev_app_get_fps(ev_app*);
ev_err_t ev_app_init(ev_app* );
void     ev_app_quit(ev_app*);
void     ev_app_set_render(ev_app*, ev_app_render);
void     ev_app_set_update(ev_app*, ev_app_update);
void     ev_app_set_key_event(ev_app*, ev_app_key_event);
void     ev_app_set_mouse_event(ev_app*, ev_app_mouse_event);
ev_err_t ev_app_start(ev_app*);
void ev_app_set_stage(ev_app *app, ev_stage *s);

ev_world* ev_app_get_world(void);

typedef enum {
    EV_APP_STATE_RESET = 0,
    EV_APP_STATE_READY = 1,
    EV_APP_INVALID = -1
} ev_app_state;

ev_app_state ev_app_get_state(ev_app*);

ev_app* ev_app_get_lua_instance(void);

#ifdef __cplusplus
}
#endif

#endif
