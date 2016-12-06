#include "Program.h"
#include "glsupport.h"
#include <stdio.h>
#include <BufferObject.h>
#include "Uniform.h"
Program::Program(const char* vs, const char *fs,
        VertexBuffer* vbo, IndexBuffer*ibo,
        const std::string& name)
    :vbo_(vbo),ibo_(ibo),name_(name)
{
    handle_ = glCreateProgram();
    if (handle_ == 0){
      throw std::runtime_error("glCreateProgram fails");
    }
    checkGlErrors(__FILE__, __LINE__);
    readAndCompileShader(handle_, vs,fs);

}
  void Program::setInputTexture(const char* uniformName, GLuint texUnit, GLuint tex)
  {
      //auto utex = std::make_shared<Uniform1i> (program_,uniformName);
      assert(ogl::getCurrentProgram() == get());
      Uniform1i utex (this,uniformName);
      utex.setValue(texUnit);
      glActiveTexture(GL_TEXTURE0 + texUnit);
      glBindTexture(GL_TEXTURE_2D, tex);
  }

void Program::useThis(){
    glUseProgram(handle_);
    vbo_->bind();
    vbo_->setAttributePointers();
    if (ibo_)
        ibo_->bind();
}
