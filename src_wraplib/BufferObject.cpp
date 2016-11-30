#include "BufferObject.h"
#include "Attribute.h"
void VertexBuffer::setAttributePointers(){
    for (auto & attrib : attributes_){
        if (attrib.enabled())
            attrib.vertexAttribPointer();
    }
}
