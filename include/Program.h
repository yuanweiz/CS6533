#ifndef __PROGRAM_H
#define __PROGRAM_H
#include <GL/glew.h>
#include "Noncopyable.h"
#include "Error.h"
#include <string>
#include "Wrapper.h"
#include <assert.h>
// Light wrapper around GLSL program handle that automatically allocates
// and deallocates. Can be casted to a GLuint.

class VertexBuffer;
class IndexBuffer;
class Program : Noncopyable {
protected:
  GLuint handle_;
  //GLuint vbo_,ibo_;
  VertexBuffer * vbo_;
  IndexBuffer * ibo_;
  std::string name_;

public:
  Program(const char* vs, const char *fs,
          VertexBuffer* vbo, IndexBuffer*ibo,
          const std::string& name = "shader program"); 

  void setInputTexture(const char* uniformName, GLuint texUnit, GLuint tex);

  ~Program() {
    glDeleteProgram(handle_);
  }
  void useThis();

  GLuint getAttribute(const char* name){
      return glGetAttribLocation(handle_,name);
  }

  GLuint getUniform(const char * name){
      return glGetUniformLocation(handle_,name);
  }

  GLuint get(){return handle_;}


};
#endif// __PROGRAM_H
