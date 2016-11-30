#include "Program.h"
#include "glsupport.h"
#include <stdio.h>
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
