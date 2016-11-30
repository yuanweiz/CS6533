#include "Attribute.h"
#include "BufferObject.h"
#include "Program.h"
#include "Error.h"

Attribute::Attribute(Program * program, VertexBuffer* vbo, const char* name,
                int length, void* offsetInBytes)
    :program_(program),
    vbo_(vbo),
    handle_(program->getAttribute(name)),
    length_(length),
    offsetInBytes_(offsetInBytes),
    enabled_(false)
{
    this->enable();
    vbo->addAttribute(*this);
}
void Attribute::vertexAttribPointer(){
    assert( ogl::getCurrentArrayBuffer() == vbo_->get());
    assert( (GLuint)ogl::getCurrentProgram() == program_->get());
    glVertexAttribPointer(get(),length_,GL_FLOAT,GL_FALSE,
            sizeof (VertexBuffer::data_type),offsetInBytes_ );
}
