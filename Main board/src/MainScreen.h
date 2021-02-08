#ifndef __MAIN_SCREEN__
#define __MAIN_SCREEN__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classMainScreen: public Screen, EventReceiver {
public:
    virtual void onActivate();
    virtual void onDeactivate();
    virtual void onEvent(Event* pevent);

private:
    time_t currentTime;
    void handleTimeEvent (TimeEvent *pevent);
    void handleKeyEvent (KeyEvent *pevent);
    void handleAlarmEvent (AlarmEvent *pevent);
    void showTime();
    void showAlarmTime();
    void toggleAlarm();
};

extern classMainScreen MainScreen;

#endif