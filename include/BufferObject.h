#ifndef __BUFFER_OBJECT_H
#define __BUFFER_OBJECT_H
#include <GL/glew.h>
#include "Noncopyable.h"
#include "Error.h"
#include <cstddef>
#include "Program.h"

#include "geometrymaker.h"
// Light wrapper around a GL buffer object handle that automatically allocates
// and deallocates. Can be casted to a GLuint.

class Attribute;
namespace detail{
template <int BUFFER,typename T>
class GlBufferObject : Noncopyable {
protected:
  GLuint handle_;
  size_t size_;

public:
  typedef T data_type;
  GlBufferObject (T*data, size_t sz){
      size_ = sz;
      glGenBuffers(1, &handle_);
      glBindBuffer(BUFFER,handle_);
      glBufferData(BUFFER,sz*sizeof(T),data,GL_STATIC_DRAW);
      checkGlErrors(__FILE__, __LINE__);
  }
  void bind (){
      glBindBuffer(BUFFER,handle_);
  }

  ~GlBufferObject() {
    glDeleteBuffers(1, &handle_);
  }

  size_t size(){return size_;}
  GLuint get(){return handle_;}
};
}

class IndexBuffer :
    public detail::GlBufferObject
    <GL_ELEMENT_ARRAY_BUFFER, unsigned short> {
        using Base = detail::GlBufferObject<
            GL_ELEMENT_ARRAY_BUFFER, unsigned short>;
        public:
        IndexBuffer(unsigned short *data,size_t sz)
            :Base(data,sz){}
    };
class VertexBuffer : public detail::GlBufferObject<GL_ARRAY_BUFFER, VertexPNTBTG> {
    public:
        using Base=detail::GlBufferObject<GL_ARRAY_BUFFER,
              VertexPNTBTG>;
        using Base::data_type;
        VertexBuffer( data_type *data,size_t sz)
            :Base(data,sz)
        {
        }
    void addAttribute(const Attribute& attrib){
        attributes_.push_back(attrib); //copy semantic
    }
    void setAttributePointers();
    private:
    std::vector<Attribute> attributes_;
};

#endif //__BUFFER_OBJECT_H
