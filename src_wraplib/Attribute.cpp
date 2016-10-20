#include "Attribute.h"
#include "Program.h"
Attribute::Attribute(Program * program, const char* name)
    :handle_(program->getAttribute(name))
{
}
