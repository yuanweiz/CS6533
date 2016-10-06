#ifndef __TIMER_H
#define __TIMER_H
#include <sys/time.h>
#include <sys/types.h>
class Timer {
	public:
	static int64_t getTimeOfDay(){
		struct timeval tv;
			::gettimeofday(&tv,nullptr);
			return static_cast<int64_t>(tv.tv_sec)*1000000
				+(tv.tv_usec);
	}

	Timer (){
        time_elapsed_ = 0;
        last_tic_ = getTimeOfDay();
        running_ = false;
    }
    void start (){
        running_ = true;
        last_tic_ = getTimeOfDay();
    }
    void stop(){
        running_ = false;
        int64_t now = getTimeOfDay();
        time_elapsed_ += (now - last_tic_);
        last_tic_ = now;
    }

    //return time elapsed in usec
    int64_t timeElapsed(){
        if (running_){
            return getTimeOfDay()-last_tic_ + time_elapsed_;
        }
        else {
            return time_elapsed_;
        }
    }
	private:
	static int64_t last_tic_;
    static int64_t time_elapsed_;
    bool running_;
};

#endif //__TIMER_H
