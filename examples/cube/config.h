#ifndef __CONFIG_H
#define __CONFIG_H
#include <vector>
extern "C"{
#include <lua.h>
}

#include "Noncopyable.h"
class LuaConfig:Noncopyable{
    public:
    LuaConfig();
    LuaConfig(const char* filename);
    ~LuaConfig();
    
    //these functions calls the same template function
    std::vector<int> getIntArray (const char*);
    std::vector<float> getFloatArray (const char*);
    std::vector<double> getDoubleArray (const char*);

    lua_State * get(){return L;}

    private:
    template <typename T>
    std::vector<T> getArray(const char * name);
    lua_State * L;
};


#endif// __CONFIG_H
