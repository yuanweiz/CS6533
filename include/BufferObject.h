#ifndef __BUFFER_OBJECT_H
#define __BUFFER_OBJECT_H
#include <GL/glew.h>
#include "Noncopyable.h"
#include "glsupport.h"
// Light wrapper around a GL buffer object handle that automatically allocates
// and deallocates. Can be casted to a GLuint.
class GlBufferObject : Noncopyable {
protected:
  GLuint handle_;

public:
  GlBufferObject() {
    glGenBuffers(1, &handle_);
    checkGlErrors(__FILE__, __LINE__);
  }

  ~GlBufferObject() {
    glDeleteBuffers(1, &handle_);
  }

  // Casts to GLuint so can be used directly glBindBuffer and so on
  operator GLuint() const {
    return handle_;
  }
};

#endif //__BUFFER_OBJECT_H
