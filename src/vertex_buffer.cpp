#include "evil.h"
#include "vertex_buffer.h"

using namespace std;

namespace evil {

template <typename T>
VertexBuffer<T>::~VertexBuffer()
{
    dispose();
}

template <typename T>
void VertexBuffer<T>::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id );
}

template <typename T>
void VertexBuffer<T>::dispose()
{
    if( id ) {
        glDeleteBuffers(1, &id);
        id = 0;
        verts.clear();
    }
}

template <typename T>
void VertexBuffer<T>::resize(size_t new_size)
{
    if( new_size > capacity() ) {
        dispose();

        glGenBuffers(1, &id);
        glBindBuffer( GL_ARRAY_BUFFER, id);

        verts.resize(new_size);
        glBufferData( GL_ARRAY_BUFFER, new_size*sizeof(T), &verts[0], GL_DYNAMIC_DRAW);
    }
    verts.resize(new_size);
}

template <typename T>
T* VertexBuffer<T>::map()
{
    if( id ) {
        bind();
        T* p = (T*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        return p;
    }
    return nullptr;
}

template <typename T>
void VertexBuffer<T>::unmap()
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
}


}
