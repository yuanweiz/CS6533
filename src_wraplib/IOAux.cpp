#include "IOAux.h"
std::string getCurrentDirectoryHelper__(const char* filename ){
	std::string buff(filename);
	auto i=buff.find_last_of('/');
	buff.erase(i,buff.size());
	return buff;
}
