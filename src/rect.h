#ifndef EV_RECT_H_
#define EV_RECT_H_

#include "evil.h"

#define EV_RECT_X2(_r) ((_r)->origin.x + (_r)->size.w/2)
#define EV_RECT_Y2(_r) ((_r)->origin.y + (_r)->size.h/2)

#define EV_RECT_X1(_r) ((_r)->origin.x - (_r)->size.w/2)
#define EV_RECT_Y1(_r) ((_r)->origin.y - (_r)->size.h/2)


#define ev_rect_intersects(r1, r2) (EV_RECT_X1(r1) < EV_RECT_X2(r2) && \
                                    EV_RECT_X2(r1) > EV_RECT_X1(r2) && \
                                    EV_RECT_Y1(r1) < EV_RECT_Y2(r2) && \
                                    EV_RECT_Y2(r1) > EV_RECT_Y1(r2))

ev_rect ev_rect_create(float x, float y, float w, float h);
#endif
