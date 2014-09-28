#ifndef EV_APPLICATION_H_
#define EV_APPLICATION_H_

#include <stdint.h>

#include "evil.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EV_NO_EVENT = 0,
    EV_QUIT,
    EV_KEYUP,
    EV_KEYDOWN
} ev_event_type;

typedef struct {
    ev_event_type type;
    uint32_t      key;
    int x;
    int y;
} ev_event;

typedef struct _ev_app ev_app;

typedef void (*ev_app_render)(ev_app*);
typedef void (*ev_app_update)(ev_app*, float);
typedef void (*ev_app_key_event)(ev_app*, ev_key_event *);
typedef void (*ev_app_mouse_event)(ev_app*, ev_mouse_event*);

EV_API ev_app*  ev_app_create(uint32_t width, uint32_t height);
EV_API void     ev_app_destroy(ev_app*);
EV_API uint32_t ev_app_get_height(ev_app*);
EV_API uint32_t ev_app_get_width(ev_app*);
EV_API float    ev_app_get_fps(ev_app*);
EV_API ev_err_t ev_app_init(ev_app* );
EV_API void     ev_app_quit(ev_app*);
EV_API void     ev_app_set_render(ev_app*, ev_app_render);
EV_API void     ev_app_set_update(ev_app*, ev_app_update);
EV_API void     ev_app_set_key_event(ev_app*, ev_app_key_event);
EV_API void     ev_app_set_mouse_event(ev_app*, ev_app_mouse_event);
EV_API ev_err_t ev_app_start(ev_app*);
EV_API int      ev_app_get_ticks(ev_app *app);
EV_API void     ev_app_swap_buffers(ev_app*);
EV_API ev_bool  ev_app_poll_event(ev_app *, ev_event *);
EV_API ev_world* ev_app_get_world(void);

typedef enum {
    EV_APP_STATE_RESET = 0,
    EV_APP_STATE_READY = 1,
    EV_APP_INVALID = -1
} ev_app_state;

EV_API ev_app_state ev_app_get_state(ev_app*);


#ifdef __cplusplus
}
#endif

#endif
