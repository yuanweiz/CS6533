#ifndef __TASK_H
#define __TASK_H
#include <GL/glew.h>
#include <vector>
#include "Program.h"
#include <assert.h>
#include "Wrapper.h"
#include "Uniform.h"
#include <memory>

class Program;
class Task{
    public:
        Task(Program*);

        //not really do bindings here, delay this procedure to initOnce()
        void setInputTexture(const char* uniformName, GLuint texUnit, GLuint tex){
            auto utex = std::make_shared<Uniform1i> (program_,uniformName);
            if (input_.size()<=texUnit){
                input_.resize(texUnit+1);
                inputTextures_.resize(texUnit+1);
            }
            input_[texUnit]=utex;
            inputTextures_[texUnit]=tex;
        }

        void setOutputTexture(GLuint tex){
            output_ = tex;
        }

        void initOnce(){
            //set up input and output target here
            assert((GLuint)ogl::getCurrentProgram() == program_->get());
            for (decltype(input_.size()) i=0;i<input_.size();++i){
                auto & ptr = input_[i];
                if (ptr){
                    ptr->setValue(i);
                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, inputTextures_[i] );
                }
            }
        }
        void draw(){
            program_->useThis();
            if (!inited){
                initOnce();
            }
            assert((GLuint)ogl::getCurrentProgram() == program_->get());
            glBindFramebuffer(GL_FRAMEBUFFER, output_); //0 for screen
            drawImpl();
        }
    protected:
        std::vector<std::shared_ptr<Uniform1i>> input_;
        std::vector<GLuint> inputTextures_;
        GLuint output_; //default=0
        virtual void initImpl()=0;
        virtual void drawImpl()=0;
        Program * program_;
        bool inited;

};
#endif// __TASK_H
