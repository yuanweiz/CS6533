#ifndef __ATTRIBUTE_H
#define __ATTRIBUTE_H
#include <GL/glew.h>
#include "Noncopyable.h"
#include "Wrapper.h"
#include <assert.h>
class Program;
class VertexBuffer;
//currently only support GL_FLOAT
class Attribute :Noncopyable{
    public:
        //obsolete
        Attribute (Attribute&&)=default;
        Attribute(Program * program, VertexBuffer* vbo, const char* name,
                int length, void* offsetInBytes);
                
        GLuint get (){return handle_;}
        void enable(){ glEnableVertexAttribArray(handle_); enabled_ = true;}
        void disable(){ glDisableVertexAttribArray(handle_); enabled_ = false;}
        bool enabled() const {return enabled_;}
        void vertexAttribPointer ();
    private:
        //weak refs, for debug use
        //maybe smart pointers are better?
        Program * program_;
        VertexBuffer * vbo_ ;
        GLuint handle_;
        int length_;
        void* offsetInBytes_;
        bool enabled_;
};
#endif// __ATTRIBUTE_H
