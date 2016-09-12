#ifndef __TIMER_H
#define __TIMER_H
#include <sys/time.h>
#include <sys/types.h>
class Timer {
	public:
	static int64_t getTimeInterval(){
		if (cached == 0 ) {
			//never called before
			cached = getTimeOfDay();
			return 0;
		}
		else{
			auto old = cached;
			cached = getTimeOfDay();
			return cached - old;
		}
	}
	static int64_t getTimeOfDay(){
		struct timeval tv;
			::gettimeofday(&tv,nullptr);
			return static_cast<int64_t>(tv.tv_usec)*100000
				+(tv.tv_sec);
	}

	Timer (){}
	private:
	static int64_t cached;
};

#endif //__TIMER_H
