#ifndef __PROGRAM_H
#define __PROGRAM_H
#include <GL/glew.h>
#include "Noncopyable.h"
#include "Error.h"
// Light wrapper around GLSL program handle that automatically allocates
// and deallocates. Can be casted to a GLuint.
class GlProgram : Noncopyable {
protected:
  GLuint handle_;

public:
  GlProgram() {
    handle_ = glCreateProgram();
    if (handle_ == 0)
      throw std::runtime_error("glCreateProgram fails");
    checkGlErrors(__FILE__, __LINE__);
  }

  ~GlProgram() {
    glDeleteProgram(handle_);
  }

  // Casts to GLuint so can be used directly by glUseProgram and so on
  operator GLuint() const {
    return handle_;
  }
};
#endif// __PROGRAM_H
