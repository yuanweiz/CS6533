#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "GL/glew.h"
#include "Noncopyable.h"
#include "Error.h"
#include "glsupport.h"
#include <memory>
// Light wrapper around a GL texture object handle that automatically allocates
// and deallocates. Can be casted to a GLuint.


class Texture : Noncopyable {
    public:
        Texture(const char *filePath) 
            :handle_(loadGLTexture(filePath))
        {
        }
        Texture (Texture && rhs)noexcept{
            if (&rhs == this)return;
            handle_ = rhs.handle_;
            rhs.handle_ = 0;
        }

        static Texture createEmptyTexture(int w,int h, GLenum color){
            GLuint ret; 
            glUseProgram(0);
            glGenTextures(1, &ret);
            glBindTexture(GL_TEXTURE_2D, ret);
            glTexImage2D(GL_TEXTURE_2D, 0,color, w, h, 0,color,
                    GL_UNSIGNED_BYTE, NULL);
            return {ret};
        }

        ~Texture() {
            glDeleteTextures(1, &handle_);
        }

        GLuint get  () const {
            return handle_;
        }

    protected:

        Texture(GLuint handle):handle_(handle){
        }
        Texture (int ,int);
        GLuint handle_;
};

#endif //_TEXTURE_H

