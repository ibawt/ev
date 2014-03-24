#ifndef DEBUG_DRAW_H_
#define DEBUG_DRAW_H_

#include "evil.h"
#include "Box2D/Box2D.h"
#include "matrix4.h"
#include "shader.h"
#include "vertex_buffer.h"

class b2DebugDraw : public b2Draw
{
 public:
    b2DebugDraw(float ratio);
    ~b2DebugDraw();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string, ...);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
    void SetTransform(ev_matrix4 *);
private:
    //TODO: these member names are inconsistent
    void SetColor(const b2Color& c, float ratio = 1.0f);
    ev_matrix4 mMatrix;
    ev_program *mShader;
    GLint mColorLocation;
    float mRatio;
    ev_vbuff  *vbuff;
    ev_vbuff  *segment_vbuff;
};


#endif
