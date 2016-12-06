#ifndef __UNIFORM_H
#define __UNIFORM_H

#include <GL/glew.h>
#include "Noncopyable.h"
#include "Program.h"
#include <assert.h>
#include <cstdio>
#include "Wrapper.h"
#include <string>
#include <stdexcept>
template <typename T>
class Uniform {
    public:
    Uniform(Program * program, const char* name)
        :handle_(program->getUniform(name)), program_(program),
        name_(name)
    {
        if (handle_ == static_cast<GLuint>(-1)){
            char buf[128];
            snprintf(buf,sizeof(buf),"Can't find uniform location of %s",name);
            throw std::runtime_error(buf);
        }
    }
    Uniform(const Uniform &)=default;
    GLuint get(){return handle_;}
    template <typename ... Args>
        void setValue(Args...args){
            static_cast<T*>(this)->func_(handle_,args...);
            assert(program_->get() == ogl::getCurrentProgram());
            auto err = glGetError(); (void) err;
    }
    protected:
    const GLuint handle_;
    Program * program_;
    std::string name_;
};

#define UNIFORM_DECLARE_TYPE(name,func)\
class name: public Uniform<name>{\
    public:\
    name(Program *program, const char* varname):\
    Uniform<name>(program,varname)\
        ,func_(func){}\
    const decltype(func) func_;\
}

UNIFORM_DECLARE_TYPE(Uniform1f,glUniform1f);
UNIFORM_DECLARE_TYPE(Uniform2f,glUniform2f);
UNIFORM_DECLARE_TYPE(Uniform3f,glUniform3f);
UNIFORM_DECLARE_TYPE(Uniform4f,glUniform4f);
UNIFORM_DECLARE_TYPE(Uniform1d,glUniform1d);
UNIFORM_DECLARE_TYPE(Uniform2d,glUniform2d);
UNIFORM_DECLARE_TYPE(Uniform3d,glUniform3d);
UNIFORM_DECLARE_TYPE(Uniform4d,glUniform4d);
UNIFORM_DECLARE_TYPE(Uniform1i,glUniform1i);
UNIFORM_DECLARE_TYPE(UniformMatrix4fv,glUniformMatrix4fv);

#endif// __UNIFORM_H
