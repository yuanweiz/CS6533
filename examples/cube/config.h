#ifndef __CONFIG_H
#define __CONFIG_H
#include <vector>
#include <string>
#include <map>
#include <memory>
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
    using AnyPtr = std::shared_ptr<void> ;
    std::vector<AnyPtr> getAnyArray (const char*);

    //get value, number-to-bool implicit convertion not permitted
    bool getBool(const char*);
    int getInt  (const char*);
    double getDouble(const char*);
    std::string getString(const char*);

    //get dictionary
    template <typename K,typename V>
        std::map<K,V> getDict (const char*);

    lua_State * get(){return L;}

    private:
    template <typename T>
    std::vector<T> getArray(const char * name);
    lua_State * L;
};


#endif// __CONFIG_H
