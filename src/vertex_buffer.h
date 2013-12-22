#ifndef EV_VERTEX_BUFFER_H_
#define EV_VERTEX_BUFFER_H_

#include <vector>
namespace evil {

template <typename T>
class VertexBuffer
{
public:
    VertexBuffer() : id(0) { }
    ~VertexBuffer();

    T* map();
    void unmap();
    void dispose();

    size_t capacity() { return verts.capacity(); }
    size_t size() { return verts.size(); }
    void resize(size_t new_size);
    void bind() const;
private:
    std::vector<T> verts;
    uint32_t id;
};
}
#endif
