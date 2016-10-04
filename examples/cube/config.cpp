#include "config.h"
extern "C"{
#include <lauxlib.h>
#include <lualib.h>
}
#include <stdexcept>
#include <assert.h>
#include <cstdio>

LuaConfig::LuaConfig(const char* fname)
    :L(luaL_newstate())
{
    luaL_openlibs(L);
    if (!L || luaL_dofile(L,fname) !=LUA_OK)
        throw std::runtime_error("can't open config file");
    //if has init() function, call it
    lua_getglobal(L,"init");
    if (lua_isfunction(L,-1)){
        if (LUA_OK!=lua_pcall(L,0,0,0)){
            const char * err = lua_tostring(L,-1);
            printf("%s",err);
            throw std::runtime_error("init failed");
        }
    }
    else lua_pop(L,1);
}

LuaConfig::~LuaConfig(){
    if (L){
        int pos = lua_gettop(L); (void)pos;
        lua_settop(L,0);
        lua_close(L);
    }
}

template <typename T>
std::vector<T> LuaConfig::getArray(const char * name){
    //std::vector<T> ret={};
    int pos=lua_gettop(L);(void)pos;
    lua_getglobal(L,name);
    if (!lua_istable(L,-1)){
        throw std::runtime_error("this table doesn't exist");
    }
    int i;
    //when loop stops, i==-cnt-2
    for (i=-1;!lua_isnil(L,-1);--i){
        lua_geti(L,i,-i);
    }
    //ret.reserve(-i-2);
    std::vector<T> ret(-i-2);
    for (int j=i+1,k=0;j<-1;++j,++k){
        ret[k]=static_cast<T>(lua_tonumber(L,j));
        //ret.push_back(static_cast<T>(lua_tonumber(L,j)));
    }
    //including table and nil
    lua_pop(L,-i);
    assert (pos==lua_gettop(L));
    return ret;
}

//instantiate
std::vector<float> LuaConfig::getFloatArray(const char*name){
    return getArray<float>(name);
}

//TODO:this double->int32 convertion is buggy (should use i=f+0.5)
std::vector<int> LuaConfig::getIntArray(const char*name){
    return getArray<int>(name);
}

