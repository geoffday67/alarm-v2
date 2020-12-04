#ifndef __TIMER__
#define __TIMER__

typedef void (*TimerHandler) (void);

class Timer {
friend class classTimerManager;

public:
    Timer(const char *pname, unsigned long duration, TimerHandler handler = 0);
    bool checkExpired();

private:
    char name [32];
    unsigned long start, duration;
    TimerHandler handler;
};

class classTimerManager {
private:
    Timer *pTimer = 0;

public:
    void start(Timer *ptimer);
    void loop();
};

extern classTimerManager TimerManager;

#endif