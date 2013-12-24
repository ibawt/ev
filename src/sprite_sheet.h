#ifndef EV_SPRITE_SHEET_H_
#define EV_SPRITE_SHEET_H_

#include "evil.h"

typedef struct _ev_sframe ev_sframe;
ev_sframe*  ev_sframe_create(const char *);
void        ev_sframe_destroy(ev_sframe *);
ev_bvertex* ev_sframe_get_bvertex(ev_sframe*);

typedef struct _ev_ssheet ev_ssheet;
ev_ssheet* ev_ssheet_create_file(const char *);
void       ev_ssheet_destroy(ev_ssheet *);
ev_sframe* ev_ssheet_get_sframe(ev_ssheet *, const char *);

#endif
