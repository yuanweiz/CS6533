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
    if (!L || luaL_dofile(L,fname) !=LUA_OK)
        throw std::runtime_error("can't open config file");
    luaL_openlibs(L);
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
        lua_close(L);
    }
}

template <typename T>
std::vector<T> LuaConfig::getArray(const char * name){
    std::vector<T> ret;
    lua_checkstack(L,3); //one for table, two for k/v pair
    lua_getglobal(L,name);
    int t=lua_gettop(L);
    if (!lua_istable(L,t)){
        throw std::runtime_error("this table doesn't exist");
    }
    lua_pushnil(L);
    while (lua_next (L, t)!=0){
        ret.push_back( static_cast<T>(lua_tonumber(L,-1)));
        lua_pop(L,1); //pop up the value and keep the key
    }
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

//type traits
template <typename T>struct LuaTypeTrait;
template <> struct LuaTypeTrait<int>{ const static int LUATYPE=LUA_TNUMBER ;};
template <> struct LuaTypeTrait<double>{ const static int LUATYPE=LUA_TNUMBER ;};
template <> struct LuaTypeTrait<float>{ const static int LUATYPE=LUA_TNUMBER ;};
template <> struct LuaTypeTrait<bool>{ const static int LUATYPE=LUA_TBOOLEAN ;};

//wrapper for lua_get* functions, no check of stack size or type
template <typename T>T luaToType (lua_State * const L, int index );
template <> int luaToType<int> (lua_State* const L, int index){
    return static_cast<int>( .5+lua_tonumber (L,index) );
}
template <> double luaToType<double> (lua_State* const L, int index){
    return  lua_tonumber (L,index) ;
}
template <> bool luaToType<bool> (lua_State* const L, int index){
    int val =  lua_toboolean(L,index);
    bool ret = static_cast<bool>(val);
    return ret;
}


//this function checks stack size and type
template <typename T> 
T luaGetValue (lua_State * const L, const char * name){
    const int lua_type = LuaTypeTrait<T>::LUATYPE;
    lua_checkstack(L,1); 
    if (lua_getglobal(L,name)!= lua_type){
        lua_pop(L,1);
        throw std::runtime_error("such value doesn't exist or is of wrong type");
    }
    else {
        T val =  luaToType<T>(L,-1);
        lua_pop(L,1);
        return val;
    }
}

int LuaConfig::getInt(const char* name){
    return luaGetValue<int>(L,name);
}
double LuaConfig::getDouble(const char* name){
    return luaGetValue<double>(L,name);
}
bool LuaConfig::getBool(const char *name){
    return luaGetValue<bool>(L,name);
}
