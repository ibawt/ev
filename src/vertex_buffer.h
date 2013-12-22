#ifndef EV_VERTEX_BUFFER_H_
#define EV_VERTEX_BUFFER_H_

#include <vector>
#include <cassert>

namespace evil {

template <typename T>
class VertexBuffer
{
public:
    VertexBuffer() : id(0) { }
    ~VertexBuffer() { dispose(); }

    T* map() {
        assert( id != 0 );
        bind();
        return static_cast<T*>( glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY) );
    }
    void unmap() {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void dispose() {
        if( id  ) {
            glDeleteBuffers(1, &id);
            id = 0;
        }
    }

    size_t capacity() { return verts.capacity(); }
    size_t size() { return verts.size(); }

    void resize(size_t new_size) {
        if( new_size > capacity() ) {
            dispose();

            glGenBuffers(1, &id);
            bind();

            verts.resize(new_size);
            glBufferData( GL_ARRAY_BUFFER, new_size*sizeof(T), verts.data(), GL_DYNAMIC_DRAW);
        }
        verts.resize(new_size);
    }

    void bind() const {
        assert( id != 0 );
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }
private:
    std::vector<T> verts;
    uint32_t id;
};
}
#endif
