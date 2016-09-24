#ifndef __UNIFORM_H
#define __UNIFORM_H
#include <GL/glew.h>
#include <functional>
namespace ywz{

template <int sz,typename T,typename ...tail>
    struct UniformFuncType{
        using type=typename UniformFuncType<sz-1,T,T,tail...>::type;
    };
template <typename T,typename ...tail>
    struct UniformFuncType<1,T,tail...>{
        using type= void(GLuint,T,tail...);
    };

template <typename T,int sz>
class Uniform {
    public:
    const static int size = sz;
    typedef T value_type;
    using func_type = typename UniformFuncType<sz,T>::type;

    template <typename ...Args>
        void setValue(Args...args){
            func_(handle_,args...);
        }
    

    Uniform(GLuint program,const char* name):
        program_(program),
        handle_(glGetUniformLocation(program,name))
    {
        init();
    }
    private:
    void init();
    GLuint program_;
    GLuint handle_;
    std::function<func_type> func_;
};

template<> void Uniform<float,1>::init(){
    func_=glUniform1f;
}

//template<> void Uniform<float,2>::setValue(float v1,float v2){
//    glUniform2f(handle_,v1,v2);
//}
//template<> void Uniform<float,3>::setValue(float v1,float v2,float v3){
//    glUniform3f(handle_,v1,v2,v3);
//}

}
#endif// __UNIFORM_H
