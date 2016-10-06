#ifndef __MOUSE_H
#define __MOUSE_H
#include "config.h"
#include <string>
#include "Timer.h"
class KeyboardEventEntry{
    private:
        std::string keyStroke_;
        double *pval_;
        double rate_;
        Timer timer_;
};

#endif //__MOUSE_H
