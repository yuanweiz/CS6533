#ifndef __UNIFORM_H
#define __UNIFORM_H
#include <GL/glew.h>
#include <functional>
#include "Noncopyable.h"

template <typename T,int sz>
class GLUniform :Noncopyable{
    //helper template
    template <int s,typename U,typename ...tail>
        struct UniformFuncType{
            using type=typename UniformFuncType<s-1,U,U,tail...>::type;
        };
    template <typename U,typename ...tail>
        struct UniformFuncType<1,U,tail...>{
            using type= void(GLuint,U,tail...);
        };
    public:
    const static int size = sz;
    typedef T value_type;
    using func_type = typename UniformFuncType<sz,T>::type;

    template <typename ...Args>
        void setValue(Args...args){
            func_(handle_,args...);
        }
    

    GLUniform(GLuint program,const char* name):
        program_(program),
        handle_(glGetUniformLocation(program,name))
    {
        init();
    }

    GLuint get(){return handle_;}

    private:
    //Uniform(GLuint program,const char* name,func_type func):
    //    program_(program),
    //    handle_(glGetUniformLocation(program,name)),
    //    func_(func)
    //{
    //}
    void init(); //not substantialized
    GLuint program_;
    GLuint handle_;
    std::function<func_type> func_;
};

//explicitly substantialized here
template<> void GLUniform<float,1>::init(){
    func_=glUniform1f;
}
template<> void GLUniform<float,2>::init(){
    func_=glUniform2f;
}
template<> void GLUniform<float,3>::init(){
    func_=glUniform3f;
}
template<> void GLUniform<float,4>::init(){
    func_=glUniform4f;
}



#endif// __UNIFORM_H
