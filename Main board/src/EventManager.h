#ifndef __EVENT_MANAGER__
#define __EVENT_MANAGER__

#include <vector>
#include <queue>
#include <Time.h>
#include "Event.h"
#include "EventReceiver.h"

#define EVENT_KEY       1
#define EVENT_TIME      2
#define EVENT_TIMER     3

/*****************************************************************************/
class KeyEvent: public Event {
public:
    KeyEvent(int key);
    int key;
};

/*****************************************************************************/
class TimeEvent: public Event {
public:
    TimeEvent(time_t time);
    time_t time;
};

/*****************************************************************************/
class TimerEvent: public Event {
public:
    TimerEvent();
};

/*****************************************************************************/
class RegisteredReceiver {
public:
    RegisteredReceiver(EventReceiver *per, int type);
    EventReceiver *pEventReceiver;
    int type;
    bool fired;
};

/*****************************************************************************/
class classEventManager {
public:
    void addListener(int type, EventReceiver *per);
    void removeListener(EventReceiver *per);
    void queueEvent (Event* pevent);
    void processEvents();

private:
    std::queue<Event*> queueEvents;
    std::vector<RegisteredReceiver> listReceivers;
};

extern classEventManager EventManager;

#endif
