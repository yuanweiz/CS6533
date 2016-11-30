#ifndef __WRAPPER_H
#define __WRAPPER_H
#include <GL/glew.h>
namespace ogl{
    inline GLint getCurrentProgram(){
        GLint ret;
        glGetIntegerv(GL_CURRENT_PROGRAM, &ret);
        return ret;
    }

    inline GLuint getCurrentArrayBuffer(){
        GLint ret;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &ret);
        return ret;
    }
}

#endif// __WRAPPER_H
