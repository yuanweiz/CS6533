#ifndef __BUFFER_OBJECT_H
#define __BUFFER_OBJECT_H
#include <GL/glew.h>
#include "Noncopyable.h"
#include "Error.h"
#include <cstddef>
#include "Program.h"

#include "geometrymaker.h"
#include "Attribute.h"
#include "Texture.h"
#include <memory>
// Light wrapper around a GL buffer object handle that automatically allocates
// and deallocates. Can be casted to a GLuint.


class Attribute;
namespace detail{
class BufferObject : Noncopyable {
protected:
  GLuint handle_;
  size_t size_;
  size_t elementSize_;
  GLenum BUFFER_;

public:
  template <class T>
  BufferObject (GLenum BUFFER,T*data, size_t sz){
      BUFFER_ = BUFFER;
      elementSize_ = sizeof (T);
      size_ = sz;
      glGenBuffers(1, &handle_);
      glBindBuffer(BUFFER,handle_);
      glBufferData(BUFFER,sz*sizeof(T),data,GL_STATIC_DRAW);
      checkGlErrors(__FILE__, __LINE__);
  }
  void bind (){
      glBindBuffer(BUFFER_,handle_);
  }

  ~BufferObject() {
    glDeleteBuffers(1, &handle_);
  }

  size_t elementSize(){return elementSize_;}
  size_t size(){return size_;}
  GLuint get(){return handle_;}
};
}

class IndexBuffer
{
        public:
        IndexBuffer(unsigned short *data,size_t sz)
            :bufferObject_(GL_ELEMENT_ARRAY_BUFFER,data,sz){}
        void bind (){
            bufferObject_.bind();
        }
        size_t elementSize(){return bufferObject_.elementSize();}
        size_t size(){return bufferObject_.size();}
        private:
        detail::BufferObject bufferObject_;
};

class VertexBuffer  {
    public:
        //using Base::data_type;
        template <typename T>
        VertexBuffer( T *data,size_t sz)
            :bufferObject_(GL_ARRAY_BUFFER,data,sz)
        {
        }
        void bind (){
            bufferObject_.bind();
        }
        size_t elementSize(){return bufferObject_.elementSize();}
        size_t size(){return bufferObject_.size();}
        size_t get() {return bufferObject_.get();}

        void addAttribute(Program* program, const char* name,int length, void* offsetInBytes){
            attributes_.push_back(
             Attribute(program,this,name,length,offsetInBytes)
             );
        }
        void setAttributePointers();
    private:
        detail::BufferObject bufferObject_;
        std::vector<Attribute> attributes_;
};

class FrameBuffer {
    public:
        FrameBuffer(int w,int h, GLenum COLOR,bool use_depth=true)
            :w_(w),h_(h)
        {
            glGenFramebuffers(1,&frameBuf_);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuf_);
            frameTex_.reset( new Texture( Texture::createEmptyTexture(w,h,COLOR)));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D, frameTex_->get(), 0);
            if (use_depth){
                depthTex_.reset(
                        new Texture(Texture::createEmptyTexture(w,h,GL_DEPTH_COMPONENT))
                        );
                glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,w,h);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                        GL_TEXTURE_2D,
                        depthTex_->get(), 0);
            }
            glBindFramebuffer(GL_FRAMEBUFFER,0);
        }

        void bind(){
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuf_);
            glViewport(0, 0, w_, h_);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        }
        GLuint getFrameTexture(){
            return frameTex_->get();
        }

    private:
        int w_,h_;
        GLuint frameBuf_ ;
        std::unique_ptr<Texture> frameTex_,depthTex_;
};

#endif //__BUFFER_OBJECT_H
