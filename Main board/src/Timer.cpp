#include "Arduino.h"
#include "Timer.h"
#include "EventManager.h"

Timer::Timer(const char *pname, unsigned long duration, TimerHandler handler) {
    strcpy (name, pname);
    this->duration = duration;
    this->start = millis();
    this->handler = handler;
}

bool Timer::checkExpired() {
    if (millis() - start > duration) {
        if (handler) {
            (*handler)();
        }
        return true;
    } else {
        return false;
    }
}

classTimerManager TimerManager;

void classTimerManager::loop() {
    if (pTimer) {
        if (pTimer->checkExpired()) {
            //EventManager.queueEvent(new TimerEvent());
            delete pTimer;
            pTimer = 0;
        }
    }
}

void classTimerManager::start(Timer *ptimer) {
    pTimer = ptimer;
}

