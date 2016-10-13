#ifndef __IOAUX_H
#define __IOAUX_H

#include <string>
std::string getCurrentDirectoryHelper__(const char*);
#define getCurrentDirectory() getCurrentDirectoryHelper__(__FILE__)

#endif// __IOAUX_H
